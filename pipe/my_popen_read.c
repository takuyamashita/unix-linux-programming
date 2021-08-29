#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

int my_popen_read(const char *command){
    int fd[2];
    pid_t pid;

    pipe(fd);

    if((pid = fork()) == 0){
	/* 子プロセス */

	/* 標準出力がパイプのINを指すようにする */
	dup2(fd[1], 1);
	
	close(fd[0]);
	close(fd[1]);

	/* このプロセスの環境を引き継ぐ
	 * 環境変数PATHを使用する
	 * 引数はリスト(配列)で指定する
	 * */
	execlp(command, command, (char *)NULL);
    }

    close(fd[1]);
    return fd[0];
}

int main(void){
    int fd, nbytes;
    char buf[1024];
    
    /* lsの出力が入っているパイプのOUTのファイルディスクリプタ */
    fd = my_popen_read("ls");
    
    while(1){
	nbytes = read(fd, buf, sizeof(buf));
	if(nbytes <= 0) break;
	write(STDOUT_FILENO, buf, nbytes);
    }

    close(fd);

    return 0;
}
