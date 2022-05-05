#include "util.h"

#define SYS_WRITE 4
#define SYS_READ 3
#define STDOUT 1
#define STDIN 0


extern int system_call();

int count(int debug)
{
	char currentByte[1];
	while(1) {
		int counter = 0;
		int newWord = 1;
		int sysCallRet;
		while (currentByte[0] != '\n')
		{
			sysCallRet = system_call(SYS_READ, STDIN, &currentByte, 1);
			if (debug == 1) 
			{
				system_call(SYS_WRITE, STDOUT, "SysCall ID is 4\n", strlen("SysCall ID is 4\n"));
				if (currentByte[0] == '\n') 
					system_call(SYS_WRITE, STDOUT, "Current char is newline", strlen("Current char is newline"));
				else if (currentByte[0] == ' ') 
					system_call(SYS_WRITE, STDOUT, "Current char is space", strlen("Current char is space"));
				else
				{
					system_call(SYS_WRITE, STDOUT, "Current char is ", strlen("Current char is "));
					system_call(SYS_WRITE, STDOUT, &currentByte, 1);
				}
				system_call(SYS_WRITE, STDOUT, "\n", 1);
				system_call(SYS_WRITE, STDOUT, "SysCall Return is ", strlen("SysCall Return is "));
				system_call(SYS_WRITE, STDOUT, itoa(sysCallRet), strlen(itoa(sysCallRet)));
				system_call(SYS_WRITE, STDOUT, "\n\n", 2);
			}
			if (currentByte[0] - ' ' == 0) newWord = 1;
			else if (currentByte[0] != '\n' && newWord == 1)
			{
				counter++;
				newWord = 0;
			}
		}
		if (debug == 1) system_call(SYS_WRITE, STDOUT, "\n\n\nSysCall ID is 4\n", strlen("\n\n\nSysCall ID is 4\n"));
		system_call(SYS_WRITE, STDOUT, "Amount of words: ", strlen("Amount of words: "));
		sysCallRet = system_call(SYS_WRITE, STDOUT, itoa(counter), strlen(itoa(counter)));
		if (debug == 1) 
		{
			system_call(SYS_WRITE, STDOUT, "\n", 1);
			system_call(SYS_WRITE, STDOUT, "SysCall Return is ", strlen("SysCall Return is "));
			system_call(SYS_WRITE, STDOUT, itoa(sysCallRet), strlen(itoa(sysCallRet)));
		}
		system_call(SYS_WRITE, STDOUT, "\n", 1);
		currentByte[0] = '.';
	}
}

int main (int argc , char* argv[])
{
	if (argc > 1 && strcmp(argv[1], "-D") == 0)
		count(1);
	else
		count(0);
	return 0;
}
