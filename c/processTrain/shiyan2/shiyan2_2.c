#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define FILEPATH "./text2.txt"
#define PROJID 1234

struct msgbuf
{
	long mtype;
	char mtext[255];
};

int main(){
	int msgid;
	key_t key;
	struct msgbuf buf;

	key = ftok(FILEPATH, PROJID);
	if(key == -1){
		perror("ftok()");
		exit(1);
	}

	msgid = msgget(key, IPC_CREAT|IPC_EXCL|0600);
	if(msgid == -1){
		perror("msgget()");
		exit(1);
	}

	char fileBuffer[255];
	FILE* fp;
	pid_t child1 = fork();
	if(child1 < 0) printf("Failed to create a process.\n");
	else if(child1 == 0){
		if((fp = fopen("./text2.txt", "w+"))==NULL) perror("fopen()");
		else{
			printf("Please enter characters('#' for ending):\n");
			char ch = getchar();
			while(ch != '#'){
				fputc(ch, fp);
				ch = getchar();
			}
		}
		fclose(fp);

		buf.mtype = 1;
		char successStr[] = "OK";
		strncpy(buf.mtext, successStr, strlen(successStr));
		if(msgsnd(msgid, &buf, strlen(buf.mtext), 0) == -1){
			perror("msgsnd()");
			exit(1);
		}

		exit(0);
	}

	pid_t cpid1 = wait(NULL);
	printf("pid %d exits.\n", cpid1);

	pid_t child2 = fork();
	if(child2 < 0) printf("Failed to create a process.\n");
	else if(child2 == 0){
        if(msgrcv(msgid, &buf, strlen(buf.mtext), 1, 0) == -1){
        	perror("msgrcv()");
        	exit(1);
        }else{
        	if((fp=fopen("./text2.txt", "r")) == NULL) printf("open file error!\n");
	        else{
	            fgets(fileBuffer, sizeof(fileBuffer), (FILE*)fp);
	            printf("%s\n", fileBuffer);
	        }
	        fclose(fp);
        }

        exit(0);
	}

	pid_t cpid2 = wait(NULL);
	printf("pid %d exits.\n", cpid2);

	int reval = msgctl(msgid, IPC_RMID, NULL); // 删除消息队列
	if(reval == -1){
		printf("unlink msg queue error\n");
		return 0;
	}
	return 0;
}