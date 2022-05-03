#include "util.h"

#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_LSEEK 19
#define SEEK_SET 0
#define SEEK_CUR 1
#define O_WRONLY 1
#define O_RDRW 2
#define STDOUT 1

int errno = 0;

extern int system_call();

int main (int argc , char* argv[], char* envp[])
{
	if (argc < 3 || argc > 3) return 0x55;
	char* name;
	if (strlen(argv[2]) > 6) 
		name = strcat(argv[2], ".\n");
	else 
		name = strcat(argv[2], ".\n\0");
	int loc = 0x291;
	int fd = system_call(SYS_OPEN, argv[1], O_RDRW, 0x0777);
	system_call(SYS_LSEEK, fd, loc, SEEK_CUR);
	errno = system_call(SYS_WRITE, fd, name, strlen(name));
	int i = strlen(name);
	for(; i < strlen("Shira.\n\0"); i++)
		system_call(SYS_WRITE, fd, "\0", 1);
	if (errno <= 0) return 0x55;
	errno = system_call(SYS_CLOSE, fd);
	if (errno < 0) return 0x55;
	return 0;
}
