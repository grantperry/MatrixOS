// MatrixOS - timer.h
// Defines the interface for all PIT-related functions.
// Written for JamesM's kernel development tutorials.

#ifndef TIMER_H
#define TIMER_H

#include <common.h>

void init_timer ( u32int frequency );

void sleep ( u32int t );

void system_sleep ( u32int t );

u32int getTick();
u32int getSeconds();

void system_mSleep ( u32int t );

void mSleep ( u32int milliseconds );

u8int SLEEP_ENABLED;

typedef struct {
	unsigned char sec;
	unsigned char min;
	unsigned char hour;
	unsigned char day;
	unsigned char month;
	unsigned char year;
} datetime_t;

datetime_t getDatetime();

datetime_t getLocaltime ( char *place );

int mktime ( datetime_t time );

#endif
