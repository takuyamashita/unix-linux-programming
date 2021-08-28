#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void){
    int fd;

    if((fd = open("input.txt", O_RDONLY)) == -1){
	perror("open");
	exit(1);
    }
    printf("fd = %d\n", fd);
    return 0;
}
