#include "util.h"

#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_LSEEK 19
#define SEEK_SET 0
#define SYS_GETDENTS 141

#define O_CW 101
#define O_RDONLY 0

#define STDIN 0
#define STDOUT 1
#define STDERR 2

extern int system_call();

void printRet(int debug, int sysRet, int sys)
{
	if (debug != 0)
	{
		system_call(SYS_WRITE, STDERR, "SysId is ", strlen("SysId is "));
		system_call(SYS_WRITE, STDERR, itoa(sys), (itoa(sys)));
		system_call(SYS_WRITE, STDERR, "\n", 1);
		system_call(SYS_WRITE, STDERR, "Sys Return is ", strlen("Sys return is "));
		system_call(SYS_WRITE, STDERR, itoa(sysRet), (itoa(sysRet)));
		system_call(SYS_WRITE, STDERR, "\n\n", 2);
	}
}

typedef struct ent {
	int inode;
	int offset;
	short len;
	char buf[1];
} ent;

void flame2(int debug)
{
	char buffer[8192];
	ent *entp;
	int count;
	int fd = system_call(SYS_OPEN, ".", O_RDONLY, 0777);
	count = system_call(SYS_GETDENTS, fd, buffer, 8192);
	printRet(debug, fd, SYS_OPEN);
	printRet(debug, count, SYS_GETDENTS);
	int pos;
	for (pos = 0; pos < count;)
	{
		entp = (ent*)(buffer + pos);
		pos += entp->len;
		if (debug == 1)
		{
			system_call(SYS_WRITE, STDERR, "DirEnt name: ", strlen("DirENt name: "));
			system_call(SYS_WRITE, STDERR, entp->buf, strlen(entp->buf));
			system_call(SYS_WRITE, STDERR, "\n", 1);
			system_call(SYS_WRITE, STDERR, "DirEnt len: ", strlen("DirENt nam: "));
			system_call(SYS_WRITE, STDERR, itoa(entp->len), strlen(itoa(entp->len)));
			system_call(SYS_WRITE, STDERR, "\n\n", 2);
		}	
		if (strcmp(entp->buf, "..") == 0 || strcmp(entp->buf, ".") == 0)
			continue;
		int sysRet = system_call(SYS_WRITE, STDOUT, entp->buf, strlen(entp->buf));
		system_call(SYS_WRITE, STDOUT, "\n", 1);
		printRet(debug, sysRet, SYS_WRITE);
	}
}



int main (int argc , char* argv[])
{
	int debug = 0;
	int i;
	for(i = 1; i < argc; i++)
		if (strcmp(argv[i], "-D") == 0)
			debug = 1;
	flame2(debug);
	return 0;
}
