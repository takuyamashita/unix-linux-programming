#include <signal.h>
#include <stdio.h>

#define MAX 1000000

static volatile long long counter1 = 0, counter2 = 0;

void handler(int sig){
    counter1 ++;
    counter2 ++;
}

int main(void){
    long long i;
    sigset_t set, oset;

    signal(SIGINT, handler);
    sigemptyset(&set);
    sigaddset(&set, SIGINT);

    for(i = 0; i < MAX; i++ ){
	/* インクリメントの最中
	 * 例えばmovlとaddlの間にハンドラが割り込まれると
	 * 値がおかしくなる
	 * それを防ぐ
	 * */
	sigprocmask(SIG_BLOCK, &set, &oset);
	counter1++;
	sigprocmask(SIG_SETMASK, &oset, NULL);
    }
    
    /* printfも途中で割り込まれる可能性がある*/
    sigprocmask(SIG_BLOCK, &set, &oset);
    printf("counter1 = %lld\n", counter1);
    printf("counter2 = %lld\n", counter2);
    sigprocmask(SIG_SETMASK, &oset, NULL);
    
}

