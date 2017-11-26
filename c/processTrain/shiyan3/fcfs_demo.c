#include <stdio.h>
#include <stdlib.h>

typedef struct PCB  //定义进程控制块
{
	//pthread_t id;
	char ID[3]; //进程号
	char name[10]; //进程名
	char state;  //运行状态
	int arrivetime; //到达时间
	int starttime; //进程开始时间
	int finishtime; //进程结束时间
	int servicetime; //进程服务时间
	float turnaroundtime; //周转时间
	float weightturnaroundtime; //带权周转时间
	struct PCB *next;  //指向下一个进程
}pcb;

int timer; //计时器
int n; //进程个数

pcb *head = NULL, *p, *q;  //进程链表指针

void run_fcfs(pcb *pl)  //运行未完成的进程
{
	timer = pl->arrivetime > timer ? pl->arrivetime : timer;
	pl->starttime = timer;
	printf("现在时间是%d，开始运行作业%s\n", timer, pl->name);
	timer += pl->servicetime;
	pl->state = 'T';
	pl->finishtime = timer;
	pl->turnaroundtime = pl->finishtime - pl->arrivetime;
	pl->weightturnaroundtime = pl->turnaroundtime / pl->servicetime;
	printf("ID 到达时间 开始时间 服务时间 完成时间 周转时间 带权周转时间 \n");

	printf("%s%6d%10d%10d%8d%10.1f%10.2f\n", pl->ID, pl->arrivetime, pl->starttime, pl->servicetime, pl->finishtime, pl->turnaroundtime, pl->weightturnaroundtime);
}

void fcfs()  //找到当前未完成的进程
{
	int i, j;
	p = head;
	for(i=0; i<n; i++){
		if(p->state == 'F'){
			q = p;  //标记当前未完成的进程
			run_fcfs(q);
		}
		p = p->next;
	}
}

void getInfo() //获取进程信息并创建进程
{
	int num;
	printf("作业个数:\n");
	scanf("%d", &n);

	for(num=0; num<n; num++){
		p = (pcb *)malloc(sizeof(pcb));
		printf("依次输入：\nID 进程名 到达时间 服务时间\n");
		scanf("%s\t%s\t%d\t%d", &p->ID, &p->name, &p->arrivetime, &p->servicetime);
		if(head == NULL){
			head = p;
			q = p;
			timer = p->arrivetime;
		}
		if(p->arrivetime < timer){
			timer = p->arrivetime;
		}
		q->next = p;
		p->starttime = 0;
		p->finishtime = 0;
		p->turnaroundtime = 0;
		p->weightturnaroundtime = 0;
		p->next = NULL;
		p->state = 'F';
		q = p;
	}
}

void main(){
	printf("先来先服务算法模拟\n");
	getInfo();
	p = head;
	fcfs();
}