#include <errno.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(void) {
    DIR *dirp;
    struct dirent *p;

    if((dirp = opendir(".")) == NULL){
	perror("opendir");
	exit(1);
    }

    errno = 0;

    /* エラーが発生してもNULLを返す*/
    while((p = readdir(dirp)) != NULL){
	printf("%s\n", p->d_name);
    }

    if(errno != 0){
	perror("readdir");
	exit(1);
    }

    if(closedir(dirp) != 0){
	perror("closedir");
	exit(1);
    }

    return 0;
}
