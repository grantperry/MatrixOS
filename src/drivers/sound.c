 #include "sound.h"
 //Play sound using built in speaker
void play_sound(u32int nFrequence) {
 	u32int Div;
 	u8int tmp;
 
        //Set the PIT to the desired frequency
 	Div = 1193180 / nFrequence;
 	outb(0x43, 0xb6);
 	outb(0x42, (u8int) (Div) );
 	outb(0x42, (u8int) (Div >> 8));
 
        //And play the sound using the PC speaker
 	tmp = inb(0x61);
  	if (tmp != (tmp | 3)) {
 		outb(0x61, tmp | 3);
 	}
 }
 
 //make it shutup
void nosound() {
 	u8int tmp = (inb(0x61) & 0xFC);
 
 	outb(0x61, tmp);
 }
 
 //Make a beep
 void beep() {
 	 play_sound(1000);
 	 sleep(1);
 	 nosound();
          //set_PIT_2(old_frequency);
 }
