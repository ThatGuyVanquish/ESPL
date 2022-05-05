#include "util.h"

#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_LSEEK 19
#define SEEK_SET 0

#define O_CW 101
#define O_RDONLY 0

#define STDIN 0
#define STDOUT 1

extern int system_call();

void count(int debug, int INPUT, int OUTPUT)
{
	char currentByte[1];
	while(1) {
		int stop = 0;
		int counter = 0;
		int newWord = 1;
		int sysCallRet;
		while (currentByte[0] != '\n')
		{
			sysCallRet = system_call(SYS_READ, INPUT, &currentByte, 1);
			if (sysCallRet == 0) 
			{
				stop = 1;
				break;
			}
			if (debug == 1) 
			{
				system_call(SYS_WRITE, OUTPUT, "SysCall ID is 4\n", strlen("SysCall ID is 4\n"));
				if (currentByte[0] == '\n') 
					system_call(SYS_WRITE, OUTPUT, "Current char is newline", strlen("Current char is newline"));
				else if (currentByte[0] == ' ') 
					system_call(SYS_WRITE, OUTPUT, "Current char is space", strlen("Current char is space"));
				else
				{
					system_call(SYS_WRITE, OUTPUT, "Current char is ", strlen("Current char is "));
					system_call(SYS_WRITE, OUTPUT, &currentByte, 1);
				}
				system_call(SYS_WRITE, OUTPUT, "\n", 1);
				system_call(SYS_WRITE, OUTPUT, "SysCall Return is ", strlen("SysCall Return is "));
				system_call(SYS_WRITE, OUTPUT, itoa(sysCallRet), strlen(itoa(sysCallRet)));
				system_call(SYS_WRITE, OUTPUT, "\n\n", 2);
			}
			if (currentByte[0] - ' ' == 0) newWord = 1;
			else if (currentByte[0] != '\n' && newWord == 1)
			{
				counter++;
				newWord = 0;
			}
		}
		if (stop == 1) break;
		if (debug == 1) system_call(SYS_WRITE, OUTPUT, "\nSysCall ID is 4\n", strlen("\nSysCall ID is 4\n"));
		system_call(SYS_WRITE, OUTPUT, "Amount of words: ", strlen("Amount of words: "));
		sysCallRet = system_call(SYS_WRITE, OUTPUT, itoa(counter), strlen(itoa(counter)));
		if (debug == 1) 
		{
			system_call(SYS_WRITE, OUTPUT, "\n", 1);
			system_call(SYS_WRITE, OUTPUT, "SysCall Return is ", strlen("SysCall Return is "));
			system_call(SYS_WRITE, OUTPUT, itoa(sysCallRet), strlen(itoa(sysCallRet)));
		}
		system_call(SYS_WRITE, OUTPUT, "\n********\n\n", 11);
		currentByte[0] = '.';
	}
}

int main (int argc , char* argv[])
{
	int debug = 0;
	int input = STDIN;
	int output = STDOUT;
	if (argc == 1)
		count(0, input, output);
	else
	{
		int i;
		for(i = 1; i < argc; i++)
			if (strcmp(argv[i], "-D") == 0)
				debug++;
		for(i = 1; i < argc; i++)
		{
			if (strncmp(argv[i], "-o", 2) == 0)
			{	
				output = system_call(SYS_OPEN, argv[i] + 2, O_CW, 00777);
				if (output < 0) 
				{
					system_call(SYS_WRITE, STDOUT, "Error opening output file\n", strlen("Error opening output file\n"));
					return 1;
				}
				if (debug == 1)
				{
					system_call(SYS_WRITE, STDOUT, "SysCall ID is 5\n", strlen("SysCall ID is 5\n"));
					system_call(SYS_WRITE, STDOUT, "SysCall Return is ", strlen("SysCall Return is "));
					system_call(SYS_WRITE, STDOUT, itoa(output), strlen(itoa(output)));
					system_call(SYS_WRITE, STDOUT, "\n", 1);
					system_call(SYS_WRITE, STDOUT, "Output path is ", strlen("Output path is "));
					system_call(SYS_WRITE, STDOUT, argv[i] + 2, strlen(argv[i]) - 2);
					system_call(SYS_WRITE, STDOUT, "\n\n", 2);
				}
			}
			if (strncmp(argv[i], "-i", 2) == 0)
			{
				input = system_call(SYS_OPEN, argv[i] + 2, O_RDONLY, 0x0777);
				if (input < 0) 
				{
					system_call(SYS_WRITE, STDOUT, "Error opening input file\n", strlen("Error opening input file\n"));
					return 1;
				}
				if (debug == 1)
				{
					system_call(SYS_WRITE, STDOUT, "SysCall ID is 5\n", strlen("SysCall ID is 5\n"));
					system_call(SYS_WRITE, STDOUT, "SysCall Return is ", strlen("SysCall Return is "));
					system_call(SYS_WRITE, STDOUT, itoa(input), strlen(itoa(input)));
					system_call(SYS_WRITE, STDOUT, "\n", 1);
					system_call(SYS_WRITE, STDOUT, "Input path is ", strlen("Input path is "));
					system_call(SYS_WRITE, STDOUT, argv[i] + 2, strlen(argv[i]) - 2);
					system_call(SYS_WRITE, STDOUT, "\n\n", 2);
				}
			}
		}
		if (input == 0 && debug == 1)
		{
			system_call(SYS_WRITE, STDOUT, "Input path is stdin\n\n", strlen("Input path is stdin\n\n"));
		}
		if (output == 1 && debug == 1)
		{
			system_call(SYS_WRITE, STDOUT, "Output path is stdout\n\n", strlen("Output path is stdout\n\n"));
		}
		count(debug, input, output);
		int closein = system_call(SYS_CLOSE, input);
		int closeout = system_call(SYS_CLOSE, output);
		if (debug == 1)
		{
			system_call(SYS_WRITE, STDOUT, "SysCall ID is 6\n", strlen("SysCall ID is 6\n"));
			system_call(SYS_WRITE, STDOUT, "SysCall Return is ", strlen("SysCall Return is "));
			system_call(SYS_WRITE, STDOUT, itoa(closein), strlen(itoa(closein)));
			system_call(SYS_WRITE, STDOUT, "\n", 1);
			system_call(SYS_WRITE, STDOUT, "SysCall ID is 6\n", strlen("SysCall ID is 6\n"));
			system_call(SYS_WRITE, STDOUT, "SysCall Return is ", strlen("SysCall Return is "));
			system_call(SYS_WRITE, STDOUT, itoa(closeout), strlen(itoa(closeout)));
			system_call(SYS_WRITE, STDOUT, "\n\n", 2);
		}	
	}
	return 0;
}
