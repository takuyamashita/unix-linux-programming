#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void handler(int sig){
    printf("hello\n");
    exit(1);
}

int main(void){
    signal(SIGINT, handler);
    while(1);
}
