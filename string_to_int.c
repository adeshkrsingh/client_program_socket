#include<stdio.h>
#include<stdlib.h>
void main()
{
char* str="1234";
int temp=atoi(getenv("SERVER_PORT"));
printf("%d\n",temp);
printf("%s\n",str);
int num=atoi(str);
printf("%d",num);
}
