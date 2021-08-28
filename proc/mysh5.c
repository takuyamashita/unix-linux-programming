#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include "../sub/mysub.h"
#include "myvec.h"

enum {
    MAXARGV = 100,
};

int main(void){
    char cmd[1024];
    char *userinput[MAXARGV], *av[MAXARGV], *infile, *outfile;
    int i, nwords, ac, status;
    pid_t cpid;

    for(;;){
	getstr("@ ", cmd, sizeof(cmd));
	
	if(feof(stdin)){
	   exit(1); 
	} else if(ferror(stdin)){
	    perror("getstr");
	    exit(1);
	}

	if((nwords = strtovec(cmd, userinput, MAXARGV)) > MAXARGV){
	    fputs("too many arguments\n", stderr);
	    continue;
	}

	nwords--;
	if(nwords == 0)
	    continue;
	
	ac = 0;
	infile = outfile = NULL;

	for(i = 0; i < nwords; i++){
	    if(strcmp(userinput[i], ">") == 0){
		/* {"ls", ">", "test.txt"} みたいな場合 
		 * "test.txt"をoutfileに入れる
		 */
		outfile = userinput[i + 1];
		i++;
	    } else if(strcmp(userinput[i], "<") == 0){
		/* {"ls", "<", "test.txt"} みたいな場合 
		 * "test"をinfileに入れる
		 */
		infile = userinput[i + 1];
		i++;
	    } else {
		av[ac++] = userinput[i];
	    }
	}
	av[ac] = NULL;

	if((cpid = fork()) == -1){
	    perror("fork");
	    continue;
	} else if(cpid == 0){
	    /* 子プロセス */
	    if(infile != NULL) {
		/* ls < text.txt みたいな場合 */
		close(0);
		if(open(infile, O_RDONLY) == -1){
		    perror(infile);
		    exit(1);
		}
	    }

	    if(outfile != NULL){
		/* ls > text.text みたいな場合 */
		close(1);
		if(open(outfile, O_WRONLY|O_CREAT|O_TRUNC, 0666) == -1){
		    perror(outfile);
		    exit(1);
		}
	    }
	    
	    printf("%s\n", av[0]);

	    execvp(av[0], av);
	    perror(cmd);
	    exit(1);
	}

	/* 親プロセス */
	if(wait(&status) == (pid_t)-1){
	    perror("wait");
	    exit(1);
	}
    }
    exit(0);
}

