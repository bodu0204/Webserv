#include "../debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

int create_socket(int port)
{
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind");
        close(sockfd);
        return -1;
    }

    if (listen(sockfd, 5) < 0) {
        perror("listen");
        close(sockfd);
        return -1;
    }

    return sockfd;
}

SSL_CTX *create_context(void)
{
    SSL_CTX *ctx;

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    ctx = SSL_CTX_new(TLS_server_method());
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        return NULL;
    }

    SSL_CTX_set_options(ctx, SSL_OP_SINGLE_DH_USE);
    SSL_CTX_set_ecdh_auto(ctx, 1);
    if (SSL_CTX_use_certificate_file(ctx, "localhost.crt", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(ctx);
        return NULL;
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, "localhost.key", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(ctx);
        return NULL;
    }
    return ctx;
}

void handle_client(SSL *ssl)
{
    char buffer[BUFFER_SIZE + 1];
    int nread;
    const char response[] = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";

    nread = SSL_read(ssl, buffer, BUFFER_SIZE);
    buffer[nread] = '\0';
Ts(buffer)
    SSL_write(ssl, response, sizeof(response));
T
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(SSL_get_fd(ssl));
}

typedef struct s_aclient
{
    SSL *ssl;
    int fd;
}aclient;

int main(int argc, char *argv[])
{
    int sockfd, maxfd, i, nready, newfd;
    fd_set readfds, allfds;
    struct sockaddr_in cliaddr;
    socklen_t clilen;
    SSL_CTX *ctx;
    SSL *ssl;
    char addr_str[INET_ADDRSTRLEN];
    aclient clients[MAX_CLIENTS];
    int num_clients = 0;

    if (argc != 2)
        return (1 + 0 * fprintf(stderr, "Usage: %s <port>\n", argv[0]));

    ctx = create_context();
    if (!ctx) {
        fprintf(stderr, "Failed to create SSL context\n");
        exit(EXIT_FAILURE);
    }

    sockfd= create_socket(atoi(argv[1]));
    if (sockfd < 0) {
        fprintf(stderr, "Failed to create socket\n");
        SSL_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }

    FD_ZERO(&allfds);
    FD_SET(sockfd, &allfds);
    maxfd = sockfd;

    while (1) {
        readfds = allfds;
        nready = select(maxfd + 1, &readfds, NULL, NULL, NULL);

        if (nready < 0) {
            perror("select");
            continue;
        }

        if (FD_ISSET(sockfd, &readfds)) {
            clilen = sizeof(cliaddr);
            newfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
            if (newfd < 0) {
                perror("accept");
                continue;
            }

            printf("New connection from %s:%d\n",
                   inet_ntop(AF_INET, &cliaddr.sin_addr, addr_str, INET_ADDRSTRLEN),
                   ntohs(cliaddr.sin_port));

            ssl = SSL_new(ctx);
            SSL_set_fd(ssl, newfd);
            if (SSL_accept(ssl) <= 0) {
                ERR_print_errors_fp(stderr);
                close(newfd);
                SSL_free(ssl);
                continue;
            }

            if (num_clients >= MAX_CLIENTS) {
                printf("Too many clients\n");
                close(newfd);
                SSL_free(ssl);
                continue;
            }

            clients[num_clients].ssl = ssl;
            clients[num_clients++].fd = newfd;
            FD_SET(newfd, &allfds);
            if (newfd > maxfd) {
                maxfd = newfd;
            }

            printf("SSL connection using %s\n", SSL_get_cipher(ssl));
        }

        for (i = 0; i < num_clients; i++) {
            if (FD_ISSET(clients[i].fd, &readfds)) {
                ssl = clients[i].ssl;
                if (SSL_accept(ssl) <= 0) {//SSL_accept()は、TLS/SSLクライアントがTLS/SSLハンドシェイクを開始するのを待ちます。通信チャネルは、基礎となるBIOを設定することによって、すでに設定され、sslに割り当てられている必要があります。
                    ERR_print_errors_fp(stderr);
                    close(clients[i].fd);
                    SSL_free(ssl);
                    FD_CLR(clients[i].fd, &allfds);
                    clients[i].fd = -1;
                    continue;
                }
                handle_client(ssl);
                FD_CLR(clients[i].fd, &allfds);
                clients[i].fd = -1;
            }
        }

        for (i = 0; i < num_clients; i++) {
            if (clients[i].fd == -1) {
                clients[i] = clients[num_clients - 1];
                clients[num_clients - 1].fd = -1;
                num_clients--;
            }
        }
    }

    close(sockfd);
    SSL_CTX_free(ctx);

    return 0;
}


//このプログラムは、TLSサーバーを作成していますが、 `select()` システムコールを使用して、複数のクライアント接続を同時に処理するためにIO多重化を実装しています。

//`FD_ZERO()` および `FD_SET()` 関数を使用して、ソケットディスクリプタを `allfds` というセットに追加し、`select()` で使用する準備をします。`while` ループで、 `select()` を呼び出して、クライアントからの接続またはデータの受信を待ちます。接

//続いて、 select() が返されると、 FD_ISSET() を使用して、ソケットディスクリプタが準備完了の状態にあるかどうかを確認します。これにより、新しいクライアント接続が発生した場合に、 accept() を呼び出して、新しいクライアント接続を処理することができます。その後、新しい接続をセットに追加し、 SSL_accept() 関数を使用して、クライアントとのTLSハンドシェイクを完了します。

//接続済みのクライアントが読み込み可能な場合は、 FD_ISSET() を使用してクライアントのソケットディスクリプタが読み込み可能かどうかを確認します。読み込み可能である場合、SSL_accept() 関数を使用して、クライアントとのTLSハンドシェイクを完了し、クライアントからのデータを handle_client() 関数で処理します。クライアントのソケットディスクリプタが読み込み可能でない場合は、そのクライアントを削除します。

//最後に、 clients 配列から削除されたクライアントをクリーンアップするために、 for ループを使用して clients 配列をスキャンします。

//このプログラムは、複数のクライアント接続を処理する際にIO多重化を使用するため、シングルスレッドのプログラムよりも効率的である可能性があります。

//以上で、IO多重化されたTLSサーバのコードの説明は終了です.



//cc test/use_openssl/tsl_server_IO.c test/openssl-3.1.0/libssl.dylib
