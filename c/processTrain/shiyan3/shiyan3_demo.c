#include <stdio.h>  
#include <stdlib.h>  
  
#define getpch(type) (type*)malloc(sizeof(type))   
//#define NULL 0  
  
//定义PCB结构体  
struct pcb  
{  
    char name[10];      //进程名称  
    int state;          //进程状态  
    int super;          //优先级  
    int ntime;          //总运行时间  
    int rtime;          //已运行时间  
    struct pcb* link;   //下一PCB的地址  
}                       //定义3个pcb类型指针,其中:  
    *ready=NULL,        //……"ready" 代表就绪队列链表首位地址  
    *p,                 //……"p"     代表当前正在运行的pcb地址  
    *bhead=NULL;        //……"bhead" 代表阻塞链表的首位地址  
  
//把结构体pcb定义成一个数据类型:PCB  
typedef struct pcb PCB;  
  
//排序函数:对就绪队列里面的进程根据优先级进行排序  
void sort()  
{  
    PCB *first,*second; //没什么好说的  
    int insert=0;       //该值用于标记在排序过程中是否发生过插队情况  
  
    //如果[就绪链表首项为空]或[当前进程的优先数高于链表首项进程的优先数]  
    if ((ready==NULL)||((p->super)>(ready->super)))  
    {  
        //将当前进程置于就绪链表首位  
        p->link=ready;  
        ready=p;  
    }  
    else //否则嘛 ^_^  
    {  
        //取出链表中的排在最前两项,分别为first,second  
        first=ready;  
        second=first->link;  
  
        //循环,直到second所指向的地址为空时才停止  
        while(second!=NULL)  
        {  
            //如果p的优先级大于second  
            if((p->super)>(second->super))  
            {  
                //将p插于first和second之间  
                p->link=second;  
                first->link=p;  
  
                second=NULL;    //设置second为空(这是循环结束的条件)  
                insert=1;       //将的值"insert"设置为1  
            }  
            else //否则 0.0  
            {     
                //first和second所指的地址向链表后移位  
                //举个栗子:循环的第一次,first和second分别指向链表的第1位和第2位;那么第二次循环则分别指向链表的第2位和第3位  
                first=first->link;second=second->link;  
            }  
        }  
        //如果没有发生过插队情况  
        if(insert==0) {  
            first->link=p;       //将p排在first之后(注:此时循环已经结束,再没有发生过插队的情况下,first即是队尾)  
        }  
    }  
}  
  
//阻塞函数:阻塞就绪队列中(从前往后)优先级相同的连续进程  
void block(){  
    PCB *pb,*index=ready;  
    int count=0;  
  
    if(ready==NULL)return;          //如果就绪队列中没有进程,则终止阻塞操作(都没有进程我还阻塞个啥?)  
  
    //循环,遍历整个就绪链表  
    while((index->link!=NULL))  
    {  
        /** 
         * 这里需要细致写一下: 
         * 一个进程是否被阻塞,其主要依据是优先数.即有连续多个优先数相等的进程排列在一起时就会发生阻塞. 
         * 这个循环所进行的工作是循环遍历就绪链表的每一项,通过依次与就序列表的首项进程的优先数比较,判断出优先数是否发生减小. 
         * 发生减小则代表连续的序列发生中断,则跳出循环. 
         * 此时pcb类型的指针"index"已指向[优先数相等进程的序列]的最后一项. 
         */   
        if((index->link->super)>=ready->super){  
            index->link->state='B';   //将进程状态标记为'B',即"block".  
            index=index->link;       //用以链表循环的循环指针向后移一位.  
            count++;                //每检测到可阻塞进程时自增计数  
        }else{  
            break;  
        }  
    }  
    //判断当就绪链表中可阻塞进程大于0时  
    if(count>0){  
        /** 
         * 这里还是需要详细注解: 
         * 通过循环找出了[优先数相等进程的序列]后需要将这条子链表从就绪链表中取出,(注:这个子链表是从就绪链表的第二项开始的,如果都阻塞了你运行啥去?) 
         * 再将这个子链表加入阻塞链表队首(这里之所以加到队首而不是队尾,是为了节省寻找队尾时产生的循环的开销) 
         * 就绪链表此时由于被取走一段子链表,需要将原先子链表首尾部分重新连接. 
         */  
        pb=index->link;  
        index->link=bhead;  
        bhead=ready->link;  
        ready->link=pb;  
    }  
  
}  
  
