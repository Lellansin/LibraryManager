#include <stdio.h>

struct student{
	long stuID;
	int score[4];
	char stu[10];
};

int main()
{
	printf("%d \n", sizeof(long));
	printf("%d \n", sizeof(long) + sizeof(char [10]) + sizeof(int [4]));
	printf("%d \n", sizeof(struct student));
}
