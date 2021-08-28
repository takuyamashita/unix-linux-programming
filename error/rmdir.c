#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(void)
{
    if(rmdir("foo") == -1){
	perror("rmdir");
	exit(1);
    }
}
