#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int fd;

    if(argc < 3){
	fprintf(stderr, "%s filename command [arg...]\n", argv[0]);
	exit(1);
    }

    /* 入力元のファイルを開く */
    fd = open(argv[1], O_RDONLY);

    /* fdをSTDIN_FILENOにコピー
     * fdが差しているFILE構造体を
     * STDIN_FILENOも指すようにする
     * */
    dup2(fd, STDIN_FILENO);
    execvp(argv[2], &argv[2]);
}
