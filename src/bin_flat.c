#include <bin_flat.h>

void load_flat ( char* name, u32int loc) {
	FILE *bin;
	bin = ( FILE* ) f_open ( name, fs_root, "r" );
	serialf ( "[FLATBIN] Opening \"%s\". ", name );
	char *buf = ( char* ) kmalloc ( sizeof ( char ) * 1024 );
	f_read ( bin, 0, 1024, buf );

	virtual_map_pages ( loc, 0x1000, 1, 0 );
	kmalloc_ap ( 1024, loc );
	memset ( loc, 0, 1024 );
	memcpy ( loc, buf, 1024 );

	asm volatile ( "call *%0"::"r"(loc) );

	serialf( "Executed.\n" );
}
