#ifndef KRONOS_GC_H
#define KRONOS_GC_H

#include "runtime.h"

// Garbage collector for reference counting and cycle detection

// Memory allocation tracking
void gc_init(void);
void gc_cleanup(void);

// Reference counting helpers
void gc_track(KronosValue* val);
void gc_untrack(KronosValue* val);

// Cycle detection (mark-and-sweep for cycles)
void gc_collect_cycles(void);

// Memory statistics
size_t gc_get_allocated_bytes(void);
size_t gc_get_object_count(void);

#endif // KRONOS_GC_H

