#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/uio.h>
#include <unistd.h>

char *httpreq = "GET / HTTP/2.0\r\n\r\n";

int main(void){
    struct addrinfo hints, *addrs;
    int cc, s;
    char buf[1024];

    memset(&hints, 0, sizeof(hints));

    /* 指定しない(unspecified) */
    hints.ai_family = AF_UNSPEC;
    /* streamタイプを指定 */
    hints.ai_socktype = SOCK_STREAM;

    /* hints(条件) を満たすアドレス情報をセットする*/
    if((cc = getaddrinfo("www.google.com", "http", &hints, &addrs)) != 0){
	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(cc));
	exit(1);
    }

    /* セットされた情報を元にソケット作成や、接続を行う 
     * プロトコルに依存しないクライアント(通信)プログラム
     * */
    if((s = socket(addrs->ai_family, addrs->ai_socktype, addrs->ai_protocol)) == -1){
	perror("socket");
	exit(1);
    }

    if(connect(s, addrs->ai_addr, addrs->ai_addrlen) == -1){
	perror("connect");
	exit(1);
    }
    
    /* addrinfoの解放) */
    freeaddrinfo(addrs);

    write(s, httpreq, sizeof(httpreq));
    while((cc = read(s, buf, sizeof(buf))) > 0){
	write(1, buf, cc);
    }

    shutdown(s, SHUT_RDWR);
    close(s);

    return 0;
}
