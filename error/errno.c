#include <stdio.h>
#include <errno.h>

int main(void)
{
    errno = ENOENT;
    perror("test");
}
