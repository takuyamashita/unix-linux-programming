#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "../sub/mysub.h"
#include "myvec.h"

enum {
    MAXARGV = 100,
};

int main(void){
    char cmd[1024];
    char *av[MAXARGV];
    int ac, status;
    pid_t cpid;

    for(;;){
	getstr("@ ", cmd, sizeof(cmd));
	
	/*入力が長い場合*/
	if(feof(stdin)){
	    exit(0);
	} else if(ferror(stdin)) {
	    perror("getstr");
	    exit(1);
	}

	if((ac = strtovec(cmd, av, MAXARGV)) > MAXARGV){
	    fputs("too many arguments\n", stderr);
	    continue;
	}

	if(ac == 1)
	    continue;
	if((cpid = fork()) == -1){
	    perror("fork");
	    continue;
	} else if (cpid == 0){
	    /* child */
	    execvp(av[0], av);
	    perror(cmd);
	    exit(1);
	}

	if(wait(&status) == (pid_t)-1){
	    perror("wait");
	    exit(1);
	}
    }

    exit(0);
}
