// MatrixOS - task.c
// Implements the functionality needed to multitask.

#include <task.h>
#include <paging.h>

#define KERNEL_PROC_NAME "kernel"

//The currently running task.
volatile task_t *current_task;

//The start of the task linked list.
volatile task_t *ready_queue;

u8int TASKING_ON = 0;

//Some externs are needed to access members in paging.c...
extern page_directory_t *kernel_directory;
extern page_directory_t *current_directory;
extern void alloc_frame ( page_t*,int,int );
extern u32int initial_esp;
extern u32int read_eip();

//The next available process ID.
u32int next_pid = 1;
u32int nTasks = 0;

s8int initialise_tasking() {
	printf ( "Initalizing Tasking." );

	move_stack ( ( void* ) 0x10000000, 0x2000 );

	u32int *stack;
	task_t *task = ( task_t* ) kmalloc ( sizeof ( task_t ) );
	current_task = ready_queue = task;
	task->stack = kmalloc ( 0x1000 ) + 0x1000;	// Allocate 4 kilobytes of space
	task->esp = task->stack;
	task->originalStack = task->stack - 0x1000; //set the originalStack to its starting location

	// Clone the address space.
	task->page_directory = current_directory;

	task->priority = PRIO_LOW;
	task->time_to_run = 10;
	task->time_running = 0;
	task->ready_to_run = TRUE;

	/*for the most part, this process will not be running anything so the average
	 * burst time will be near 0 */
	task->burst_time = 0;
	task->averaged_priority = 0;

	nTasks++;

	stack = ( u32int* ) task->stack;

// processor data (iret)
	*--stack = 0x202;	// EFLAGS
	*--stack = 0x08;	// CS
//~ *--stack = (u32int)func; // EIP
	*--stack = 0;	// EIP
	task->eip = 0;

// pusha
	*--stack = 0;	// EDI
	*--stack = 0;	// ESI
	*--stack = 0;	// EBP
	task->ebp = 0;

	*--stack = 0;	// NULL
	*--stack = 0;	// EBX
	*--stack = 0;	// EDX
	*--stack = 0;	// ECX
	*--stack = 0;	// EAX

// data segments
	*--stack = 0x10;	// DS
	*--stack = 0x10;	// ES
	*--stack = 0x10;	// FS
	*--stack = 0x10;	// GS

	task->id = next_pid++;
//~ task->id = 0;
	task->stack = ( u32int ) stack;
	task->thread = 0;
	task->thread_flags = 0;

	strcpy ( task->name, KERNEL_PROC_NAME );
	strcpy ( task->name+strlen ( KERNEL_PROC_NAME ), '\000' );

	task->next = 0;

	asm volatile ( "sti" );

	return 0;
}

void move_stack ( void *new_stack_start, u32int size ) {
	u32int i;

	// Allocate some space for the new stack.
	for ( i = ( u32int ) new_stack_start;
			i >= ( ( u32int ) new_stack_start - size );
			i -= 0x1000 ) {
		// General-purpose stack is in user-mode.
		alloc_frame ( get_page ( i, 1, current_directory ), 0 /* User mode */, 1 /* Is writable */ );
	}

	// Flush the TLB by reading and writing the page directory address again.
	u32int pd_addr;
	asm volatile ( "mov %%cr3, %0" : "=r" ( pd_addr ) );
	asm volatile ( "mov %0, %%cr3" : : "r" ( pd_addr ) );

	// Old ESP and EBP, read from registers.
	u32int old_stack_pointer;
	asm volatile ( "mov %%esp, %0" : "=r" ( old_stack_pointer ) );
	u32int old_base_pointer;
	asm volatile ( "mov %%ebp, %0" : "=r" ( old_base_pointer ) );

	// Offset to add to old stack addresses to get a new stack address.
	u32int offset = ( u32int ) new_stack_start - initial_esp;

	// New ESP and EBP.
	u32int new_stack_pointer = old_stack_pointer + offset;
	u32int new_base_pointer = old_base_pointer + offset;

	// Copy the stack.
	memcpy ( ( void* ) new_stack_pointer, ( void* ) old_stack_pointer, initial_esp-old_stack_pointer );

	// Backtrace through the original stack, copying new values into
	// the new stack.
	for ( i = ( u32int ) new_stack_start; i > ( u32int ) new_stack_start - size; i -= 4 ) {
		u32int tmp = * ( u32int* ) i;

		// If the value of tmp is inside the range of the old stack, assume it is a base pointer
		// and remap it. This will unfortunately remap ANY value in this range, whether they are
		// base pointers or not.
		if ( ( old_stack_pointer < tmp ) && ( tmp < initial_esp ) ) {
			tmp = tmp + offset;
			u32int *tmp2 = ( u32int* ) i;
			*tmp2 = tmp;
		}
	}

	// Change stacks.
	asm volatile ( "mov %0, %%esp" : : "r" ( new_stack_pointer ) );
	asm volatile ( "mov %0, %%ebp" : : "r" ( new_base_pointer ) );

}

