#include <stdio.h>
#include <stdlib.h> // perror
int main()
{
    FILE *fp;
    char ch, filename[255];
    printf("Please enter the file's name(in 255 characters):\n");
    scanf("%s", filename);
    if((fp = fopen(filename, "w+")) == NULL){
        perror("fopen()");
        exit(1);
    }else{
        ch = getchar(); // the variable "ch" needs initialize
        printf("Please enter characters('#' for end):\n");
        while(ch != '#'){
            fputc(ch, fp);
            ch = getchar();
        }
        fclose(fp);
    }
    
    return 0;
}
