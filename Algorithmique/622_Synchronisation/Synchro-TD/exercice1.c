#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	int x = 0;
	int i;
	int PID = fork();
	for (i = 0; i < 10; i++)
	{
		if (PID == 0)
		{
			x--;
			printf("Je suis le fils et x vaut : %d \n",x);
			sleep(1);
		}
		else
		{
			x++;
			printf("Je suis le père et x vaut : %d \n",x);
			sleep(1);
		}
	}
}
