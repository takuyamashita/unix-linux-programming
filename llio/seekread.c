#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <unistd.h>

int main(void){
    int d, cc;
    char buf[5];
    off_t pos;

    if((d = open("input.file", O_RDONLY)) == -1){
	perror("open");
	exit(1);
    }

    if((pos = lseek(d, 10, SEEK_SET)) == -1){
	perror("lseek");
	exit(1);
    }

    printf("new offset = %d\n", (int)pos);

    if((cc = read(d, buf, sizeof(buf))) == -1){
	perror("read");
	exit(1);
    }

    if((pos = lseek(d, 0, SEEK_CUR)) == -1){
	perror("lseek");
	exit(1);
    }

    if(close(d) == -1){
	perror("close");
	exit(1);
    }

    printf("offset after read = %d\n", (int)pos);
    printf("number of bytes read = %d\n", cc);
    
    fflush(stdout);
    if(cc > 0 ){
	if(write(STDOUT_FILENO, buf, cc) == -1){
	    perror("write");
	    exit(1);
	}
    }
    putchar('\n');

    return 0;
}
