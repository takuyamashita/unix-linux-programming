#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>

enum {
    SERVER_PORT = 12345,
    NQUEUESIZE = 5,
    MAXNCLIENTS = 6,
};

int clients[MAXNCLIENTS];
int nclients = 0;

void sorry(int ws){
    char *message = "Sorry, it's full.\n";

    write(ws, message, strlen(message));
}

void delete_client(int ws){
    int i;
    for(i = 0; i < nclients; i++){
	if(clients[i] == ws){
	    clients[i] = clients[nclients - 1];
	    nclients--;
	    break;
	}
    }
}

void talks(int ws){
    int i, cc;
    char c;

    /* 1文字ずつ処理していく*/
    do {
	if((cc = read(ws, &c, 1)) == -1){
	    perror("read");
	    exit(1);
	} else if(cc == 0){
	    /* EOF */
	    shutdown(ws, SHUT_RDWR);
	    close(ws);
	    delete_client(ws);
	    fprintf(stderr, "Connection closed on descriptor %d.\n", ws);
	    return;
	}

	/* 全てのクライアントに送信する*/
	for(i = 0; i < nclients; i++)
	    write(clients[i], &c, 1);
	    
    } while(c != '\n');
}

int main(void){
    int s, soval;
    struct sockaddr_in sa;

    if((s = socket(AF_INET, SOCK_STREAM, 0)) == -1){
	perror("socket");
	exit(1);
    }

    soval = 1;
    if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &soval, sizeof(soval)) == -1){
	perror("setsockopt");
	exit(1);
    }

    memset(&sa, 0, sizeof(sa));

    sa.sin_family = AF_INET;
    sa.sin_port = htons(SERVER_PORT);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(bind(s, (struct sockaddr *)&sa, sizeof(sa)) == -1){
	perror("bind");
	exit(1);
    }

    if(listen(s, NQUEUESIZE)){
	perror("listen");
	exit(1);
    }

    fprintf(stderr, "Ready.\n");

    for(;;){
	int i, maxfd;
	fd_set readfds;

	/* 全てのfd_setのbitを0にする
	 * 初期化
	 * */
	FD_ZERO(&readfds);
	
	/* readfdsにsを追加する */
	FD_SET(s, &readfds);

	maxfd = s;

	/* 最初はnclientsは0 */
	for(i = 0; i < nclients; i++){
	    /* clients[i]をreadfdsに追加する */
	    FD_SET(clients[i], &readfds);

	    /* selectで最大のディスクリプタを使用するため */
	    if(clients[i] > maxfd)
		maxfd = clients[i];
	}

	/* readfdsの中のどれかが読み込み可能になるまで待つ
	 * 1ループ目は受付用のsしか入ってない
	 * */
	if(select(maxfd + 1, &readfds, NULL, NULL, NULL) < 0){
	    perror("select");
	    exit(1);
	}

	/* 受付用に変化があれば(新規接続)*/
	if(FD_ISSET(s, &readfds)){
	    struct sockaddr_in ca;
	    socklen_t ca_len;
	    int ws;

	    ca_len = sizeof(ca);

	    if((ws = accept(s, (struct sockaddr *)&ca, &ca_len)) == -1){
		perror("accept");
		continue;
	    }

	    if(nclients >= MAXNCLIENTS){
		/* 満杯 */
		sorry(ws);
		shutdown(ws, SHUT_RDWR);
		close(ws);
		fprintf(stderr, "Refused a new connection.\n");
	    } else {
		/* clientに追加する
		 * 次のloopでreadfdsにセットされselectで監視される
		 * */
		clients[nclients] = ws;
		nclients ++;
		fprintf(stderr, "Accepted a connection on description %d.\n", ws);
	    }
	}

	for(i = 0;i < nclients; i++){
	    if(FD_ISSET(clients[i], &readfds)){
		/* 変化のあった(読み取り可能状態=発信元)クライアントの発信を
		 * 全クライアントに送信
		 * */
		talks(clients[i]);
		break;
	    }
	}
    }

}


