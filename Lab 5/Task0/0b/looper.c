#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void sigHandler(int sig)
{
	printf("Received signal: %s\n", strsignal(sig));
	signal(sig, SIG_DFL);
	raise(sig);
	sig == SIGTSTP ? signal(SIGCONT, sigHandler) : signal(SIGTSTP, sigHandler); // can't be sigint as raise would've stopped this process
}

int main(int argc, char **argv){ 

	printf("Starting the program\n");
	signal(SIGTSTP, sigHandler);
	signal(SIGCONT, sigHandler);
	signal(SIGINT, sigHandler);
	while(1) {
		sleep(2);
	}

	return 0;
}