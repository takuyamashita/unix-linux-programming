#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

void listdir(DIR *, int);
void showdir(char *, int);

void listdir(DIR *dp, int lev){
    /* inodeの中身 */
    struct stat sb;
    
    /* ディレクトリの1つの項目 */
    struct dirent *dep;

    int i, errflag;
    int ndir, nreg, nchr, nblk, nfifo, nsock, nlnk, nunknown;

    errno = 0;
    ndir = nreg = nchr = nblk = nfifo = nsock = nlnk = nunknown = 0;

    /* ディレクトリの中身を1項目ずつdepに入れる */
    while((dep = readdir(dp)) != NULL){
	/* inodeの中身をsbに入れる */
	if(lstat(dep->d_name, &sb) != 0)
	    continue;
	
	switch (sb.st_mode & S_IFMT){
	    case S_IFDIR:
		ndir++;
		break;
	    case S_IFREG:
		nreg++;
		break;
	    case S_IFCHR:
		nchr++;
		break;
	    case S_IFBLK:
		nblk++;
		break;
	    case S_IFIFO:
		nfifo++;
		break;
	    case S_IFSOCK:
		nsock++;
		break;
	    case S_IFLNK:
		nlnk++;
		break;
	    default:
		nunknown++;
		break;
	}  
    }

    /* .と..は対象外とする */
    ndir -= 2;
    
    /* エラーを覚えておく */
    errflag = errno ? 1 : 0;
    
    /* ファイルの種類を出力 */
    for(i = 0; i < ndir; i ++ ) putchar('/');
    for(i = 0; i < nreg; i ++ ) putchar('-');
    for(i = 0; i < nchr; i ++ ) putchar('%');
    for(i = 0; i < nblk; i ++ ) putchar('#');
    for(i = 0; i < nfifo; i ++ ) putchar('|');
    for(i = 0; i < nsock; i ++ ) putchar('=');
    for(i = 0; i < nlnk; i ++ ) putchar('@');
    for(i = 0; i < nunknown; i ++ ) putchar('?');
    putchar('\n');

    if(errflag) return;

    /* ディレクトリストリームのオフセットを先頭にする */
    rewinddir(dp);
    errno = 0;

    while((dep = readdir(dp)) != NULL){
	/* .と..はスキップ*/
	if(!strcmp(dep->d_name, ".")) continue;
	if(!strcmp(dep->d_name, "..")) continue;

	if(lstat(dep->d_name, &sb) != 0)
	    continue;
	
	/* 対象がディレクトリなら再帰 */
	if((sb.st_mode & S_IFMT) == S_IFDIR)
	    showdir(dep->d_name, lev + 1);
    }

}

void showdir(char *dir, int lev){
    DIR *dp;
    char prevdir[PATH_MAX + 1];
    int i;

    /* プロセスの現在のディレクトリが取得できない場合 
     * unlinkされていたり、パーミッションエラー*/
    if(getcwd(prevdir, sizeof(prevdir)) == NULL){
	perror("getcwd");
	return;
    }
    
    /* ディレクトリの深さからインテンドを入れる */
    for(i = 0; i < lev * 2; i++)
	putchar(' ');

    printf("%s: ", dir);

    if(chdir(dir) != 0){
	/* ディレクトリを移動 */
	printf("<cannot chdir>\n");
    } else if((dp = opendir(".")) == NULL){
	/* ディレクトリを開く */
	printf("<cannot list>\n");
    } else {
	/* 再帰 */
	listdir(dp, lev);
	closedir(dp);
    }

    if(chdir(prevdir) != 0){
	perror("chdir");
    }
}

int main(int argc, char *argv[]){
    if(argc != 2){
	fprintf(stderr, "usage: %s <directory>\n", argv[0]);
	exit(1);
    }

    showdir(argv[1], 0);

    return 0;
}

