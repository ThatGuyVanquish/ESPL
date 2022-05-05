#include <dirent.h>
#include "util.h"

enum
  {
    DT_UNKNOWN = 0,
# define DT_UNKNOWN	DT_UNKNOWN
    DT_FIFO = 1,
# define DT_FIFO	DT_FIFO
    DT_CHR = 2,
# define DT_CHR		DT_CHR
    DT_DIR = 4,
# define DT_DIR		DT_DIR
    DT_BLK = 6,
# define DT_BLK		DT_BLK
    DT_REG = 8,
# define DT_REG		DT_REG
    DT_LNK = 10,
# define DT_LNK		DT_LNK
    DT_SOCK = 12,
# define DT_SOCK	DT_SOCK
    DT_WHT = 14
# define DT_WHT		DT_WHT
  };

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

int printNames(int debug, char* prefix)
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
		if (strlen(prefix) != 0 && strncmp(dir->buf, prefix, strlen(prefix)) != 0) continue;
		int sysRet = system_call(SYS_WRITE, STDOUT, dir->buf, strlen(dir->buf));
		system_call(SYS_WRITE, STDOUT, "\n", 1); 
		char entType = *(buffer + pos + dir->len - 1);
		char* strType =
		(entType == DT_REG) ?  "regular" :
                                    (entType == DT_DIR) ?  "directory" :
                                    (entType == DT_FIFO) ? "FIFO" :
                                    (entType == DT_SOCK) ? "socket" :
                                    (entType == DT_LNK) ?  "symlink" :
                                    (entType == DT_BLK) ?  "block dev" :
                                    (entType == DT_CHR) ?  "char dev" : "???";
		system_call(SYS_WRITE, STDOUT, "Entry type: ", strlen("Entry type: ")); 
		system_call(SYS_WRITE, STDOUT, strType, strlen(strType));
		system_call(SYS_WRITE, STDOUT, "\n\n", 2); 
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
	char* prefix = "\0";
	for(;i < argc; i++)
	{
		if (strcmp(argv[i], "-D") == 0) debug = 1;
		if (strncmp(argv[i], "-p", 2) == 0) prefix = argv[i] + 2;
	}
	return printNames(debug, prefix);
}
