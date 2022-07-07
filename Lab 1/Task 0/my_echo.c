#include <stdio.h>

// task o lab 1
// echo the command
int main(int argc, char** argv)
{
	//start with 1 becuse in argv[0] 
	//is name of file
	for(int i = 1; i < argc; i++)
	{	

		printf("%s", argv[i]);
		if (i != argc - 1)
			printf(" ");
	}
	printf("\r\n");
	return 0;
}
