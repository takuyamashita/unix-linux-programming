#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

enum {
    SIZE = 1024 * 1024,
};

int main(void){
    int fd[2], nbytes;
    char buf[SIZE], buf2[SIZE];
    
    /* fd[1]が書き込み fd[0]が読み込み */
    pipe(fd);

    /* バッファを溢れさせる  */
    write(fd[1], buf2, sizeof(buf2));

    nbytes = read(fd[0], buf, sizeof(buf));

    close(fd[0]);
    close(fd[1]);

    return 0;
}
