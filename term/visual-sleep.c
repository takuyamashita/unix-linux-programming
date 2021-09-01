#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int i = 0, n = atoi(argv[1]);
    char c[] = {'.', 'o', 'O'};
    for(i = 0; i < n; i++){
	printf("%c\033[D", c[i % 3]);
	fflush(stdout);
	sleep(1);
    }

    return 0;
}
