#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static sigjmp_buf env;

void sig_handler(int sig){
    printf("handler\n");
    siglongjmp(env, 1);
}

int main(void){
    int i;
    struct sigaction act;

    act.sa_handler = sig_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);
    sigsetjmp(env, 1);

    for(i = 0; ; i ++){
	printf("%d\n", i);
	sleep(1);
    }

    return 0;
}
