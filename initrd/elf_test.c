//this file is used for testing elf binary execution
int main(void)
{
	int a;
	unsigned char num = 0;
	char *str = "YAY FROM HELL\n";
	char *p1 = (char*)&str;
	asm volatile("int $0x80" : "=a" (a) : "0" (num), "b" ((int)p1));
	return a;
}