//唤醒函数:将阻塞队列中优先数最大且剩余运行时间最小的进程调入就绪链表首位  
void weakup(){  
    PCB *index,*prior,*t;       //"index"和"prior"的作用请参考[sort]排序函数中的"first"和"second"  
                                //"t"的作用是用于两个指针调换位置时使用的中间变量.  
    int exchange=0;             //该值用于标记是否需要进行位置调换操作  
    if(bhead==NULL)return;      //如果就阻塞列中没有进程,则终止唤醒操作(都没有进程我还唤醒个啥?)  
  
    //如果阻塞链表不止一个进程  
    if(bhead->link!=NULL)  
    {  
        index=bhead->link;  
        prior=bhead;  
        //循环,遍历整个阻塞链表  
        while(index!=NULL)  
        {  
            /** 
             * 虽然写备注快写吐了,但这里还是需要详细描述一下: 
             * 唤醒阻塞链表中的哪一个进程,其判断依据有两点: 
             *      1.优先数在阻塞链表中最判断大; 
             *      2.当优先数最大的进程不唯一时,判断进程的剩余运行时间,剩余运行时间少的进程优先执行. 
             * 这里采用的是一次循环,所以在循环时每当找到比之前进程更符合条件的进程时都将该进程与阻塞链表首尾的进程调换位置. 
             * 这样我们再循环结束时就可知道,阻塞链表首位的进程是应该唤醒的进程. 
             */  
            if((index->super)>(bhead->super))  
            {  
                exchange=1;         //标记需要进行位置调换操作  
            }  
            else if((index->super)==(bhead->super))  
            {  
                if((index->ntime-index->rtime)<(bhead->ntime-bhead->rtime))  
                {  
                    exchange=1;     //标记需要进行位置调换操作  
                }  
            }  
  
            if(exchange==1){  
                //与阻塞链表首位进程调换位置  
                prior->link=bhead;  
                t=bhead->link;  
                bhead->link=index->link;  
                index->link=t;  
                bhead=index;  
                //将标记变量exchange重新标记为0  
                exchange=0;  
            }  
            prior=index;        //"prior"向后移动一位(这里之所以不使用"prior=prior->link"是因为执行交换位置操作时,prior的后继地址将不为index)  
            index=index->link;   //"index"向后移动一位  
        }  
    }  
  
    //将阻塞链表首位的进程调到就绪列表首位  
    t=bhead->link;  
    bhead->link=ready;  
    ready=bhead;  
    bhead=t;  
}  
  
//录入进程信息  
void input()  
{  
    int i,num;  
    printf("\n请输入进程数量:");  
    scanf("%d",&num);  
  
    //循环录入每个进程的信息  
    for (i=0;i<num;i++)  
    {  
        //赋予每个进程初始属性值  
        printf("\n[请设置 %d号 进程的基本属性]\n",i+1);  
        p=getpch(PCB);  //getpch(type)是宏定义函数,使用malloc函数根据数据类型,动态开辟内存空间  
        printf("\n  名称：");  
        scanf("%s",p->name);  
        printf("\n  优先级：");  
        scanf("%d",&p->super);  
        printf("\n  所需时间：");  
        scanf("%d",&p->ntime);  
        printf("\n");  
  
        p->rtime=0;  
        p->state='W';    //'W'等待状态,'R'运行状态,'B'阻塞状态  
        p->link=NULL;  
        sort();         //执行排序  
    }  
}  
  
//获取链表长度(不用看,没啥o用)  
int space()  
{  
    int l=0;PCB*pr=ready;  
    while(pr!=NULL)  
    {  
        l++;  
        pr=pr->link;  
    }  
    return(l);  
}  
  
//显示列名(不用看,没啥o用)  
void show()  
{  
    printf("\n名称\t状态\t优先数\t总时长\t已用时\n");  
}  
  
//显示指定进程的信息  
void disp(PCB*pr)  
{  
    printf("%s\t",pr->name);  
    printf("%c\t",pr->state);  
    printf("%d\t",pr->super);  
    printf("%d\t",pr->ntime);  
    printf("%d\t",pr->rtime);  
    printf("\n");  
}  
  
//显示当前正在运行的进程和就绪队列内进程的信息  
void check()  
{  
    PCB *pr,*pb;  
    printf("\n====[当前运行中的进程]====\n");  
    printf("进程名称:%s\n",p->name);  
    show(); //显示列名  
    disp(p);//显示当前正在运行的进程信息  
    pr=ready;  
    if(pr==NULL){  
        printf("\n====[当前就绪队列为空]====\n");  
    }  
    else  
    {  
        printf("\n====[当前就绪队列状态]====");  
        show();  
        while(pr!=NULL)  
        {  
            disp(pr);  
            pr=pr->link;  
        }  
    }  
  
    pb=bhead;  
    if(pb==NULL){  
        printf("\n====[当前阻塞队列为空]====\n");  
    }  
    else  
    {  
        printf("\n====[当前阻塞队列状态]====");  
        show();  
        while(pb!=NULL)  
        {  
            disp(pb);  
            pb=pb->link;  
        }  
    }  
}  
  
//销毁函数:销毁当前正在运行的进程  
void destroy()  
{  
    printf("\n进程[%s]已完成.\n",p->name);  
    free(p);    //free函数用于释放内存空间  
}  
  
//运行函数  
void running()  
{  
    (p->rtime)++;    //当前进程的已运行时间自增  
    //如果进程实际运行时间已经等于了预设的总时长  
    if(p->rtime==p->ntime)  
    {  
        destroy();  //办掉他!  
    }  
    else //否则啊 @.@  
    {  
        (p->super)--;    //进程优先数减一  
        p->state='W';    //状态标记为等待  
        sort();         //就绪队列重新排序  
        //如果阻塞链表不为空则唤醒阻塞进程  
        if(bhead!=NULL){  
            weakup();  
        }  
        block();        //执行阻塞操作  
    }  
}  
  
//主函数  
void main()  
{  
    int len,h=0;  
    char ch;  
    input();        //录入进程信息  
    len=space();    //获取链表长度(没啥o用)  
  
    //只要链表长度不等于0且链表首项不为空就一直循环下去  
    while((len!=0)&&(ready!=NULL))  
    {  
        //system("cls");  
        ch=getchar();  
        h++;  
        printf("\n\n[第%d次运行]\n",h);  
  
        //取就绪链表的首项进程,转为正在进行状态  
        p=ready;  
        ready=p->link;  
        p->link=NULL;  
        p->state='R';  
  
        check();    //显示当前运行状态信息  
        running();  //运行  
  
        printf("\n按任一键继续.....");  
        ch=getchar();  
    }  
    printf("\n\n进程已经完成.\n");  
    ch=getchar();  
}  