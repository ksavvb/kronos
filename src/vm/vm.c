#include "vm.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Create new VM
KronosVM* vm_new(void) {
    KronosVM* vm = malloc(sizeof(KronosVM));
    if (!vm) return NULL;
    
    vm->stack_top = vm->stack;
    vm->global_count = 0;
    vm->function_count = 0;
    vm->ip = NULL;
    vm->bytecode = NULL;
    vm->return_value = NULL;
    
    return vm;
}

// Free VM
void vm_free(KronosVM* vm) {
    if (!vm) return;
    
    // Release all values on stack
    while (vm->stack_top > vm->stack) {
        vm->stack_top--;
        value_release(*vm->stack_top);
    }
    
    // Release global variables
    for (size_t i = 0; i < vm->global_count; i++) {
        free(vm->globals[i].name);
        value_release(vm->globals[i].value);
    }
    
    // Release functions
    for (size_t i = 0; i < vm->function_count; i++) {
        function_free(vm->functions[i]);
    }
    
    // Release return value
    if (vm->return_value) {
        value_release(vm->return_value);
    }
    
    free(vm);
}

// Free a function
void function_free(Function* func) {
    if (!func) return;
    
    free(func->name);
    for (size_t i = 0; i < func->param_count; i++) {
        free(func->params[i]);
    }
    free(func->params);
    free(func->bytecode);
    
    for (size_t i = 0; i < func->const_count; i++) {
        value_release(func->constants[i]);
    }
    free(func->constants);
    
    free(func);
}

// Define a function
void vm_define_function(KronosVM* vm, Function* func) {
    if (vm->function_count >= FUNCTIONS_MAX) {
        fprintf(stderr, "Too many functions\n");
        return;
    }
    
    vm->functions[vm->function_count++] = func;
}

// Get a function by name
Function* vm_get_function(KronosVM* vm, const char* name) {
    for (size_t i = 0; i < vm->function_count; i++) {
        if (strcmp(vm->functions[i]->name, name) == 0) {
            return vm->functions[i];
        }
    }
    return NULL;
}

// Stack operations
static void push(KronosVM* vm, KronosValue* value) {
    if (vm->stack_top >= vm->stack + STACK_MAX) {
        fprintf(stderr, "Stack overflow\n");
        return;
    }
    *vm->stack_top = value;
    vm->stack_top++;
    value_retain(value); // Retain while on stack
}

static KronosValue* pop(KronosVM* vm) {
    if (vm->stack_top <= vm->stack) {
        fprintf(stderr, "Stack underflow\n");
        return value_new_nil();
    }
    vm->stack_top--;
    KronosValue* val = *vm->stack_top;
    return val;
}

static KronosValue* peek(KronosVM* vm, int distance) {
    return vm->stack_top[-1 - distance];
}

// Global variable management
void vm_set_global(KronosVM* vm, const char* name, KronosValue* value) {
    // Check if variable already exists
    for (size_t i = 0; i < vm->global_count; i++) {
        if (strcmp(vm->globals[i].name, name) == 0) {
            value_release(vm->globals[i].value);
            vm->globals[i].value = value;
            value_retain(value);
            return;
        }
    }
    
    // Add new global
    if (vm->global_count >= GLOBALS_MAX) {
        fprintf(stderr, "Too many global variables\n");
        return;
    }
    
    vm->globals[vm->global_count].name = strdup(name);
    vm->globals[vm->global_count].value = value;
    value_retain(value);
    vm->global_count++;
}

KronosValue* vm_get_global(KronosVM* vm, const char* name) {
    for (size_t i = 0; i < vm->global_count; i++) {
        if (strcmp(vm->globals[i].name, name) == 0) {
            return vm->globals[i].value;
        }
    }
    
    fprintf(stderr, "Undefined variable: %s\n", name);
    return value_new_nil();
}

// Read byte from bytecode
static uint8_t read_byte(KronosVM* vm) {
    return *vm->ip++;
}

// Read constant from pool
static KronosValue* read_constant(KronosVM* vm) {
    uint8_t idx = read_byte(vm);
    return vm->bytecode->constants[idx];
}

