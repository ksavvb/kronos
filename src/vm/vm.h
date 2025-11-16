#ifndef KRONOS_VM_H
#define KRONOS_VM_H

#include "../compiler/compiler.h"
#include "../core/runtime.h"
#include <stddef.h>

#define STACK_MAX 1024
#define GLOBALS_MAX 256
#define FUNCTIONS_MAX 128
#define CALL_STACK_MAX 256
#define LOCALS_MAX 64

// Function definition
typedef struct {
  char *name;
  char **params;
  size_t param_count;
  Bytecode bytecode; // Full bytecode structure
} Function;

// Call frame for function calls
typedef struct {
  Function *function;
  uint8_t *return_ip;        // Where to return to
  Bytecode *return_bytecode; // Which bytecode to return to
  KronosValue **frame_start; // Start of this frame's stack

  // Local variables (includes parameters)
  struct {
    char *name;
    KronosValue *value;
    bool is_mutable;
    char *type_name; // NULL if no type restriction
  } locals[LOCALS_MAX];
  size_t local_count;
} CallFrame;

// Virtual machine state
typedef struct KronosVM {
  // Value stack
  KronosValue *stack[STACK_MAX];
  KronosValue **stack_top;

  // Call stack
  CallFrame call_stack[CALL_STACK_MAX];
  size_t call_stack_size;
  CallFrame *current_frame;

  // Global variables
  struct {
    char *name;
    KronosValue *value;
    bool is_mutable;
    char *type_name; // NULL if no type restriction
  } globals[GLOBALS_MAX];
  size_t global_count;

  // Functions
  Function *functions[FUNCTIONS_MAX];
  size_t function_count;

  // Instruction pointer
  uint8_t *ip;

  // Current bytecode
  Bytecode *bytecode;
} KronosVM;

// VM lifecycle
KronosVM *vm_new(void);
void vm_free(KronosVM *vm);

// Execute bytecode
int vm_execute(KronosVM *vm, Bytecode *bytecode);

// Variable management
void vm_set_global(KronosVM *vm, const char *name, KronosValue *value, bool is_mutable, const char *type_name);
KronosValue *vm_get_global(KronosVM *vm, const char *name);
void vm_set_local(CallFrame *frame, const char *name, KronosValue *value, bool is_mutable, const char *type_name);
KronosValue *vm_get_local(CallFrame *frame, const char *name);
KronosValue *vm_get_variable(KronosVM *vm, const char *name); // Try local, then global

// Function management
void vm_define_function(KronosVM *vm, Function *func);
Function *vm_get_function(KronosVM *vm, const char *name);
void function_free(Function *func);

#endif // KRONOS_VM_H
