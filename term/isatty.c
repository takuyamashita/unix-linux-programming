#include <unistd.h>

int main(void){
    return isatty(STDOUT_FILENO);
}
