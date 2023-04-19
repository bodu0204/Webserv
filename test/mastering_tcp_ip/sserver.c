/* 単純なechoサーバー */
#include "common.h"
#include "server.h"
#include "echo.h"

static int s_server_session_id_context = 1;

int main(int argc, char *argv[])
{
    int sock, s;
    BIO *sbio;
    SSL_CTX *ctx;
    SSL *ssl;
    int r;
    
    /* SSLコンテキストを作成する */
    ctx = initialize_ctx(KEYFIKE, PASSWORD);
    load_dh_params(ctx, DHFILE);

    SSL_CTX_set_session_id_context(ctx, (void *)&s_server_session_id_context, sizeof(s_server_session_id_context));

    sock = tcp_listen();

    while (1)
    {
        s = accept(sock, 0, 0);
        if (s < 0)
            err_exit("Problem accepting");
        
        sbio = BIO_new_socket(s, BIO_NOCLOSE);
        ssl = ssl_new(ctx);
        SSL_set_bio(ssl, sbio, sbio);
        r = SSL_accept(ssl);

        if (r <= 0)
            berr_exit("SSL accept error");
        
        echo(ssl, s);
    }
    destroy_ctx(ctx);
    return (0);
}
