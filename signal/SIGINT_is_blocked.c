#include <signal.h>
#include <stdio.h>

int SIGINT_is_blocked(void){
    sigset_t old_sigset;

    /* 現在のマスクをold_sigsetにマスクをセットする */
    sigprocmask(SIG_SETMASK, NULL, &old_sigset);

    return sigismember(&old_sigset, SIGINT);
}

int main(void){
    sigset_t sigset;
    /* 初期化(フラグを全て落とす) */
    sigemptyset(&sigset);

    /* SIGINTを追加 */
    sigaddset(&sigset, SIGINT);

    /* ブロック */
    sigprocmask(SIG_BLOCK, &sigset, NULL);

    printf("%d\n", SIGINT_is_blocked());

    /* 解除 */
    sigprocmask(SIG_UNBLOCK, &sigset, NULL);

    printf("%d\n", SIGINT_is_blocked());

    return 0;

}
