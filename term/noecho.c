#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int main(void){
    int c;
    struct termios new, old;

    /* 端末ラインディシプリンの設定を取得 */
    tcgetattr(STDIN_FILENO, &old);

    new = old;

    /* エコフラグを落とす */
    new.c_lflag &= ~ECHO;

    /* 設定を反映 */
    tcsetattr(STDIN_FILENO, TCSANOW, &new);
    
    while((c =getchar()) != EOF){
	putchar(c);
    }

    /* 設定を元戻す */
    tcsetattr(STDIN_FILENO, TCSANOW, &old);

    return 0;
}
