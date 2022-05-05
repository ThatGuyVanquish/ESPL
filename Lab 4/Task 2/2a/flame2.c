#include "util.h"
#include <dirent.h>

#define sys_getdents 141

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

typedef struct ent {
	int inode;
	int offset;
	short len;
	char buf[1];
	} ent;

int printNames(int debug)
{
	char buffer[8192];
	ent *dir;
	int dirfd = system_call(SYS_OPEN, ".", O_RDONLY, 00777);
	if (debug)
	{
		system_call(SYS_WRITE, STDOUT, "SysId is 4\n", strlen("SysId is 4\n"));
		system_call(SYS_WRITE, STDOUT, "SysCall return is ", strlen("SysCall return is "));
		system_call(SYS_WRITE, STDOUT, itoa(dirfd), strlen(itoa(dirfd)));
		system_call(SYS_WRITE, STDOUT, "\n\n", 2);
	}
	if (dirfd < 0) 
	{
		system_call(SYS_WRITE, STDOUT, "Error opening folder\n", strlen("Error opening folder\n"));
		return 0x55;
	}
	int nread = system_call(sys_getdents, dirfd, buffer, 8192);
	if (debug)
	{
		system_call(SYS_WRITE, STDOUT, "SysId is 141\n", strlen("SysId is 141\n"));
		system_call(SYS_WRITE, STDOUT, "SysCall return is ", strlen("SysCall return is "));
		system_call(SYS_WRITE, STDOUT, itoa(nread), strlen(itoa(nread)));
		system_call(SYS_WRITE, STDOUT, "\n\n", 2);
	}
	int pos;
	for(pos = 0; pos < nread;)
	{
		dir = buffer + pos;
		pos += dir->len;
		if (debug)
		{
			system_call(SYS_WRITE, STDOUT, "DirEnt name is ", strlen("DirEnt name is "));
			system_call(SYS_WRITE, STDOUT, dir->buf, strlen(dir->buf));
			system_call(SYS_WRITE, STDOUT, "\n", 1);
			system_call(SYS_WRITE, STDOUT, "DirEnt length is ", strlen("DirEnt length is "));
			system_call(SYS_WRITE, STDOUT, itoa(dir->len), strlen(itoa(dir->len)));
			system_call(SYS_WRITE, STDOUT, "\n\n", 2);
		}
		if (strcmp(dir->buf, "..") == 0 || strcmp(dir->buf, ".") == 0)
			continue;
		int sysRet = system_call(SYS_WRITE, STDOUT, dir->buf, strlen(dir->buf));
		system_call(SYS_WRITE, STDOUT, "\n", 1); 
		if (debug)
		{
			system_call(SYS_WRITE, STDOUT, "SysId is 4\n", strlen("SysId is 4\n"));
			system_call(SYS_WRITE, STDOUT, "SysCall return is ", strlen("SysCall return is "));
			system_call(SYS_WRITE, STDOUT, itoa(sysRet), strlen(itoa(sysRet)));
			system_call(SYS_WRITE, STDOUT, "\n\n", 2);
		}
	}
	return 0;
}

int main(int argc, char** argv)
{
	int i = 0;
	int debug = 0;
	for(;i < argc; i++)
		if (strcmp(argv[i], "-D") == 0) debug = 1;
	
	return printNames(debug);
}
