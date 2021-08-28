#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void){
    pid_t pid;

    if((pid = fork()) == 0){
	printf("child: parent PID = %d, my PID = %d\n", getppid(), getpid());
    } else {
	printf("parent: myPID = %d, child PID = %d\n", getpid(), pid);
	for(;;)
	    sleep(1);
    }
}
