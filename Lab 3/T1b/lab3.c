#include <stdio.h>	
#include <stdlib.h>

void PrintHex(unsigned char *buffer, size_t length)
{
	for(int i = 0; i < length; i++)
		printf("%02X ", buffer[i]);
	printf("\n");
}

typedef struct virus {
    unsigned short SigSize;
    unsigned char* sig;
    char virusName[16];
} virus;

typedef struct link link;
 
struct link {
    link *nextVirus;
    virus *vir;
};

void readVirus(virus* vir, FILE* input) {
	char size[2];
	fread(size, 2, 1, input);
	vir->SigSize = size[0] | size[1] << 8;
	vir->sig = malloc(vir->SigSize);
	size_t sig = fread(vir->sig, 1, vir->SigSize, input);
	if (sig == 0){
		free(vir->sig);
		free(vir);
		return;
	}
	fread(vir->virusName, 1, 16, input);
}

void printVirus(virus* vir, FILE* output)
{
	if (vir == NULL || vir->SigSize == 0 || vir->virusName == NULL)
		return;
	fprintf(output, "Virus Name: %s\n", vir->virusName);
	fprintf(output, "Virus Size: %d\n", vir->SigSize);
	fprintf(output, "Virus Signature:\n");
	PrintHex(vir->sig, vir->SigSize);
	printf("\n");
}

link* list_print(link *virus_list, FILE* output) {
	if (virus_list == NULL)
		return NULL;
	printVirus(virus_list->vir, output);
	if (sizeof(virus_list->nextVirus) != 0)
		list_print(virus_list->nextVirus, output);
	return virus_list;
}

link* list_append(link* virus_list, link* to_add) {
	if (to_add == NULL || virus_list == NULL)
		return NULL;
	// If we need to add to head of the list:
	to_add->nextVirus = virus_list;
	return to_add;
	
	// If we need to add to the tail of the list:
	if (virus_list->nextVirus == NULL) {
		virus_list->nextVirus = to_add;
		return virus_list;
	}
	list_append(virus_list->nextVirus, to_add);
	return virus_list;
}

void list_free(link *virus_list) {
	if (virus_list == NULL)
		return;
	else {
		list_free(virus_list->nextVirus);
		free(virus_list);
	}
}

typedef struct fun_desc {
  char *name;
  link* (*fun)(link*, FILE*);
} fun_desc;

link* quit(link *virus_list, FILE* output)
{
	if (virus_list != NULL)
		list_free(virus_list);
	exit(0);
	return virus_list;
}

link *loadViruses(link *virus_list, FILE* output)
{	
	char fileName[100];
	printf("Please enter a file name\n");
	scanf("%[^\n]%*c", fileName);
	printf("File name is %s\n", fileName);
	if (sizeof(fileName) == 0)
		return NULL;
	FILE *fp = fopen(fileName, "r");
	
	link *vlist = malloc(sizeof(link));
	vlist->vir = NULL;
	if (fp == NULL) {
		fprintf(stderr, "Can't open file\n");
		exit(1);
	}
	while (!feof(fp)) {
		virus *vir = malloc(sizeof(virus));
		readVirus(vir, fp);
		if (vir == NULL)
			continue;
		if (vlist->vir == NULL) {
			vlist->vir = vir;
			continue;
		}
		link *toInsert = malloc(sizeof(link));
		toInsert->vir = vir;
		vlist = list_append(vlist, toInsert);
	}
	fclose(fp);
	return vlist;
}

int main(int argc, char **argv)
{
	link *virus_list = NULL;
	struct fun_desc menu[] = { 
	{"Load signatures", loadViruses}, 
	{"Print signatures", list_print},
	{"Quit", quit},
	{NULL, NULL}};
	FILE *output = stdout;
	do {
		printf("Please choose a function:\n");
		int len = sizeof(menu)/sizeof(struct fun_desc) - 1;
		for(int i = 0; i < len; i++)
			printf("%d) %s\n", i, menu[i].name);
		int input = fgetc(stdin) - '0';
		fgetc(stdin);
		printf("Option: %d\n", input);
		if (input >= 0 && input < len)
			printf("Within bounds\n");
		else
		{
			printf("Not within bounds\n");
			list_free(virus_list);
			exit(0);
		}
		virus_list = menu[input].fun(virus_list, output);
	}
	while(1);
	return 0;
}
