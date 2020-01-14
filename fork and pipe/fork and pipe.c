#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


#define MAXLINE 4096
#define STDOUT_FILENO 1

int pipe1[2];
int pipe2[2];
char name[12];// 이름을 대시오
int s; // 학번

void client(int, int), server(int, int);

int main (int argc , char *argv[])
{
    
    
    pid_t pid;
    
    pipe(pipe1);
    pipe(pipe2);
    
    printf("파이프 생성\n");
    
    if(pipe(pipe1)<0 || pipe(pipe2)<0){
        printf("pipe error");
        exit(0);
    }
    
    printf("프로세스 생성\n");
    pid = fork();

    
    if(pid == -1)
    {
       printf("fork() error");
    }
    else if(pid == 0)// child 프로세스 생성
    {
        close(pipe1[1]);
        close(pipe2[0]);
        
        client(pipe1[0],pipe2[1]);
        exit(0);
    }
    else
    {
        close(pipe1[0]);
        close(pipe2[1]);
        
        server(pipe1[1], pipe2[0]);
        
        waitpid(pid , NULL , 0);
        exit(0);
        
    }
}

void client(int readfd, int writefd)
{
    size_t len;
    char buff[MAXLINE];
    int flag = 1;
    char type;
    
    len = strlen(buff);

    
    while(flag){
        printf("파일 타입을 선택하시오. r,w,e\n");//r은 읽기 ,w은 수정 e는 프로그램 종료
        scanf("%c",&type);
    
        if(type=='r'){
            printf("조회를 선택하셨습니다.\n");

            write(writefd,buff,len);
            fgets(buff, MAXLINE, stdin);
            read(readfd,buff,len);
            printf("너의 이름과 학번은 -> %s \n", buff);
            
        }
        else if (type == 'w'){
            printf("수정을 선택하셨습니다.");
            printf("이름: ");
            scanf("%s" , name);
            fgets(buff, MAXLINE, stdin);
            write(readfd,&name,len);
       
            printf("학번 : ");
            scanf("%d", &s);
            fgets(buff, MAXLINE, stdin);
            write(writefd,&s,len);
            len = strlen(buff);
            if(buff[len-1] == '\n') len--;
        }
        
        else if(type =='e'){
            printf("종료");
            break;
        }
        
    }
}

void server(int readfd, int writefd)
{
 
    size_t n,len;
    char buff [MAXLINE+1];
    char *inp ;
    int flag = 1;
    char *word;
    char type;
char str[MAXLINE];	
    len = strlen(buff);
    while(flag){

        if(type=='r'){
          
            n=read(readfd,buff,MAXLINE);
            if(n == 0)
            {
                printf("SERVER : read NULL\n");
                exit(0);
            }
            else{
            FILE* file_pointer;
            file_pointer=fopen("buffer.txt","r");
            word = fgets(inp,MAXLINE,file_pointer);
            write(writefd, word, len);
           
        }
    }
        else if (type == 'w')
        {
            if((n=read(readfd, buff, MAXLINE))==0){
                printf("End of file");
                exit(0);
            }
            buff[n]='\0';
            FILE *fd = fopen("buffer.txt", "w");
            if (fd == NULL) return ;
            if((n=read(readfd, buff, MAXLINE))>0)
                buff[n] = '\n';
            write(writefd, buff, n);
            sprintf(buff,"%d",n);
            len = strlen(buff);
            write(writefd, buff, len+1);
            close(writefd);
        }
        else
        {
            flag=0;
        }
    }
}

