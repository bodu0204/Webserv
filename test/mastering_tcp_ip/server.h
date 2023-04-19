#pragma onece

#include <openssl/ssl.h>

#define KEYFIKE "server.pem"
#define PASSWORD "password"
#define DHFILE "dh1024.pem"

int tcp_listen();
void load_dh_params(SSL_CTX *, char *);
void generate_eph_rsa_key(SSL_CTX *);
