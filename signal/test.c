#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <unistd.h>

int main(void){
    int ofd;
    ssize_t cc;
    char buf[1024];

    while((cc = read(STDIN_FILENO, buf, sizeof(buf))) > 0)
	write(STDOUT_FILENO, buf, cc);
}
