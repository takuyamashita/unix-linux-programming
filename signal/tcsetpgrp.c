#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void){
    /* 親プロセスには子プロセスのpidが返される */
    pid_t pid = fork();
    int c = (pid == 0 ? '.' : '@');

    if(pid != 0){
	/* 親プロセス */

	/* 子プロセスidをグループプロセスidににしてグループを作成、移動*/
	setpgid(pid, pid);
	/* 子プロセスグループをフォアグランドにする */	
	tcsetpgrp(1, pid);
    }

    while(1){
	fprintf(stderr, "%c", c);
	sleep(1);
    }
}
