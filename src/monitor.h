// MatrixOS - monitor.h
// Defines the interface for monitor.h

#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

// Write a single character out to the screen.
void monitor_put ( char c );

// Clear the screen to all black.
void monitor_clear();

// Output a null-terminated ASCII string to the monitor.
void monitor_write ( char *c );

void monitor_set_colour ( u8int background, u8int foreground );

void monitor_set_fore_colour ( u8int foreground );

void monitor_set_back_colour ( u8int background );

void monitor_set_cursor_pos ( u8int cursorX, u8int cursorY );

void monitor_command ( char com[], char arg[] );

void monitor_backspace();

u8int monitor_getX();

u8int monitor_getY();

u8int monitor_get_back_colour();

u8int monitor_get_fore_colour();

// Output a hex value to the monitor.
void monitor_write_hex ( u32int n );

// Output a decimal value to the monitor.
void monitor_write_dec ( u32int n );

#endif // MONITOR_H
