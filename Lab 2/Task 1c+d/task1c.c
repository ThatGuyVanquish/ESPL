#include <stdio.h>

int main(int argc, char** argv)
{
	int iarray[3];
	float farray[3];
	double darray[3];
	char carray[3];
	int* iarrayPtr = iarray;
	char* carrayPtr = carray;
	printf("iarray: %p\niarray + 1: %p\n", iarray, iarray + 1);
	printf("iarray via pointer: %p\niarray + 1 via pointer: %p\n", iarrayPtr, iarrayPtr + 1);
	printf("farray: %p\nfarray + 1: %p\n", farray, farray + 1);
	printf("darray: %p\ndarray + 1: %p\n", darray, darray + 1);
	printf("carray: %p\ncarray + 1: %p\n", carray, carray + 1);
	printf("carray via pointer: %p\ncarray + 1 via pointer: %p\n", carrayPtr, carrayPtr + 1);
	int* p;
	printf("local pointer: %p\n", p);
}
