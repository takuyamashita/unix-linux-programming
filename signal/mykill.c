#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char *argv[]){
    pid_t pid = atoi(argv[1]);
    kill(pid, SIGTERM);
}
