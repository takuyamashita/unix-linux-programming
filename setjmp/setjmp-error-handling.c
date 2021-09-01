#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include "../sub/mysub.h"

static jmp_buf env;

void do_show_item(int num){
    if(num > 10){
	fprintf(stderr, "no such item: %d\n", num);
	/* num > 10がエラーの体 */
	longjmp(env, 1);
    }

    printf("item %d is ...\n", num);
}

void do_show(void){
    int item_no = getint("item number?");
    do_show_item(item_no);
}

void do_input(void){
    printf("input done\n");
}

void do_end(void){
    exit(0);
}

int main(void){
    int command;

    /* envにスタックポインタ等を保存 */
    if(setjmp(env) != 0){
	fprintf(stderr, "error recoverd\n");
    }

    while(1){
	command = getint("command(0 = show, 1=input, 2=end)? ");
	switch(command){
	    case 0:
		do_show();
		break;
	    case 1: 
		do_input();
		break;
	    case 2:
		do_end();
		break;
	    default:
		fprintf(stderr, "unknown command\n");
		break;
	}
    }

    return 0;
}
