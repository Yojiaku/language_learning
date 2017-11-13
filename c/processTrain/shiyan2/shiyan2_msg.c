#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define FILEPATH "/home/yangruo/workspace/processTrain/shiyan2/text2.txt"
#define PROJID 1234

int main(){
	int msgid;
	key_t key;
	struct msqid_ds msg_buf;
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

	if(msgctl(msgid, IPC_STAT, &msg_buf) == -1){
		perror("msgctl()");
		exit(1);
	}

	printf("msgid: %d\n", msgid);
    printf("msg_perm.uid: %d\n", msg_buf.msg_perm.uid);
    printf("msg_perm.gid: %d\n", msg_buf.msg_perm.gid);
    printf("msg_stime: %lu\n", msg_buf.msg_stime);
    printf("msg_rtime: %lu\n", msg_buf.msg_rtime);
    printf("msg_qnum: %lu\n", msg_buf.msg_qnum);
    printf("msg_qbytes: %lu\n", msg_buf.msg_qbytes);

	return 0;
}
