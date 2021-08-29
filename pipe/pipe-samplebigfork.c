#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>

enum {
    SIZE = 1024 * 1024,
};

int main(void){
    int n, fd[2];
    char buf[SIZE], buf2[SIZE] = "Hello\n";
    pid_t pid;

    pipe(fd);

    if((pid = fork()) == 0){
	/* 子プロセス　「読み」に使用 */
	close(fd[1]);
	while((n = read(fd[0], buf, sizeof(buf))) > 0){
	    printf("%6sn = %d\n", buf, n);
	}
	close(fd[0]);
    } else {
	/* 親プロセス 「書き」に使用 */
	close(fd[0]);
	write(fd[1], buf2, sizeof(buf2));
	close(fd[1]);
	wait(NULL);
    }

    return 0;
}
