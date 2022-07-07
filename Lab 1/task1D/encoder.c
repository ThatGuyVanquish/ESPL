//task 1 lab 1 - fopen fclose fprint

#include <stdio.h>
#include <string.h>

//encoder reads the characters from standard input and 
//prints it except the uppercase (capital letters) that
// it replaced with the character '.'.v
int task1a(FILE *input)
{
	char currentChar;
	//fgetc is blocking. eof is control d
	while ((currentChar = fgetc(input)) != EOF)
	{
		if (currentChar <= 90 && currentChar >= 65)
			printf(".");
		else
			printf("%c", currentChar);
	}
	return 0;
}

//debug mode:
//encoder receives text characters from standard input 
//and prints it after encoding uppercase letters into 
//'.' to the standard output.
int task1b(FILE *input, int debug)
{
	char currentChar;
	int upper = 0;
	while ((currentChar = fgetc(input)) != EOF)
	{
		if (currentChar <= 90 && currentChar >= 65)
		{
			if (debug)
				fprintf(stderr, "%i 46\n", currentChar);
			printf(".");
			upper++;
			continue;
		}
		if (debug)
		{
			if (currentChar == '\n')
			{
				fprintf(stderr, "the number of letters: %i\n", upper);
				upper = 0;
			}
			else
				fprintf(stderr, "%i %i\n", currentChar, currentChar);
		}
		printf("%c", currentChar);
	}
	return 0;
}

int hexToInt(char sym, char hex)
{
	int mul = 1;
	if (sym == '-')
		mul = -1;
	// this is A-F
	if (hex >= 65 && hex <= 70)
		return mul * (hex - 55);
	//this hs number
	if (hex >= 48 && hex <= 57)
		return mul * (hex - 48);
	return 104;
}

//The encryption key is of the following structure: +e{key}. 
//The argument {key} stands for a digit in hexadecimal whose
// value will be the number of characters to add to the end of
// the input such that it repeats the first character. 

//Task 1d: Supporting input from a file
int task1c(FILE *input, int debug, int hex)
{
	char currentChar;
	int num = hex;
	int upper = 0;
	int first = 1;
	char firstCh;
	while ((currentChar = fgetc(input)) != EOF)
	{
		if (hex > 0 && first)
		{
			firstCh = currentChar;
			first = 0;
		}

		if (firstCh == '\n')
		{
			first = 1;
			continue;
		}

		if (num < 0)
		{
			num++;
			continue;
		}

		if (currentChar <= 90 && currentChar >= 65)
		{
			if (debug)
				fprintf(stderr, "%i 46\n", currentChar);
			printf(".");
			upper++;
			continue;
		}

		if (currentChar == '\n')
		{
			while (num > 0)
			{
				if (firstCh <= 90 && firstCh >=65)
				{
					if (debug)
						fprintf(stderr, "%i 46\n", firstCh);
					printf(".");
					upper++;
				}
				else
				{
					if (debug)
						fprintf(stderr, "%i %i\n", firstCh, firstCh);
					printf("%c", firstCh);
				}
				num--;
			}
			if (debug)
				fprintf(stderr, "the number of letters is: %i\n", upper);
			upper = 0;
			num = hex;
			first = 1;
			firstCh = 0;
		}
		if (debug && currentChar != '\n')
			fprintf(stderr, "%i %i\n", currentChar, currentChar);
		printf("%c", currentChar);
	}
	return 0;
}

int main(int argc, char **argv)
{
	int debug = 0;
	int hex = 0;
	//stdin == 0
	FILE *input = stdin;
	for (int i = 1; i < argc; i++)
	{
		if (strncmp(argv[i], "-D", 2)==0) // task1b arg
		{
			debug++;
			continue;
		}
		if (strncmp(argv[i], "-e", 2) == 0 || strncmp(argv[i], "+e", 2) == 0) // task1c arg
		{
			hex = hexToInt(*argv[i],*(argv[i] + 2));
			continue;
		}
		if (strncmp(argv[i], "-i", 2) == 0) // task1d arg
		{
			input = fopen(argv[i] + 2, "r");
			if (input == NULL)
			{
				fprintf(stderr, "Couldn't open file!\n");
				return 1;
			}
		}
	}
	task1c(input, debug, hex);
	return 0;
}
