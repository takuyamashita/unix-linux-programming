#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

void handler(int sig){
    pid_t pid;

    /* 任意のプロセスを待つ
     * 状態を返せるプロセスが無ければ復帰
     * 子プロセスの状態は要らない
     * */
    while((pid = waitpid(-1, NULL, WNOHANG)) != 0){
	if(pid == -1){
	    /* 子プロセスがいない */
	    if(errno == ECHILD) break;
	    /* シグナルの割り込み発生 */
	    if(errno == EINTR) continue;

	    perror("waitpid");
	    exit(1);
	}
	printf("handler: pid = %d\n", pid);
    }
}

int main(void){
    pid_t pid = fork();
    signal(SIGCHLD, handler);

    if(pid == 0){
	/* 子プロセス */
	execlp("cat", "cat", (char *)0);
    }

    while(1){
	fprintf(stderr, ".");
	sleep(1);
    }
}

