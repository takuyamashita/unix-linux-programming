#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <unistd.h>

int main(void){
    int ifd;
    ssize_t cc;
    char buf[1024];

    if((ifd = open("input.file", O_RDONLY)) == -1){
	perror("open");
	exit(1);
    }

    while((cc = read(ifd, buf, sizeof(buf))) > 0){
	if(write(STDOUT_FILENO, buf, cc) < cc){
	    perror("write");
	    exit(1);
	}
    }

    if(cc == -1){
	perror("read");
	exit(1);
    }

    if(close(ifd) == -1){
	perror("close");
	exit(1);
    }

    return 0;
}
