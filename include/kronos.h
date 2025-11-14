#ifndef KRONOS_H
#define KRONOS_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Public API for Kronos runtime
// This header provides the main interface for executing Kronos programs

// Forward declarations
typedef struct KronosVM KronosVM;

// Initialize and destroy the VM
KronosVM* kronos_vm_new(void);
void kronos_vm_free(KronosVM* vm);

// Execute a Kronos source file
int kronos_run_file(KronosVM* vm, const char* filepath);

// Execute a Kronos source string
int kronos_run_string(KronosVM* vm, const char* source);

// REPL mode
void kronos_repl(void);

#endif // KRONOS_H

