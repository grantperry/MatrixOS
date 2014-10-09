// MatrixOS - task.h
// Defines the structures and prototypes needed to multitask.

#ifndef TASK_H
#define TASK_H

#include "common.h"
#include "paging.h"

s8int initialise_tasking();

//process priorities
#define PRIO_DEAD 9900
#define PRIO_IDLE 0
#define PRIO_LOW 100
#define PRIO_MED 200
#define PRIO_HIGH 300

//process length times
#define PROC_VERY_LONG 10
#define PROC_LONG 30
#define PROC_MED 50
#define PROC_SHORT 70
#define PROC_VERY_SHORT 90

/*Causes the current process' stack to be forcibly moved to a new location*/
void move_stack ( void *new_stack_start, u32int size );

// This structure defines a 'task' - a process.
typedef struct task {
	u32int id; //Process ID

	char name[32]; //Process name
	//~ u32int name; //Process name

	u32int esp, ebp; //Stack and base pointers.
	u32int eip; //Instruction pointer.

	u32int priority; //the priority of the process
	u32int burst_time; //the process burst time
	u32int averaged_priority; //A number that indicates average priority (priority and burst time)

	u32int time_to_run; //Time left on quanta
	u32int time_running; //Time spent running
	u32int ready_to_run; //is the process ready to run

	page_directory_t *page_directory; // Page directory.

	void ( *thread ) (); //thread entry point
	u32int thread_flags; //location of an array of thread arguments to go allong with that function

	u32int stack; //kernel stack location.
	u32int originalStack; //the original kernel stack location.

	struct task *next; //the next task in a linked list.
} task_t;

int getpid();

void switch_task();

u32int start_task ( u32int priority, u32int burst_time, void ( *func ) (), void *arg, char *task_Name );

void enable_tasking();

void disable_tasking();

s8int switch_to_user_mode();
#endif
