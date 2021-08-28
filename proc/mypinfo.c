#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void){
    
    pid_t mypid;
    char command[1024];

    mypid = getpid();
    printf("pid = %d\n", mypid);

    if(snprintf(command, sizeof(command), "ps -l -p %d", mypid) >= sizeof(command)){
	fprintf(stderr, "too long command line (pid = %d)\n", mypid);
	exit(1);
    }

    system(command);

    return 0;
}

