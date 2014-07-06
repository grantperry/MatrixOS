#ifndef COMMON_H
#define COMMON_H

/*
// Some nice typedefs, to standardise sizes across platforms.
// These typedefs are written for 32-bit X86.
*/
typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

void outb ( u16int port, u8int value );

u8int inb ( u16int port );

u16int inw ( u16int port );

void sti();

// define our structure for our registers.
typedef struct __attribute__ ( ( packed ) ) {
	unsigned short di, si, bp, sp, bx, dx, cx, ax;
	unsigned short gs, fs, es, ds, eflags;
}
regs16_t;

u8int checkstr ( char* str1, char* str2 );

extern void int32 ( u8int intnum, regs16_t *regs );

void numToAsciChar ( char *asciInChar, int integerLength );
void intToChar ( int integer, char *numbersInChar );

#define TRUE    1  //define a few variable
#define ON      1
#define FALSE   0
#define OFF     0

#define PANIC(msg) panic(msg, __FILE__, __LINE__);

#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

extern void panic ( const char *message, const char *file, u32int line );

extern void panic_assert ( const char *file, u32int line, const char *desc );

u32int return_memory();

#endif // COMMON_H
