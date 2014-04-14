//this file is used for testing elf binary execution
int main(void)
{
	long *video_memory = ( long * ) 0xB8002;
	*video_memory = 16;
	return 0;
}

