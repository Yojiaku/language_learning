#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <time.h>

#define THREAD_NUM 20
pthread_mutex_t Device_mutex;

//Virtual PCB of threads
struct VirtualPCB
{
	int tid;  //线程号
	char state;  //线程运行状态(F为处于就绪，T为处于执行)
	int priority;
	int arrivetime;  //到达时间
	int cpuburst;  //cpu区间
	int runnedtime; //进程已经运行的时间
}PCB[THREAD_NUM];

void thread_init()
{
	int n;
	srand(time(NULL));
	for(n=0; n<THREAD_NUM; n++)
	{
		PCB[n].tid = n+1; //用线程创建序号作为虚拟进程id
		PCB[n].state = 'F';
		//用随机数随机产生虚拟PCB的值
		PCB[n].priority = 1+rand()%19;
		PCB[n].arrivetime = n; //模拟时，默认进程按创建顺序依次在0时刻到达
		PCB[n].cpuburst = 1+rand()%19;
		PCB[n].runnedtime = 0;
	}
}

void *thread_print(void *num)
{
	int n = *(int *)num;
	while(1)
	{
		pthread_mutex_lock(&Device_mutex);
		printf("Thread%-2d: ", n);
		printf("tid:%-2d 优先级：%-2d 到达时间：%-2d 执行时间：%-2d \n", PCB[n-1].tid, PCB[n-1].priority, PCB[n-1].arrivetime, PCB[n-1].cpuburst);
		pthread_mutex_unlock(&Device_mutex);
		sleep(1);
		break;
	}
	pthread_exit(0);
}

void *Create_children()
{
	int ret[THREAD_NUM];
	thread_init();  //初始化线程
	pthread_t tid[THREAD_NUM];
	//让线程按顺序执行
	pthread_mutex_init(&Device_mutex, NULL);
		//创建20个线程
		int i, j;
		for(i=0; i<THREAD_NUM; i++){
			int idnum = i+1;
			ret[i] = pthread_create(&tid[i], NULL, &thread_print, &idnum);
			if(ret[i]==0){sleep(1);}
			else{printf("Thread%-2d 创建失败！\n", idnum);}
		}
		//等待20个线程运行完成
		for(j=0; j<THREAD_NUM; j++){
			pthread_join(tid[j], NULL);
		}
	pthread_mutex_destroy(&Device_mutex);
	pthread_exit(0);
}

void FCFS(){
	printf("\n------------------先来先服务FCFS调度算法实现结果------------------\n");
	int i, j;
	int clock = 0; //时钟
	float waittime = 0;  //单个进程的等待时间
	float total_waittime = 0;  //FCFS总等待时间
	float average_waittime = 0;  //FCFS平均等待时间
	int starttime = 0;  //每个进程的开始时间
	printf("进程\t 开始时间 运行时间 等待时间 结束时间\n");
	for(i=0; i<THREAD_NUM; i++){
		for(j=0; j<THREAD_NUM; j++){
			if(PCB[j].arrivetime == i && PCB[j].state == 'F'){
				PCB[j].state = 'T';  //将其状态设为T
				starttime = clock;
				waittime = starttime;  //FCFS等待时间就是其开始时间
				total_waittime = total_waittime + waittime;
				clock = clock + PCB[j].cpuburst;
				PCB[j].runnedtime = PCB[j].cpuburst; //进程已经运行的时间就为其CPU区间
				printf("Thread:%-2d \t%-3d \t%-2d \t%-.2f \t%-2d \n", PCB[j].tid, starttime, PCB[j].cpuburst, waittime, clock);
			}
		}
	}
	average_waittime = total_waittime / (float)THREAD_NUM;
	printf("总等待时间:%f\n", total_waittime);
	printf("平均等待时间:%f\n", average_waittime);
}

void SJF(){
	printf("\n------------------最短作业优先调度SJF调度算法实现结果------------------\n");
	//先将FCFS算法中已设置的所有进程状态置为F
	for(int k=0; k<THREAD_NUM; k++){
		PCB[k].state = 'F';
	}
	int i, j;
	int clock = 0;
	float waittime = 0;
	float total_waittime = 0;
	float average_waittime = 0;
	//将所有进程先按照CPU区间从小到大排队
}

int main(){
	//创建主线程
	int ret1;
	pthread_t tid1;
	ret1 = pthread_create(&tid1, NULL, &Create_children, NULL);
	if(ret1==0){
		printf("主线程创建成功！\n");
		sleep(20);
	}
	else{
		printf("主线程创建失败！\n");
	}

	//先来先服务FCFS
	FCFS();
	/*//最短作业优先
	SJF();
	//轮转
	printf("请输入时间片长度：\n");
	int timeslice;
	scanf("%d", &timeslice);
	RR(timeslice);
	//优先级调度
	Priority();
	//多级队列调度*/

	return 0;
}