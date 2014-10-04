#include "elf.h"
#include "fs.h"
#include "paging.h"

void load() {
	FILE *bin;
	bin = ( FILE* ) f_open ( "binary.bin", fs_root, "r" );
	serialf ( "Binary openend\n" );
	char *buf = ( char* ) kmalloc ( sizeof ( char ) * 1024 );
	f_read ( bin, 0, 1024, buf );
	//serialf("\n\n%s\n", buf);
	
	virtual_map_pages ( 0x500000, 0x1000, 1, 0 );
	kmalloc_ap(1024, 0x500000);
	serialf("yay memory allocation worked :)\n");
	memset(0x500000, 0, 1024);
	memcpy(0x500000, buf, 1024);
	serialf("memory coppied :) :) :)\n");
	
	asm volatile("call 0x500000");
	
	printf("FINISHED\n");
}