int getpid() {
	return current_task->id;
}

char gettaskname() {
	return ( int ) current_task->name;
}

void exit() {
	asm volatile ( "cli" );
	//Just incase the removal doesn't work 100%
	//we make sure we are using as little time as possible
	current_task->priority = PRIO_DEAD;
	current_task->time_to_run = 0;
	current_task->ready_to_run = 0;

	//Find previous task
	task_t *task_prev = 0;
	task_t *task_r = ( task_t* ) ready_queue;

	for ( ; task_r->next != 0; task_r = task_r->next ) {
		if ( task_r->next == current_task ) {
			//We got the previous task
			task_prev = task_r;
			break; //Don't bother with the rest of the list
		}
	}

	//We didn't find the task and it is not the ready_queue
	if ( !task_prev && current_task != ready_queue ) {
		return;
	}

	//if our current task is the ready_queue then set the starting task as the next task after current_task
	if ( current_task == ready_queue ) {
		ready_queue = current_task->next;

	} else {
		task_prev->next = current_task->next;
	}

	nTasks--;

	//Free the memory
	kfree ( ( void* ) current_task );
	kfree ( ( void* ) current_task->originalStack );
	kfree ( ( void* ) current_task->page_directory );

	asm volatile ( "sti" ); //Restart Interrupts before switching - stop CPU lockup
	switch_task(); //Don't waste any time
}

void switch_task() {
	// If we haven't initialised tasking yet, just return.
	if ( !current_task ) {
		return;
	}


	asm volatile ( "cli" );

	// Read esp, ebp now for saving later on.
	u32int esp, ebp, eip;
	asm volatile ( "mov %%esp, %0" : "=r" ( esp ) );
	asm volatile ( "mov %%ebp, %0" : "=r" ( ebp ) );


	// Read the instruction pointer. We do some cunning logic here:
	// One of two things could have happened when this function exits -
	// (a) We called the function and it returned the EIP as requested.
	// (b) We have just switched tasks, and because the saved EIP is essentially
	// the instruction after read_eip(), it will seem as if read_eip has just
	// returned.
	// In the second case we need to return immediately. To detect it we put a dummy
	// value in EAX further down at the end of this function. As C returns values in EAX,
	// it will look like the return value is this dummy value! (0x12345).
	eip = read_eip();

	// Have we just switched tasks?
	if ( eip == 0x12345 ) {
		return;
	}

	// No, we didn't switch tasks. Let's save some register values and switch.
	current_task->eip = eip;
	current_task->esp = esp;
	current_task->ebp = ebp;

	// Get the next task to run.
	current_task = current_task->next;

	// If we fell off the end of the linked list start again at the beginning.
	if ( !current_task ) {
		current_task = ready_queue;
	}

	eip = current_task->eip;
	esp = current_task->esp;
	ebp = current_task->ebp;

	// Make sure the memory manager knows we've changed page directory.
	if ( current_task->page_directory ) {
		current_directory = current_task->page_directory;
	}

	// Here we:
	// * Stop interrupts so we don't get interrupted.
	// * Temporarily put the new EIP location in ECX.
	// * Load the stack and base pointers from the new task struct.
	// * Change page directory to the physical address (physicalAddr) of the new directory.
	// * Put a dummy value (0x12345) in EAX so that above we can recognise that we've just
	// switched task.
	// * Restart interrupts. The STI instruction has a delay - it doesn't take effect until after
	// the next instruction.
	// * Jump to the location in ECX (remember we put the new EIP in there).
	asm volatile ( " \
		cli; \
		mov %0, %%ecx; \
		mov %1, %%esp; \
		mov %2, %%ebp; \
		mov %3, %%cr3; \
		mov $0x12345, %%eax; \
		sti; \
		jmp *%%ecx "
				   : : "r" ( eip ), "r" ( esp ), "r" ( ebp ), "r" ( current_directory->physicalAddr ) );

	asm volatile ( "sti" );
	printf ( "nt" );
}

