#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>

#include <openssl/ssl.h>

#define CA_LIST "root.pem"
#define HOST "localhost"
#define RANDOM "random.pem"
#define PORT 4433
#define BUFSIZZ 1024

extern BIO *bio_err;
int berr_exit(char *);
int err_exit(char *);

SSL_CTX *initialize_ctx(char *, char *);
void destroy_ctx(SSL_CTX *);
