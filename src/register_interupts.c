#include "register_interupts.h"
#include "monitor.h"
#include "isr.h"

void dec(u32int i);
void dec(u32int i) {
	monitor_write_dec(i);
}
void hex(u32int i) {
	monitor_write_hex(i);
}
void str(char *c);
void str(char *c) {
	monitor_write(c);
}

void irq_Divide_Error(registers_t regs) {
monitor_write("Divide Error!\n");
	dump(regs);
}

void irq_Debug_Exceptions(registers_t regs) {
monitor_write("Debug Exception\n");
	dump(regs);
}

void irq_Intel_Reserved1(registers_t regs) {
monitor_write("Intel Reserved\n");
	dump(regs);
}

void irq_Breakpoint(registers_t regs) {
monitor_write("Breakpoint Reached\n");
	dump(regs);
}
void irq_Overflow(registers_t regs) {
monitor_write("Stack Overflow\n");
	dump(regs);
}
void irq_Bounds_Check(registers_t regs) {
monitor_write("Bounds Check\n");
	dump(regs);
}
void irq_Invalid_Opcode(registers_t regs) {
monitor_write("Invalid Opcode. who the hell wrote this program :)\n");
	dump(regs);
}
void irq_Coprocessor_Not_Availiable(registers_t regs) {
monitor_write("This coprocessor is not avaliable for use!\n");
	dump(regs);
}
void irq_Double_Fault(registers_t regs) {
monitor_write("Dobble Fault. 1 to go :(\n");
	dump(regs);
}
void irq_Coprocessor_Segment_Overrun(registers_t regs) {
monitor_write("Coprocessor Segment Overrun\n");
	dump(regs);
}
void irq_Invalid_TSS(registers_t regs) {
monitor_write("Invalid TSS\n");
	dump(regs);
}
void irq_Segment_Not_Present(registers_t regs) {
monitor_write("what the hell are you lookin at. theres nothing here(Segment Not Present)\n");
	dump(regs);
}
void irq_Stack_Exception(registers_t regs) {
monitor_write("Stack Exception\n");
	dump(regs);
}

void irq_General_Protection_Exception(registers_t regs) {
	monitor_write("Triple Fault !!!\n");
	dump(regs);
asm volatile("hlt");
}
void dump(registers_t regs) {
	str("\n");
	dump_obj("ds", regs.ds);
	str("\n");
	dump_obj("edi", regs.edi);
	dump_obj("ebp", regs.ebp);
	dump_obj("esp", regs.esp);
	str("\n");
	dump_obj("ebx", regs.ebx);
	dump_obj("edx", regs.edx);
	dump_obj("ecx", regs.ecx);
	str("\n");
	dump_obj("eax", regs.eax);
	str("\n");
	dump_obj("int_no", regs.int_no);
	dump_obj("err_code", regs.err_code);
	str("\n");
	dump_obj("eip", regs.eip);
	dump_obj("cs", regs.cs);
	dump_obj("eflags", regs.eflags);
	str("\n");
	dump_obj("user esp", regs.useresp);
	dump_obj("ss", regs.ss);
}
void dump_obj(char *c, u32int i) {
	str(c);
	str(": ");
	dec(i);
	str("     ");
	hex(i);
	str("  ");
}

void irq_Page_Fault(registers_t regs) {
monitor_write("Page Fault\n");
	dump(regs);
}

void irq_Intel_Reserved2(registers_t regs) {
monitor_write("Only Intel can do this!\n");
	dump(regs);
}

void irq_Coprocessor_Error(registers_t regs) {
monitor_write("coprocessor error!\n");
	dump(regs);
}



void init_interupts() {
	register_interrupt_handler(0, irq_Divide_Error);
	register_interrupt_handler(1, irq_Debug_Exceptions);
	register_interrupt_handler(2, irq_Intel_Reserved1);
	register_interrupt_handler(3, irq_Breakpoint);
	register_interrupt_handler(4, irq_Overflow);
	register_interrupt_handler(5, irq_Bounds_Check);
	register_interrupt_handler(6, irq_Invalid_Opcode);
	register_interrupt_handler(7, irq_Coprocessor_Not_Availiable);
	register_interrupt_handler(8, irq_Double_Fault);
	register_interrupt_handler(9, irq_Coprocessor_Segment_Overrun);
	register_interrupt_handler(10, irq_Invalid_TSS);
	register_interrupt_handler(11, irq_Segment_Not_Present);
	register_interrupt_handler(12, irq_Stack_Exception);
	register_interrupt_handler(13, irq_General_Protection_Exception);
	register_interrupt_handler(14, irq_Page_Fault);
	register_interrupt_handler(15, irq_Intel_Reserved2);
	register_interrupt_handler(16, irq_Coprocessor_Error);

}
