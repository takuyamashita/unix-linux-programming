#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void handler(int sig){
    printf("hello\n");
    /* 終了 */
    exit(1);
}

int main(void){
    struct sigaction act;
    /* sa_handlerをセット */
    act.sa_handler = handler;
    /* システムコールをハンドラが割り込んだら自動で復帰 */
    act.sa_flags = SA_RESTART;
    /* マスクは空=全てブロック */
    sigemptyset(&act.sa_mask);
    sigaction(SIGINT, &act, NULL);

    while(1);
}
