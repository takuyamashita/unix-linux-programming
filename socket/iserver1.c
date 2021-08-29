#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 12345

enum {
    NQUEUESIZE = 5,
};

void sendmessage(int s){
    char buf[1024];

    while(fgets(buf, sizeof(buf), stdin) != NULL){
	if(write(s, buf, strlen(buf)) == -1){
	    fprintf(stderr, "send error\n");
	    return;
	}
    }

    /* EOFを消す */
    clearerr(stdin);
}

char *message = "Hello!\nGood-bye!!\n";

int main(void){
    int s, ws, soval, cc;
    /* サーバとクライアントの2つのアドレス */
    struct sockaddr_in sa, ca;
    socklen_t ca_len;

    /* ソケット作成 */
    if((s = socket(AF_INET, SOCK_STREAM, 0)) == -1){
	perror("socket");
	exit(1);
    }

    /* アドレスを再利用する設定 */
    soval = 1;
    if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &soval, sizeof(soval)) == -1){
	perror("setsockopt");
	exit(1);
    }

    /* アドレス情報を作成 */
    memset(&sa, 0, sizeof(sa));
    /* sa.sin_len = sizeof(sa); */
    sa.sin_family = AF_INET;
    /* ネットワークのバイトオーダーに変換 */
    sa.sin_port = htons(SERVER_PORT);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    /* アドレス情報をソケットにバインド */
    if(bind(s, (struct sockaddr *)&sa, sizeof(sa)) == -1){
	perror("bind");
	exit(1);
    }

    /* ソケットのqueueを準備して
     * 受付を開始する
     * 接続があればqueueに溜まる
     * */
    if(listen(s, NQUEUESIZE)){
	perror("listen");
	exit(1);
    }

    for(;;){
	fprintf(stderr, "Waitng for a connection...\n");
	ca_len = sizeof(ca);

	/* queueの中の1つを処理する
	 * 通信用のソケットをもう一つ作成して、そのソケットディスクリプタを返す
	 * クライアントのアドレスとその長さがセットされる
	 * */
	if((ws = accept(s, (struct sockaddr *)&ca, &ca_len)) == -1){
	    perror("accept");
	    exit(1);
	}

	fprintf(stderr, "Connection established.\n");

	fprintf(stderr, "Sending the message...\n");
	/* 通信用のディスクリプタ(ws)に書き込む  */
	/*if((cc = write(ws, message, strlen(message))) == -1){
	    perror("write");
	    exit(1);
	}*/
	sendmessage(ws);

	fprintf(stderr, "Message sent.\n");

	/* 読み書き書き終了 */
	if(shutdown(ws, SHUT_RDWR) == -1){
	    perror("shutdown");
	    exit(1);
	}
	
	if(close(ws) == -1){
	    perror("close");
	    exit(1);
	}
    }

}

