#include "common.h"

BIO *bio_err;
static char *pass;
static int password_cb(char *, int, int, void *);
static int sigpipe_handle(int);

/* 単純なエラーと終了ルーチン */
int err_exit(char *string)
{
    fprintf(stderr, "%s\n", string);
    exit(0);
}

/* SSLエラーを表示して終了する */
int berr_exit(char *string)
{
    BIO_printf(bio_err, "%s\n", string);
    ERR_print_erros(bio_err);/*  */
    exit(0);
}

/* パスワードコードはスレッドセーフではない */
static int password_cb(char *buf, int num, int rflag, void *userdata)
{
    if (num < strlen(pass) + 1)
        return (0);
    strcpy(buf, pass);
    return (strlen(pass));
}

static int sigpipe_handle(int x){(void) x; return (0);}

SSL_CTX *initialize_ctx(char *keyfile, char *password)
{
    SSL_METHOD *meth;
    SSL_CTX *ctx;

    if (!bio_err)
    {
        /* グローバルなシステムの初期化 */
        SSL_library_init();
        SSL_load_error_strings();
        /* エラー書き込みコンテキスト */
        bio_err = BIO_new_fp(stderr, BIO_NOCLOSE);
    }

    /* SIGPIPEハンドラーをセットアップする */
    signal(SIGPIPE, sigpipe_handle);

    /* コンテキストを作成する */
    meth = SSLv3_method();
    ctx = SSL_CTX_new(meth);

    /* 鍵と証明書をロードする */
    if (!(SSL_CTX_use_certificate_file(ctx, keyfile, SSL_FILETYPE_PEM)))
        berr_exit("Couldn't read certificate file");
    pass = password;
    SSL_CTX_set_default_passwd_cb(ctx, password_cb);
    if (!(SSL_CTX_use_PrivateKey_file(ctx, keyfile, SSL_FILETYPE_PEM)))
        berr_exit("Couldn't read key file");

    /* 信頼するCAをロードする */
    if(!(SSL_CTX_load_verify_locations(ctx, CA_LIST, 0)))
        berr_exit("Couldn't read CA list");
    SSL_CTX_set_verify_depth(ctx, 1);

    /* 乱数ファイルをロードする */
    if (!(RAND_load_file(RANDOM, 1024 * 1024)))/*  */
        berr_exit("Couldn't read randomness");

    return (ctx);
}

void destroy_ctx(SSL_CTX *ctx)
{
    SSL_CTX_free(ctx);
}

