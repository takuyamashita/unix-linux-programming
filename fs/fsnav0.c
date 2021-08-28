#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>

void restoreterm(void);
void showlist(void);
void showcwd(void);
void goup(void);
void godown(void);
void delfile(void);
void makedir(void);
void deldir(void);

struct termios termsave, termcur;

void restoreterm(void){
    tcsetattr(STDIN_FILENO, TCSANOW, &termsave);
}

void showlist(void){
    DIR *dp;
    struct dirent *dep;

    if((dp = opendir(".")) == NULL){
	fprintf(stderr, "cannot open directory\n");
	return;
    }

    errno = 0;

    while((dep = readdir(dp)) != NULL)
	printf("%s ", dep->d_name);

    if(errno == 0)
	putchar('\n');
    else
	perror("\nreaddir");

    closedir(dp);
}

void showcwd(void){
    char buf[PATH_MAX + 1];

    getcwd(buf, sizeof(buf));
    printf("%s\n", buf);
}

void goup(void){
    chdir("..");
}

void godown(void){
    char buf[PATH_MAX + 1], *p;

    tcsetattr(STDIN_FILENO, TCSANOW, &termsave);

    fputs("directory? ", stderr);
    fgets(buf, sizeof(buf), stdin);

    if((p = strchr(buf, '\n')) != NULL)
	*p = '\0';
    
    if(chdir(buf) != 0)
	perror("chdir");

    tcsetattr(STDIN_FILENO, TCSANOW, &termcur);
}

void delfile(void){
    char buf[PATH_MAX + 1], *p;

    tcsetattr(STDIN_FILENO, TCSANOW, &termsave);

    fputs("file to remove? ", stderr);
    fgets(buf, sizeof(buf), stdin);

    if((p = strchr(buf, '\n')) != NULL)
	*p = '\0';

    if(unlink(buf) != 0)
	perror("unlink");

    tcsetattr(STDIN_FILENO, TCSANOW, &termcur);
}

void makedir(void){
    char buf[PATH_MAX + 1], *p;
    
    tcsetattr(STDIN_FILENO, TCSANOW, &termsave);
    
    fputs("new dirctory name? ", stderr);
    fgets(buf, sizeof(buf), stdin);

    if((p = strchr(buf, '\n')) != NULL)
	*p = '\0';

    if(mkdir(buf, 0777) != 0)
	perror("mkdir");

    tcsetattr(STDIN_FILENO, TCSANOW, &termcur);
}

void deldir(void){
    char buf[PATH_MAX + 1], *p;

    tcsetattr(STDIN_FILENO, TCSANOW, &termsave);

    fputs("directory to remove? ", stderr);
    fgets(buf, sizeof(buf), stdin);

    if((p = strchr(buf, '\n')) != NULL)
	*p = '\0';

    if(rmdir(buf) != 0)
	perror("rmdir");

    tcsetattr(STDIN_FILENO, TCSANOW, &termcur);
}

int main(void){
    int c;
    tcgetattr(STDIN_FILENO, &termsave);
    termcur = termsave;

    if(atexit(restoreterm) != 0){
	perror("atexit");
	exit(1);
    }

    termcur.c_lflag &= ~(ICANON | ECHO);
    termcur.c_cc[VMIN] = 1;
    termcur.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &termcur);
    setbuf(stdin, NULL);

    for(;;){
	c = getchar();

	switch(c){
	    case 'l': 
		showlist();
		break;
	    case 'u':
		goup();
		showcwd();
		break;
	    case 'd':
		godown();
		showcwd();
		break;
	    case '?':
		showcwd();
		break;
	    case 'q':
		exit(0);
	    case 'r':
		delfile();
		break;
	    case 'M':
		makedir();
		break;
	    case 'R':
		deldir();
		break;
	    default: fprintf(stderr, "unknown command '%c'\n", c);
		break;
	}
    }
}
