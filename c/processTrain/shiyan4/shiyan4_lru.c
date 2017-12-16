#include <stdio.h>
#include <stdlib.h>

#define MAX 100
#define TRUE 1
#define FALSE 0

typedef struct cpuBlock
{
	int value;
	int countNum;
}cpuBlock;

void lru(int input, cpuBlock frame[], int frameSize)
{
	int maxCount = -1; //最大访问次数,即要淘汰的页号
	int changeNum;  //要淘汰的页号
	int i;
	for(i=0; i<frameSize; i++)
	{
		
		if(frame[i].value == -1)
		{
			changeNum = i;
			break;
		}else{
			if(frame[i].countNum > maxCount)
			{
				maxCount = frame[i].countNum;
				changeNum = i;
			}
		}
		frame[i].countNum += 1;

	}
	frame[changeNum].value = input;
	frame[changeNum].countNum = 0;
	//printf("maxCount = %d\n", maxCount);
	maxCount = -1;
	//printf("changeNum = %d\n", changeNum);
	//printf("frame[changeNum] = %d\n", frame[changeNum].value);

}

int find(int a, cpuBlock b[], int size)
{
	int i;
	int flag;
	flag = FALSE;
	for(i=0; i<size; i++)
	{
		b[i].countNum += 1;
		if(a == b[i].value)
		{
			flag = TRUE;
			b[i].countNum = 0;
		}
	}
	return flag;
}

int main()
{
	int size = 5; //设置页框为3
	cpuBlock cpu[MAX];  //分配页框
	int miss = 0;
	FILE* fp;
	int c;
	int count;
	int i, j;
	for(i=0; i<size; i++)  //初始化页框
	{
		cpu[i].value = -1;
		cpu[i].countNum = 0;
	}

	fp = fopen("workload0", "r");
	printf("\nfor the frame size is %d\n", size);

	count = 0;
	miss = 0;
	while(fscanf(fp, "%d*[^0-9]", &c)!=EOF)
	{
		if(find(c, cpu, size)==FALSE)
		{
			lru(c, cpu, size);
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