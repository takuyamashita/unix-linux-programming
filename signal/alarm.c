#include <stdio.h>
#include <unistd.h>

void loop(void){
    int i;
    for(i = 0; i < 0xFFFFFF; i ++);
}

int main(void){
    /* SIGALRMのデフォルトの動作で実行終了 */
    /* 3秒後にSIGALRMをこのプロセスに送る */
    alarm(3);

    while(1){
	/* 時間稼ぎ */
	loop();
	printf(".");

	fflush(stdout);
    }
}
