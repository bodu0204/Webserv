#include <stdio.h> /* fprintf()に必要 */
#include <netdb.h> /* gethostbyname()に必要 */
#include <stdlib.h>
unsigned long ResolveName(char name[]); 

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    fprintf(stderr, "ARG ERROR\n");
    return 1;
  }
  printf("%lx\n",ResolveName(argv[1]));
  return 0;
} 

unsigned long ResolveName(char name[])
{
  struct hostent *host; /* ホスト情報を格納する構造体 */

  if ((host = gethostbyname(name)) == NULL)
  {
    fprintf(stderr, "gethostbyname() failed");
    exit(1);
  }

  /* ネットワークバイト順で表したバイナリのIPアドレスを返す */
  return *((unsigned long *)host->h_addr_list[0]);
}
