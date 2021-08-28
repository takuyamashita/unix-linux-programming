#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main(void)
{
    if(rmdir("foo") == -1){
	int old_errno = errno;
	perror("rmdir");
	if(old_errno == ENOTEMPTY){
	    system("ls foo");
	}
	exit(1);
    }
}
