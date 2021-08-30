#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void alarm_handler(int sig){}

void my_sleep(int seconds){
    /* デフォルトだとプロセスが終了するのでハンドラをセット */
    signal(SIGALRM, alarm_handler);
    /* seconds後にアラームを送る*/
    alarm(seconds);
    /* シグナルが来るまでブロック */
    pause();
}

int main(void){
    printf("hello(1)\n");
    my_sleep(3);
    printf("hello(2)\n");
}
