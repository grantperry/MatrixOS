#include <bin_flat.h>

void load_flat ( char* name, u32int loc) {
	FILE *bin;
	bin = ( FILE* ) f_open ( name, fs_root, "r" );
	serialf ( "Binary openend\n" );
	char *buf = ( char* ) kmalloc ( sizeof ( char ) * 1024 );
	f_read ( bin, 0, 1024, buf );
	//serialf("\n\n%s\n", buf);

	virtual_map_pages ( loc, 0x1000, 1, 0 );
	kmalloc_ap ( 1024, loc );
	serialf ( "yay memory allocation worked :)\n" );
	memset ( loc, 0, 1024 );
	memcpy ( loc, buf, 1024 );
	serialf ( "memory coppied :) :) :)\n" );

	asm volatile ( "call %0"::"r"(loc) );

	printf ( "FINISHED\n" );
}
