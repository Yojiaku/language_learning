#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int fd[2] = {1, 1};
    pipe(fd);
    int read_fd = fd[0];
    int write_fd = fd[1];

    FILE *fp;
    pid_t child1 = fork();
    if(child1 < 0) printf("Failed to create a process.\n");
    else if(child1 == 0){
        if((fp=fopen("./text.txt", "w+")) == NULL) printf("open file error!\n");
        else{
            printf("Please enter characters('#' for ending):\n");
			char ch = getchar();
			while(ch != '#'){
				fputc(ch, fp);
				ch = getchar();
			}           
        }
        fclose(fp);

    	char pipeStr[] = "OK";
        if(write(write_fd, pipeStr, strlen(pipeStr))) exit(0);
        else{
        	printf("write error\n");
        	exit(1);
        }
    }

	pid_t cpid1 = wait(NULL);
	printf("PID is %d exits\n", cpid1);

    pid_t child2 = fork();
    if(child2 < 0) printf("Failed to create a process.\n");
    else if(child2 == 0){
    	char readBuffer[255];
    	if(read(read_fd, readBuffer, sizeof(readBuffer))){
    		if(strcmp(readBuffer, "OK") == 0){
    			if((fp=fopen("./text.txt", "r")) == NULL) printf("open file error!\n");
		        else{
    				char fileBuffer[255];
		            fgets(fileBuffer, sizeof(fileBuffer), (FILE*)fp);
		            printf("%s\n", fileBuffer);
		        }
		        fclose(fp);
		        exit(0);
    		}else{
    			printf("Sorry.\n");
    			exit(1);
    		}
    	}else{
    		printf("read error\n");
    		exit(1);
    	}
        
    }

    pid_t cpid2 = wait(NULL);
	printf("PID is %d exits\n", cpid2);

    return 0;
}
