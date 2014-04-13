// timer.c -- Initialises the PIT, and handles clock updates.
//			Written for JamesM's kernel development tutorials.

#include "timer.h"
#include "isr.h"
#include "monitor.h"
#include "task.h"

u32int tick = 0, globalFreq, secondTick = 0;
u32int pass = 0, systemTimePassed = 0;
unsigned long long int secondsPassed = 0;

extern volatile task_t *current_task;

extern u8int TASKING_ON;


u16int taskTickCount = 0;
u32int taskTickTime = 3;

void sleep ( u32int seconds ) {
	u8int sleeping = 1;
	u32int timeLeft = getSeconds() + seconds;

	while ( sleeping ) {
		if ( getSeconds() == timeLeft ) {
			sleeping = 0;
		}
	}
}

void system_sleep ( u32int t ) {
	syscall_sleep ( t );
}

void system_mSleep ( u32int t ) {
	syscall_mSleep ( t );
}

//~ void mSleep(long long int milliseconds)
void mSleep ( u32int milliseconds ) {

	if ( milliseconds > 0 ) {
		unsigned long eticks;

		eticks = systemTimePassed + milliseconds;

		while ( systemTimePassed < eticks );
	}

}

u32int getTick() {
	return tick;
}
u32int getSeconds() {
	return secondsPassed;
}


unsigned long long int getSystemUpTime() {
	return systemTimePassed;
}

void timer_callback() {
	tick = tick + 1;
	secondTick++;

	if ( secondTick == globalFreq ) {
		secondsPassed++;
		secondTick = 0;
	}

	if ( TASKING_ON ) {
		//Reduce the running tasks time in queue
		if ( current_task->time_to_run > 0 && current_task->ready_to_run == TRUE ) {
			current_task->time_to_run--;
			current_task->time_running++;

		} else {
			switch_task();
		}
	}

	return;
}


extern u8int SLEEP_ENABLED;

void init_timer ( u32int frequency ) {
	asm volatile ( "sti" );
	globalFreq = frequency;
	// Firstly, register our timer callback.
	register_interrupt_handler ( IRQ0, &timer_callback );

	// The value we send to the PIT is the value to divide it's input clock
	// (1193180 Hz) by, to get our required frequency. Important to note is
	// that the divisor must be small enough to fit into 16-bits.
	u32int divisor = 1193180 / frequency;

	// Send the command byte.
	outb ( 0x43, 0x36 );

	// Divisor has to be sent byte-wise, so split here into upper/lower bytes.
	u8int l = ( u8int ) ( divisor & 0xFF );
	u8int h = ( u8int ) ( ( divisor>>8 ) & 0xFF );

	// Send the frequency divisor.
	outb ( 0x40, l );
	outb ( 0x40, h );

	SLEEP_ENABLED = 1;
}

#define BCD2BIN(bcd) ((((bcd)&15) + ((bcd)>>4)*10))
#define MINUTE 60
#define HOUR (60*MINUTE)
#define DAY (24*HOUR)
#define YEAR (365*DAY)

//Gets CMOS actual time
datetime_t getDatetime() {
	datetime_t now;

	cpu_cli();
	now.sec = BCD2BIN ( readCMOS ( 0x0 ) );
	now.min = BCD2BIN ( readCMOS ( 0x2 ) );
	now.hour = BCD2BIN ( readCMOS ( 0x4 ) );
	now.day = BCD2BIN ( readCMOS ( 0x7 ) );
	now.month = BCD2BIN ( readCMOS ( 0x8 ) );
	now.year = BCD2BIN ( readCMOS ( 0x9 ) );
	cpu_sti();

	return now;
}

datetime_t getLocaltime ( char *place ) {
	datetime_t now = getDatetime();

	if ( checkstr ( place, "sydney" ) ) {
		now.hour = now.hour + 10;

		if ( now.hour > 24 ) {
			now.day = now.day + 1;
			now.hour = now.hour - 24;
		}

		return now;
	}
}

/* Seconds since 01/01/1970 */
int mktime ( datetime_t time ) {
	long res;
	int year;

	int month[12] = {
		0,
		DAY* ( 31 ),
		DAY* ( 31+29 ),
		DAY* ( 31+29+31 ),
		DAY* ( 31+29+31+30 ),
		DAY* ( 31+29+31+30+31 ),
		DAY* ( 31+29+31+30+31+30 ),
		DAY* ( 31+29+31+30+31+30+31 ),
		DAY* ( 31+29+31+30+31+30+31+31 ),
		DAY* ( 31+29+31+30+31+30+31+31+30 ),
		DAY* ( 31+29+31+30+31+30+31+31+30+31 ),
		DAY* ( 31+29+31+30+31+30+31+31+30+31+30 )
	};

	year = time.year - 70;
	/* magic offsets (y+1) needed to get leapyears right.*/
	res = YEAR*year + DAY* ( ( year+1 ) /4 );
	res += month[time.month];

	/* and (y+2) here. If it wasn't a leap-year, we have to adjust */
	if ( time.month>1 && ( ( year+2 ) %4 ) ) {
		res -= DAY;
	}

	res += DAY* ( time.day-1 );
	res += HOUR*time.hour;
	res += MINUTE*time.min;
	res += time.sec;
	return res;
}
