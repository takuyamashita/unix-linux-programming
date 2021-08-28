#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <unistd.h>

int main(void){
    int fd;

    if((fd = open("bye.txt", O_WRONLY)) < 0){
	perror("open");
	exit(1);
    }

    if(lseek(fd, 5, SEEK_SET) == -1){
	perror("lseek");
	exit(1);
    }

    if(write(fd, "night!\n", 7) != 7){
	perror("write");
	exit(1);
    }

    if(close(fd) < 0){
	perror("close");
	exit(1);
    }

    return 0;
}
