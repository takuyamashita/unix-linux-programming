#include <signal.h>

int main(void){
    sigset_t sigset;

    sigfillset(&sigset);
    sigdelset(&sigset, SIGINT);
    sigdelset(&sigset, SIGQUIT);

}
