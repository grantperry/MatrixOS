// task.h - Defines the structures and prototypes needed to multitask.

#ifndef TASK_H
#define TASK_H

#include "common.h"
#include "paging.h"

#define KERNEL_STACK_SIZE 2048       // Use a 2kb kernel stack.

// This structure defines a 'task' - a process.
typedef struct task {
	int id;                // Process ID.
	u32int esp, ebp;       // Stack and base pointers.
	u32int eip;            // Instruction pointer.
	page_directory_t *page_directory; // Page directory.
	u32int kernel_stack;   // Kernel stack location.
	u32int ready_to_run;   //is the process ready to run
	struct task *next;     // The next task in a linked list.
} task_t;

// The currently running task.
volatile task_t *current_task;

// Initialises the tasking system.
s8int initialise_tasking();

// Called by the timer hook, this changes the running process.
void task_switch();

// Forks the current process, spawning a new one with a different
// memory space.
int fork();

// Causes the current process' stack to be forcibly moved to a new location.
void move_stack ( void *new_stack_start, u32int size );

// Returns the pid of the current process.
int getpid();

void switch_to_user_mode();

#endif
