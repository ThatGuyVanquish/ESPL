#include <stdio.h>
#include <stdlib.h>

void PrintHex(char *buffer, size_t length)
{
	for(int i = 0; i < length; i++)
		printf("%x ", buffer[i]);
}

typedef struct virus {
    unsigned short SigSize;
    unsigned char* sig;
    char virusName[16];
} virus;

void readVirus(virus* vir, FILE* input) {
	//fread(vir->virusName, 16, 1, input);
	//fread(vir->sig, vir->SigSize, 1, input);
	char* size = malloc(2);
	printf("previous size is %x\n", size);
	fread(size, 2, 1, input);
	printf("hex size is %x\n", size);
	printf("%d %s\n", vir->SigSize, size);


}

void printVirus(virus* vir, FILE* output)
{
	fprintf(output, "Virus Name: %s\n", vir->virusName);
	fprintf(output, "Virus Size: %d\n", vir->SigSize);
	fprintf(output, "Virus Signature:\n");
	PrintHex(vir->sig, sizeof(vir->sig));
}

int main(int argc, char **argv)
{
	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL) printf("BLYAT");
	virus *vir = malloc(sizeof(virus));
	readVirus(vir, fp);
	printVirus(vir, stdout);
	return 0;
}
