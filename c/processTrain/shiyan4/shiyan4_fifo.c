#include <stdio.h>
#include <stdlib.h>

#define MAX 100
#define TRUE 1
#define FALSE 0

int offset = 0;

void fifo(int input, int frame[], int frameSize)
{
	offset = offset % frameSize;
	frame[offset] = input;
	offset += 1;
}

int find(int a, int b[], int size)
{
	int i;
	int flag;
	flag = FALSE;
	for(i=0; i<size; i++)
	{
		if(a == b[i])
		{
			flag = TRUE;
		}
	}
	return flag;
}

int main()
{
	int size = 3; //设置页框为3
	int cpu[MAX];
	int miss = 0;
	FILE* fp;
	int c;
	int count;
	int i, j;
	fp = fopen("workload0", "r");
	printf("\nfor the frame size is %d\n", size);

	count = 0;
	miss = 0;
	offset = 0;
	while(fscanf(fp, "%d*[^0-9]", &c)!=EOF)
	{
		if(find(c, cpu, size)==FALSE)
		{
			fifo(c, cpu, size);
			miss++;
		}
		count++;
	}

	double missRate = miss*1.0/count*100;
	printf("缺失%d次\n", miss);
	printf("共有%d次计数\n", count);
	printf("缺失率%f%%\n", missRate);
	fclose(fp);
}