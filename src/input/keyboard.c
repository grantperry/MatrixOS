#include "keyboard.h"
#include "../isr.h"
#include "../common.h"

void keyboard_handler(registers_t *regs);

void init_keyboard() {
	register_interrupt_handler(IRQ1, &keyboard_handler);
}

void keyboard_handler(registers_t *regs) {
	print("key pressed:)\n");
}
