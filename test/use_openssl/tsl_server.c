#include <openssl/ssl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define CERT_FILE "server.crt"
#define KEY_FILE "server.key"

int create_socket(int port)
{
    int sockfd;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        close(sockfd);
        return -1;
    }

    listen(sockfd, 5);

    return sockfd;
}

SSL_CTX *create_context()
{
    SSL_CTX *ctx;

    SSL_library_init();

    ctx = SSL_CTX_new(TLS_server_method());
    if (!ctx) {
        return NULL;
    }

    SSL_CTX_set_ecdh_auto(ctx, 1);

    if (SSL_CTX_use_certificate_file(ctx, CERT_FILE, SSL_FILETYPE_PEM) <= 0) {
        SSL_CTX_free(ctx);
        return NULL;
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, KEY_FILE, SSL_FILETYPE_PEM) <= 0 ) {
        SSL_CTX_free(ctx);
        return NULL;
    }

    return ctx;
}

void handle_request(SSL *ssl)
{
    const char response[] = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";

    SSL_write(ssl, response, sizeof(response));
}

void handle_connections(int sockfd, SSL_CTX *ctx)
{
    SSL *ssl;

    while (1) {
        int newsockfd = accept(sockfd, NULL, NULL);
        if (newsockfd < 0) {
            continue;
        }

        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, newsockfd);
        if (SSL_accept(ssl) <= 0) {
            close(newsockfd);
            SSL_free(ssl);
            continue;
        }

        handle_request(ssl);
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(newsockfd);
    }
}

int main(int argc, char *argv[])
{
    SSL_CTX *ctx;
    int sockfd;

    if (argc != 2)
        return (1 + 0 * fprintf(stderr, "Usage: %s <port>\n", argv[0]));

    ctx = create_context();
    if (!ctx) {
        return -1;
    }

    sockfd = create_socket(atoi(argv[1]));
    if (sockfd < 0) {
        SSL_CTX_free(ctx);
        return -1;
    }

    handle_connections(sockfd, ctx);

    close(sockfd);
    SSL_CTX_free(ctx);

    return 0;
}
