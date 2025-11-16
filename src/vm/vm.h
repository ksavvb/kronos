#ifndef KRONOS_VM_H
#define KRONOS_VM_H

#include "../compiler/compiler.h"
#include "../core/runtime.h"
#include <stddef.h>

#define STACK_MAX 1024
#define GLOBALS_MAX 256
#define FUNCTIONS_MAX 128

// Function definition
typedef struct {
    char* name;
    char** params;
    size_t param_count;
    uint8_t* bytecode;
    size_t bytecode_size;
    KronosValue** constants;
    size_t const_count;
} Function;

// Virtual machine state
typedef struct KronosVM {
    // Stack
    KronosValue* stack[STACK_MAX];
    KronosValue** stack_top;
    
    // Global variables
    struct {
        char* name;
        KronosValue* value;
    } globals[GLOBALS_MAX];
    size_t global_count;
    
    // Functions
    Function* functions[FUNCTIONS_MAX];
    size_t function_count;
    
    // Instruction pointer
    uint8_t* ip;
    
    // Current bytecode
    Bytecode* bytecode;
    
    // Return value register
    KronosValue* return_value;
} KronosVM;

// VM lifecycle
KronosVM* vm_new(void);
void vm_free(KronosVM* vm);

// Execute bytecode
int vm_execute(KronosVM* vm, Bytecode* bytecode);

// Variable management
void vm_set_global(KronosVM* vm, const char* name, KronosValue* value);
KronosValue* vm_get_global(KronosVM* vm, const char* name);

// Function management
void vm_define_function(KronosVM* vm, Function* func);
Function* vm_get_function(KronosVM* vm, const char* name);
void function_free(Function* func);

#endif // KRONOS_VM_H

