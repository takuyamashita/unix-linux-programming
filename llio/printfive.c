#include <sys/types.h>	/* open, read */
#include <stdio.h>	/* perror, printf */
#include <stdlib.h>	/* exit */
#include <sys/stat.h>	/* open */
#include <fcntl.h>	/* open */
#include <sys/uio.h>	/* read */
#include <unistd.h>	/* close, read */

int main(void){
    int fd;
    char buf[6];

    if((fd = open("hello.txt", O_RDONLY)) == -1){
	perror("read");
	exit(1);
    }

    if(read(fd, buf, 5) != 5 ){
	perror("read");
	exit(1);
    }

    buf[5] = '\n';
    printf("%s\n", buf);

    if(close(fd) == -1){
	perror("close");
	exit(1);
    }

    return 0;
}
