// timer.c -- Initialises the PIT, and handles clock updates.
//			Written for JamesM's kernel development tutorials.

#include "timer.h"
#include "isr.h"
#include "monitor.h"

extern u8int __TASKING_ENABLED;

u32int tick = 0, globalFreq, secondTick = 0;
u32int pass = 0, systemTimePassed = 0;
unsigned long long int secondsPassed = 0;

u16int taskTickCount = 0;
u32int taskTickTime = 3;

void sleep(u32int seconds) {
	u8int sleeping = 1;
	u32int timeLeft = getSeconds() + seconds;
	while(sleeping) {
		if(getSeconds() == timeLeft) {
			sleeping = 0;
		}
	}
}

void system_sleep(u32int t)
{
	syscall_sleep(t);
}

void system_mSleep(u32int t) {
	syscall_mSleep(t);
}

//~ void mSleep(long long int milliseconds)
void mSleep(u32int milliseconds)
{

  if(milliseconds > 0)
  {
    unsigned long eticks;

    eticks = systemTimePassed + milliseconds;
    while(systemTimePassed < eticks);
  }
  
}

u32int getTick() {
	return tick;
}
u32int getSeconds() {
	return secondsPassed;
}


unsigned long long int getSystemUpTime()
{
  return systemTimePassed;
}

void timer_callback()
{
	tick = tick + 1;
	secondTick++;
	if (secondTick == globalFreq) {
		secondsPassed++;
		secondTick = 0;
	}
	if(__TASKING_ENABLED) {
		if(taskTickCount == taskTickTime) {
			switch_task();
		}
	}
	return;
}


extern u8int SLEEP_ENABLED;

void init_timer(u32int frequency)
{
	asm volatile("sti");
  globalFreq = frequency;
  // Firstly, register our timer callback.
  register_interrupt_handler(IRQ0, &timer_callback);

  // The value we send to the PIT is the value to divide it's input clock
  // (1193180 Hz) by, to get our required frequency. Important to note is
  // that the divisor must be small enough to fit into 16-bits.
  u32int divisor = 1193180 / frequency;

  // Send the command byte.
  outb(0x43, 0x36);

  // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
  u8int l = (u8int)(divisor & 0xFF);
  u8int h = (u8int)( (divisor>>8) & 0xFF );

  // Send the frequency divisor.
  outb(0x40, l);
  outb(0x40, h);
  
  SLEEP_ENABLED = 1;
}
