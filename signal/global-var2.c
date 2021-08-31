#include <signal.h>
#include <stdio.h>
#define MAX 1000000000

static volatile long long counter = 0;

void handler(int sig){
    printf("%lld\n", counter);
}

int main(void){
    long long i;
    signal(SIGINT, handler);

    /* ループ中にカウンターを参照するかもしれない
     * counterを別のレジスタに入れてループ(しながらインクリメント)するように
     * 最適化されてコンパイルされると、counterがループを終わるまで0になってしまう
     * volatileをつけて宣言すれば解決する
     * */
    for(i = 0; i < MAX; i++)
	counter ++;

    printf("counter = %lld\n", counter);
}
