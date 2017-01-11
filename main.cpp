#include "assembler.h"

int main()
{
	assembler assembler("/home/lamuguo/code/tmp/yangbin/myassembler/testcpu.asm", 0);
	assembler.Parser("/tmp/testcpu.out");

	return 0;
}
