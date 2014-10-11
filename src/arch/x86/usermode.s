; MatrixOS - usermode.s
[bits 32]

global asm_switch_to_usermode

section .text
asm_switch_to_usermode:
	cli
	mov ax, 0x23
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov eax, esp
	push 0x23
	push eax
	pushf
	
	pop eax ; Get EFLAGS back into EAX. The only way to read EFLAGS is to pushf then pop.
	or eax, 0x200 ; Set the IF flag.
	push eax ; Push the new EFLAGS value back onto the stack.
	
	push 0x1B
	push .4
	iret

.4:
	ret
