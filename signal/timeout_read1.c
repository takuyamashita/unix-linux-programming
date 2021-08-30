#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

void alarm_handler(int sig){}

ssize_t timeout_read(int fd, void *buf, size_t count, int seconds){
    int ret;
    struct sigaction act;

    /* 終了しないようにする */
    act.sa_handler = alarm_handler;
    /* 何も指定しない */
    act.sa_flags = 0;
    /* 全てブロックする */
    sigemptyset(&act.sa_mask);
    /* シグナルを設定 */
    sigaction(SIGALRM, &act, NULL);

    /* seconds秒後にSIGALRMをこのプロセスに送信 */
    alarm(seconds);

    /* XXX:ここで例えばカーネルのスケジューリングで
     * 他のプロセスがseconds秒以上処理をさせていたらまずい
     * 割り込みが入らずにタイムアウトできない
     * */

    /* 読み込む 
     * 割り込みが発生すればEINTERをerrnoにセットして
     * -1を返す
     * */
    ret = read(fd, buf, count);

    /* アラーム解除
     * タイムアウトせずにreadから復帰したらセットされたままのため
     * */
    alarm(0);

    return ret;
}

int main(void){
    char buf[1024];
    ssize_t ret;

    printf("Please type your name: ");
    fflush(stdout);

    ret = timeout_read(STDIN_FILENO, buf, sizeof(buf), 3);

    if(ret == -1){
	printf("timed out\n");
    } else {
	write(STDOUT_FILENO, buf, ret);
    }
}
