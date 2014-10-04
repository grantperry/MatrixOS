[BITS 32]

section .text                           ; this is the code section
global _start                           ; _start is the entry point and needs global scope to be 'seen' by the
                                            ; linker&nbsp;–    equivalent to main() in C/C++
_start:                                 ; procedure start
	mov	eax, 0                   ; specify the sys_write function code (from OS vector table)
	mov	ebx, str                 ; move start _address_ of string message to ecx regist
	int	80h                      ; tell kernel to perform the system call we just set up 
	nop
	nop
	cli
	hlt
	ret

section .data                           ; section for initialized data
str:     db 'Hello world!', 0Ah         ; message string with new-line char at the end (10 decimal)
                                            ; from the str's start address