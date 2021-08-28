#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    int ac, status, bg;
    pid_t cpid, zpid;

    for(;;){
	/* 状態を返せるプロセスが無ければ待たない */
	while((zpid = waitpid(-1, &status, WNOHANG)) > 0)
	    fprintf(stderr, "process %d salvaged\n", zpid);
	
	/* waitpidをコールした時点で子プロセスが無い場合以外のエラー */
	if(zpid == -1 && errno != ECHILD){
	    perror("waitpid");
	    exit(1);
	}

	getstr("@ ", cmd, sizeof(cmd));

	if(feof(stdin)){
	    /* 入力途中で^Dが入力された場合 */
	    exit(0);
	} else if (ferror(stdin)){
	    /* ストリームエラー */
	    perror("getstr");
	    exit(1);
	}

	if((ac = strtovec(cmd, av, MAXARGV)) > MAXARGV){
	    fputs("argument list too ling \n", stderr);
	    continue;
	}

	/* nullポインタ分を引く */
	ac--;

	/* 入力なし */
	if(ac == 0)
	    continue;
	
	if(!strcmp(av[ac-1], "&")){
	    /* バックグランド実行命令の場合 */
	    
	    /* {"ls", "-a", NULL, "&"} の状態にして
	     * 子プロセスではフォアグラウンドで実行させる
	     */
	    av[ac-1] = NULL;
	    ac--;
	    bg = 1;
	} else {
	    bg = 0;
	}

	if((cpid = fork()) == -1){
	    perror("fork");
	    exit(1);
	} else if (cpid == 0){
	    /* 子プロセス */
	    execvp(av[0], av);
	    perror(av[0]);
	    exit(1);
	}

	if(!bg){
	    /* フォアグラウンドなので終了を待つ*/
	    if(waitpid(cpid, &status, 0) == (pid_t)-1){
		perror("waitpid(1)");
		exit(1);
	    }

	    fprintf(stderr, "process %d finished\n", cpid);
	}
    }
    exit(0);
}
