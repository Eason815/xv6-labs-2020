#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MSG 32

int main(int argc, char *argv[]){

    int p[2];
    char buf[MSG];
    pipe(p);

    int pid = fork();

    if(pid > 0){
        write(p[1],"ping",MSG);
        wait(0);
        read(p[0],buf,MSG); fprintf(1,"%d: received %s\n",getpid(),buf);
    }else if(pid == 0){
        read(p[0],buf,MSG); fprintf(1,"%d: received %s\n",getpid(),buf);
        write(p[1],"pong",MSG);
    }else{
        fprintf(2,"fork error\n");
        exit(1);
    }
    exit(0);
}