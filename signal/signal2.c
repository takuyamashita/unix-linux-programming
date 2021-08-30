#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void handler(int sig){
    signal(SIGINT, handler);
    fprintf(stderr, "@");//XXX
}

int main(void){
    /* ^C = SIGINT */
    signal(SIGINT, handler);

    while(1){
	sleep(1);
	fprintf(stderr, ".");
    }
}
