#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
char censor(char c) {
  if(c == '!')
    return '*';
  else
    return c;
}
 /* LAB 2 TASK 3
 The important part of this lab is to learn how to use 
 pointers to functions
 we created a menu of fun_desc structs
 */
char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
  for(int i = 0; i < array_length; i++) {
  	mapped_array[i] = (*f)(array[i]);	
  }	
  return mapped_array;
}
 
char encrypt(char c) {
	return ((c >= 'A') && (c <= 'z')) ? c + 2 : c;
}

          
char decrypt(char c) {
	return ((c >= 'A') && (c <= 'z')) ? c - 2 : c;

}            
char dprt(char c) { 
	printf("%d\n", c);
	return c;
}
           
char cprt(char c) {
	c >= 'A' && c <= 'z' ? printf("%c\n", c) : printf("*\n");
	return c;
}
char my_get(char c) {
	return fgetc(stdin);
}

char quit(char c) {
	c == 'q' ? exit(0) : c;
	return c;
}

struct fun_desc {
  char *name;
  char (*fun)(char);
};

int main(int argc, char **argv){
	char *carray = malloc(5);
	carray[0] = '\0';
	struct fun_desc menu[] = {
	{"Censor", censor},
	{"Encrypt", encrypt},
	{"Decrypt", decrypt},
	{"Print dec", dprt},
	{"Print string", cprt},
	{"Get string", my_get},
	{"Quit", quit},
	{NULL, NULL}
	};
	do {
		printf("Please choose a function:\n");
		int len = sizeof(menu)/sizeof(struct fun_desc) - 1;
		for(int i = 0; i < len; i++)
			printf("%d) %s\n", i, menu[i].name);
		int input = fgetc(stdin) - '0'; //deduct the ascii value of 0 from fgetc
		fgetc(stdin); // get rid of newline
		printf("Option: %d\n", input);
		if (input >= 0 && input < len)
			printf("Within bounds\n");
		else
		{
			printf("Not within bounds\n");
			free(carray);
			exit(0);
		}
		char* arr = map(carray, 5, menu[input].fun);
		free(carray);
		carray = arr;
		printf("Done.\n\n");
	}while(1);
}
