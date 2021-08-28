#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void){
    if(fork() == 0){
	/* 子プロセスで3秒待つ*/
	sleep(3);
	printf("\nmy parent is now pid %d\n", getppid());
	exit(0);
    }

    printf("parent process, pid %d\n", getpid());
    exit(0);
}
