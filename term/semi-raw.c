#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int main(void){
    int c;
    struct termios new, old;
    tcgetattr(STDIN_FILENO, &old);
    new = old;

    /* ICANONとISIGを落とす */
    new.c_lflag &= ~(ICANON | ISIG);

    /* 最小で1byte, タイムアウトなし */
    new.c_cc[VMIN] = 1;
    new.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &new);
    
    while((c = getchar()) != EOF){
	if(c == 'q'){
	    tcsetattr(STDIN_FILENO, TCSANOW, &old);
	    exit(0);
	}
	putchar(c);
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &old);

}