// Execute bytecode
int vm_execute(KronosVM* vm, Bytecode* bytecode) {
    if (!vm || !bytecode) return -1;
    
    vm->bytecode = bytecode;
    vm->ip = bytecode->code;
    
    while (1) {
        uint8_t instruction = read_byte(vm);
        
        switch (instruction) {
            case OP_LOAD_CONST: {
                KronosValue* constant = read_constant(vm);
                push(vm, constant);
                break;
            }
            
            case OP_LOAD_VAR: {
                KronosValue* name_val = read_constant(vm);
                if (name_val->type != VAL_STRING) {
                    fprintf(stderr, "Variable name must be string\n");
                    return -1;
                }
                KronosValue* value = vm_get_global(vm, name_val->as.string.data);
                push(vm, value);
                break;
            }
            
            case OP_STORE_VAR: {
                KronosValue* name_val = read_constant(vm);
                if (name_val->type != VAL_STRING) {
                    fprintf(stderr, "Variable name must be string\n");
                    return -1;
                }
                KronosValue* value = pop(vm);
                vm_set_global(vm, name_val->as.string.data, value);
                value_release(value); // Release our reference
                break;
            }
            
            case OP_PRINT: {
                KronosValue* value = pop(vm);
                value_print(value);
                printf("\n");
                value_release(value);
                break;
            }
            
            case OP_ADD: {
                KronosValue* b = pop(vm);
                KronosValue* a = pop(vm);
                
                if (a->type == VAL_NUMBER && b->type == VAL_NUMBER) {
                    KronosValue* result = value_new_number(a->as.number + b->as.number);
                    push(vm, result);
                    value_release(result); // Push retains it
                } else {
                    fprintf(stderr, "Operands must be numbers\n");
                }
                
                value_release(a);
                value_release(b);
                break;
            }
            
            case OP_SUB: {
                KronosValue* b = pop(vm);
                KronosValue* a = pop(vm);
                
                if (a->type == VAL_NUMBER && b->type == VAL_NUMBER) {
                    KronosValue* result = value_new_number(a->as.number - b->as.number);
                    push(vm, result);
                    value_release(result);
                } else {
                    fprintf(stderr, "Operands must be numbers\n");
                }
                
                value_release(a);
                value_release(b);
                break;
            }
            
            case OP_MUL: {
                KronosValue* b = pop(vm);
                KronosValue* a = pop(vm);
                
                if (a->type == VAL_NUMBER && b->type == VAL_NUMBER) {
                    KronosValue* result = value_new_number(a->as.number * b->as.number);
                    push(vm, result);
                    value_release(result);
                } else {
                    fprintf(stderr, "Operands must be numbers\n");
                }
                
                value_release(a);
                value_release(b);
                break;
            }
            
            case OP_DIV: {
                KronosValue* b = pop(vm);
                KronosValue* a = pop(vm);
                
                if (a->type == VAL_NUMBER && b->type == VAL_NUMBER) {
                    if (b->as.number == 0) {
                        fprintf(stderr, "Division by zero\n");
                        value_release(a);
                        value_release(b);
                        return -1;
                    }
                    KronosValue* result = value_new_number(a->as.number / b->as.number);
                    push(vm, result);
                    value_release(result);
                } else {
                    fprintf(stderr, "Operands must be numbers\n");
                }
                
                value_release(a);
                value_release(b);
                break;
            }
            
            case OP_EQ: {
                KronosValue* b = pop(vm);
                KronosValue* a = pop(vm);
                bool result = value_equals(a, b);
                push(vm, value_new_bool(result));
                value_release(a);
                value_release(b);
                break;
            }
            
            case OP_NEQ: {
                KronosValue* b = pop(vm);
                KronosValue* a = pop(vm);
                bool result = !value_equals(a, b);
                push(vm, value_new_bool(result));
                value_release(a);
                value_release(b);
                break;
            }
            
            case OP_GT: {
                KronosValue* b = pop(vm);
                KronosValue* a = pop(vm);
                
                if (a->type == VAL_NUMBER && b->type == VAL_NUMBER) {
                    bool result = a->as.number > b->as.number;
                    push(vm, value_new_bool(result));
                } else {
                    fprintf(stderr, "Operands must be numbers\n");
                }
                
                value_release(a);
                value_release(b);
                break;
            }
            
            case OP_LT: {
                KronosValue* b = pop(vm);
                KronosValue* a = pop(vm);
                
                if (a->type == VAL_NUMBER && b->type == VAL_NUMBER) {
                    bool result = a->as.number < b->as.number;
                    push(vm, value_new_bool(result));
                } else {
                    fprintf(stderr, "Operands must be numbers\n");
                }
                
                value_release(a);
                value_release(b);
                break;
            }
            
            case OP_GTE: {
                KronosValue* b = pop(vm);
                KronosValue* a = pop(vm);
                
                if (a->type == VAL_NUMBER && b->type == VAL_NUMBER) {
                    bool result = a->as.number >= b->as.number;
                    push(vm, value_new_bool(result));
                } else {
                    fprintf(stderr, "Operands must be numbers\n");
                }
                
                value_release(a);
                value_release(b);
                break;
            }
            
            case OP_LTE: {
                KronosValue* b = pop(vm);
                KronosValue* a = pop(vm);
                
                if (a->type == VAL_NUMBER && b->type == VAL_NUMBER) {
                    bool result = a->as.number <= b->as.number;
                    push(vm, value_new_bool(result));
                } else {
                    fprintf(stderr, "Operands must be numbers\n");
                }
                
                value_release(a);
                value_release(b);
                break;
            }
            
            case OP_JUMP: {
                int8_t offset = (int8_t)read_byte(vm);
                vm->ip += offset;
                break;
            }
            
            case OP_JUMP_IF_FALSE: {
                uint8_t offset = read_byte(vm);
                KronosValue* condition = peek(vm, 0);
                if (!value_is_truthy(condition)) {
                    vm->ip += offset;
                }
                value_release(pop(vm)); // Pop condition
                break;
            }
            
            case OP_DEFINE_FUNC: {
                // Read function name
                KronosValue* name_val = read_constant(vm);
                uint8_t param_count = read_byte(vm);
                
                // Create function
                Function* func = malloc(sizeof(Function));
                func->name = strdup(name_val->as.string.data);
                func->param_count = param_count;
                func->params = malloc(sizeof(char*) * param_count);
                
                // Read parameter names
                for (size_t i = 0; i < param_count; i++) {
                    KronosValue* param_val = read_constant(vm);
                    func->params[i] = strdup(param_val->as.string.data);
                }
                
                // Read function body start position
                uint8_t body_high = read_byte(vm);
                uint8_t body_low = read_byte(vm);
                size_t body_start = (body_high << 8) | body_low;
                
                // Read jump offset (skip body)
                uint8_t skip_offset = read_byte(vm);
                
                // Calculate body end (before the jump we just read)
                uint8_t* body_end_ptr = vm->ip + skip_offset;
                
                // Copy function body bytecode
                func->bytecode_size = body_end_ptr - vm->ip;
                func->bytecode = malloc(func->bytecode_size);
                memcpy(func->bytecode, vm->ip, func->bytecode_size);
                
                // Copy constants (share with main bytecode for now)
                func->const_count = vm->bytecode->const_count;
                func->constants = vm->bytecode->constants;
                
                // Store function
                vm_define_function(vm, func);
                
                // Skip over function body
                vm->ip = body_end_ptr;
                break;
            }
            
            case OP_CALL_FUNC: {
                KronosValue* name_val = read_constant(vm);
                uint8_t arg_count = read_byte(vm);
                
                // Get function
                Function* func = vm_get_function(vm, name_val->as.string.data);
                if (!func) {
                    fprintf(stderr, "Undefined function: %s\n", name_val->as.string.data);
                    return -1;
                }
                
                if (arg_count != func->param_count) {
                    fprintf(stderr, "Function %s expects %zu args, got %d\n",
                            func->name, func->param_count, arg_count);
                    return -1;
                }
                
                // Pop arguments and bind to parameters
                KronosValue** args = malloc(sizeof(KronosValue*) * arg_count);
                for (int i = arg_count - 1; i >= 0; i--) {
                    args[i] = pop(vm);
                }
                
                // Save current state
                uint8_t* saved_ip = vm->ip;
                Bytecode* saved_bytecode = vm->bytecode;
                size_t saved_global_count = vm->global_count;
                
                // Set parameters as local variables (stored as globals for simplicity)
                for (size_t i = 0; i < arg_count; i++) {
                    vm_set_global(vm, func->params[i], args[i]);
                    value_release(args[i]);
                }
                free(args);
                
                // Create a temporary bytecode structure for the function
                Bytecode temp_bytecode;
                temp_bytecode.code = func->bytecode;
                temp_bytecode.count = func->bytecode_size;
                temp_bytecode.capacity = func->bytecode_size;
                temp_bytecode.constants = func->constants;
                temp_bytecode.const_count = func->const_count;
                temp_bytecode.const_capacity = func->const_count;
                
                // Execute function body by recursively calling vm_execute
                vm->bytecode = &temp_bytecode;
                vm->ip = func->bytecode;
                
                // Execute the function bytecode
                while (vm->ip < func->bytecode + func->bytecode_size) {
                    uint8_t instr = *vm->ip;
                    
                    // Check for return instruction
                    if (instr == OP_RETURN_VAL) {
                        read_byte(vm); // Consume the instruction
                        vm->return_value = pop(vm);
                        break;
                    }
                    
                    // For all other instructions, we need to manually dispatch
                    // This is a simplified approach - ideally we'd refactor to a helper
                    read_byte(vm);
                    vm->ip--; // Back up so the main switch can read it
                    
                    // Re-execute the main instruction handler
                    // We'll break out and let the outer loop handle it
                    // But for now, let's just handle the common cases inline
                    
                    switch (instr) {
                        case OP_LOAD_CONST: {
                            read_byte(vm); // Re-consume
                            KronosValue* constant = read_constant(vm);
                            push(vm, constant);
                            break;
                        }
                        case OP_LOAD_VAR: {
                            read_byte(vm);
                            KronosValue* n = read_constant(vm);
                            KronosValue* val = vm_get_global(vm, n->as.string.data);
                            push(vm, val);
                            break;
                        }
                        case OP_STORE_VAR: {
                            read_byte(vm);
                            KronosValue* n = read_constant(vm);
                            KronosValue* val = pop(vm);
                            vm_set_global(vm, n->as.string.data, val);
                            value_release(val);
                            break;
                        }
                        case OP_PRINT: {
                            read_byte(vm);
                            KronosValue* val = pop(vm);
                            value_print(val);
                            printf("\n");
                            value_release(val);
                            break;
                        }
                        case OP_ADD: {
                            read_byte(vm);
                            KronosValue* b = pop(vm);
                            KronosValue* a = pop(vm);
                            if (a->type == VAL_NUMBER && b->type == VAL_NUMBER) {
                                KronosValue* result = value_new_number(a->as.number + b->as.number);
                                push(vm, result);
                                value_release(result);
                            }
                            value_release(a);
                            value_release(b);
                            break;
                        }
                        case OP_SUB: {
                            read_byte(vm);
                            KronosValue* b = pop(vm);
                            KronosValue* a = pop(vm);
                            if (a->type == VAL_NUMBER && b->type == VAL_NUMBER) {
                                KronosValue* result = value_new_number(a->as.number - b->as.number);
                                push(vm, result);
                                value_release(result);
                            }
                            value_release(a);
                            value_release(b);
                            break;
                        }
                        case OP_MUL: {
                            read_byte(vm);
                            KronosValue* b = pop(vm);
                            KronosValue* a = pop(vm);
                            if (a->type == VAL_NUMBER && b->type == VAL_NUMBER) {
                                KronosValue* result = value_new_number(a->as.number * b->as.number);
                                push(vm, result);
                                value_release(result);
                            }
                            value_release(a);
                            value_release(b);
                            break;
                        }
                        case OP_DIV: {
                            read_byte(vm);
                            KronosValue* b = pop(vm);
                            KronosValue* a = pop(vm);
                            if (a->type == VAL_NUMBER && b->type == VAL_NUMBER && b->as.number != 0) {
                                KronosValue* result = value_new_number(a->as.number / b->as.number);
                                push(vm, result);
                                value_release(result);
                            }
                            value_release(a);
                            value_release(b);
                            break;
                        }
                        default:
                            vm->ip--; // Back up
                            goto function_done;
                    }
                }
                
                function_done:
                // Restore state
                vm->ip = saved_ip;
                vm->bytecode = saved_bytecode;
                
                // Clean up local variables (parameters)
                for (size_t i = saved_global_count; i < vm->global_count; i++) {
                    free(vm->globals[i].name);
                    value_release(vm->globals[i].value);
                }
                vm->global_count = saved_global_count;
                
                // Push return value
                if (vm->return_value) {
                    push(vm, vm->return_value);
                    vm->return_value = NULL;
                } else {
                    push(vm, value_new_nil());
                }
                
                break;
            }
            
            case OP_RETURN_VAL: {
                vm->return_value = pop(vm);
                // In function context, this will be handled by OP_CALL_FUNC
                // In main context, just keep the value
                break;
            }
            
            case OP_POP: {
                value_release(pop(vm));
                break;
            }
            
            case OP_HALT: {
                return 0;
            }
            
            default:
                fprintf(stderr, "Unknown instruction: %d\n", instruction);
                return -1;
        }
    }
    
    return 0;
}

