#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
    char cwd[PATH_MAX + 1];

    if(chdir("..") == -1){
	perror("chdir");
	exit(1);
    }

    if(getcwd(cwd, sizeof(cwd)) == NULL){
	perror("getcwd");
	exit(1);
    }

    printf("%s\n", cwd);

    return 0;
}

