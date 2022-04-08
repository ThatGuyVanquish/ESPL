#include <stdio.h>
#include <stdlib.h>

void PrintHex(char* buffer, size_t length)
{
	for(int i = 0; i < length; i++)
		printf("%x ", buffer[i]);
}

int main(int argc, char** argv)
{
	FILE *fp = fopen(argv[1], "r");
	char *c = malloc(sizeof(fp));
	size_t size;
	while ((size = fread(c, 1, 1, fp)) > 0)
		PrintHex(c, size);	
	printf("\n");
	return 0;
}