u32int start_task ( u32int priority, u32int burst_time, void ( *func ) (), void *arg, char *task_Name ) {

	asm volatile ( "cli" );

	//Take a pointer to this process' task struct for later reference.
	//task_t *parent_task = (task_t*)current_task;

	u32int id = next_pid++;

	u32int *stack;
	task_t *task = ( task_t* ) kmalloc ( sizeof ( task_t ) );
	task->stack = kmalloc ( 0x1000 ) + 0x1000;	// Allocate 4 kilobytes of space
	task->esp = task->stack;
	task->originalStack = task->stack - 0x1000; //set the originalStack to its starting location

	// Clone the address space.
	//page_directory_t *directory = clone_directory(current_directory);
	//task->page_directory = directory;

	//TODO Set this to make a new page directory when in user mode.
	//this is a task (process) not a fork, we do not need a cloned address space
	task->page_directory = 0;

	task->priority = priority;
	task->time_to_run = 0;
	task->time_running = 0;
	task->ready_to_run = TRUE;

	task->burst_time = burst_time;
	task->averaged_priority = priority + burst_time;

	nTasks++;

	stack = ( u32int* ) task->stack;

	// processor data (iret)
	*--stack = 0x202;	// EFLAGS
	*--stack = 0x08;	// CS
	*--stack = ( u32int ) func;	// EIP
	task->eip = ( u32int ) func;

	// pusha
	*--stack = 0;	// EDI
	*--stack = 0;	// ESI
	*--stack = 0;	// EBP
	task->ebp = 0;
	*--stack = 0;	// NULL
	*--stack = 0;	// EBX
	*--stack = 0;	// EDX
	*--stack = 0;	// ECX
	*--stack = 0;	// EAX

	// data segments
	*--stack = 0x10;	// DS
	*--stack = 0x10;	// ES
	*--stack = 0x10;	// FS
	*--stack = 0x10;	// GS

	task->id = id;
	task->stack = ( u32int ) stack;
	task->thread = func;
	task->thread_flags = ( u32int ) arg;
	strcpy ( task->name, task_Name );

	task->next = 0;

	task_t *tmp_task;
	tmp_task = ( task_t* ) ready_queue;

	while ( tmp_task->next != 0 ) {
		tmp_task = tmp_task->next;

	}

	// ...And extend it.
	tmp_task->next = task;

	asm volatile ( "sti" );

	return id;
}

void enable_tasking() {
	TASKING_ON = 1;
}

void disable_tasking() {
	TASKING_ON = 0;
}

/*void asm_switch_to_usermode(void);

void switch_to_user_mode()
{
   // Set up a stack structure for switching to user mode.
   set_kernel_stack(current_task->stack+0x2000);
	asm_switch_to_usermode();
	//syscall_monitor_write("Hello,user\n");
     //sleep(1);
     if(current_task->id != 1)  {
		syscall_exit();
	}
}*/


s8int switch_to_user_mode() {
	printf ( "Switching to UserMode" );
	// Set up our kernel stack.
	//set_kernel_stack(current_task->kernel_stack+KERNEL_STACK_SIZE);

	// Set up a stack structure for switching to user mode.
	asm volatile ( " \
cli; \
mov $0x23, %ax; \
mov %ax, %ds; \
mov %ax, %es; \
mov %ax, %fs; \
mov %ax, %gs; \
\
\
mov %esp, %eax; \
pushl $0x23; \
pushl %esp; \
pushf; \
pushl $0x1B; \
push $1f; \
iret; \
1: \
" );
	return 0;
}
