//version:2.1.0

#ifndef DEBUG_H
#define DEBUG_H
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#define TEST(msg) printf("[(%s/%d) %s ]%s\n", __FILE__, __LINE__, __func__, ((char *)msg)); fflush(stdin);
#define TEST_(msg) printf("---------------------------------------------[(%s/%d) %s ]%s\n", __FILE__, __LINE__, __func__, ((char *)msg)); fflush(stdin);
#define TESTn(msg, i) printf("[(%s/%d) %s ]%s:%ld\n", __FILE__, __LINE__, __func__, ((char *)msg), (long)(i)); fflush(stdin);
#define TESTu(msg, i) printf("[(%s/%d) %s ]%s:%lu\n", __FILE__, __LINE__, __func__, ((char *)msg), (unsigned long)(i)); fflush(stdin);
#define TESTx(msg, i) printf("[(%s/%d) %s ]%s:%lx\n", __FILE__, __LINE__, __func__, ((char *)msg), (unsigned long)(i)); fflush(stdin);
#define TESTd(msg, i) printf("[(%s/%d) %s ]%s:%lf\n", __FILE__, __LINE__, __func__, ((char *)msg), (double)(i)); fflush(stdin);
#define TESTp(msg, i) printf("[(%s/%d) %s ]%s:%p\n", __FILE__, __LINE__, __func__, ((char *)msg), (void *)(i)); fflush(stdin);
#define TESTs(msg, i) printf("[(%s/%d) %s ]%s:%s\n", __FILE__, __LINE__, __func__, ((char *)msg), (char *)(i)); fflush(stdin);
#define TESTc(msg, i) printf("[(%s/%d) %s ]%s:%c\n", __FILE__, __LINE__, __func__, ((char *)msg), (char)(i)); fflush(stdin);
#define T printf("[(%s/%d) %s ]\n", __FILE__, __LINE__, __func__); fflush(stdin);
#define T_ printf("---------------------------------------------[(%s/%d) %s ]\n", __FILE__, __LINE__, __func__); fflush(stdin);
#define Tn(i) printf("[(%s/%d) %s ]%s:%ld\n", __FILE__, __LINE__, __func__, #i, (long)(i)); fflush(stdin);
#define Tu(i) printf("[(%s/%d) %s ]%s:%lu\n", __FILE__, __LINE__, __func__, #i, (unsigned long)(i)); fflush(stdin);
#define Tx(i) printf("[(%s/%d) %s ]%s:%lx\n", __FILE__, __LINE__, __func__, #i, (unsigned long)(i)); fflush(stdin);
#define Td(i) printf("[(%s/%d) %s ]%s:%lf\n", __FILE__, __LINE__, __func__, #i, (double)(i)); fflush(stdin);
#define Tp(i) printf("[(%s/%d) %s ]%s:%p\n", __FILE__, __LINE__, __func__, #i, (void *)(i)); fflush(stdin);
#define Ts(i) printf("[(%s/%d) %s ]%s:%s\n", __FILE__, __LINE__, __func__, #i, (char *)(i)); fflush(stdin);
#define Tc(i) printf("[(%s/%d) %s ]%s:%c\n", __FILE__, __LINE__, __func__, #i, (char)(i)); fflush(stdin);
#define FORn(x,j) {printf("[(%s/%d) %s ]%s[]:", __FILE__, __LINE__, __func__, #x);for(size_t i = 0; j; i++){printf("%ld,", (long)(x[i])); fflush(stdin);}printf("\n");}
#define FORu(x,j) {printf("[(%s/%d) %s ]%s[]:", __FILE__, __LINE__, __func__, #x);for(size_t i = 0; j; i++){printf("%ld,", (unsigned long)(x[i])); fflush(stdin);}printf("\n");}
#define FORx(x,j) {printf("[(%s/%d) %s ]%s[]:", __FILE__, __LINE__, __func__, #x);for(size_t i = 0; j; i++){printf("%ld,", (unsigned long)(x[i])); fflush(stdin);}printf("\n");}
#define FORd(x,j) {printf("[(%s/%d) %s ]%s[]:", __FILE__, __LINE__, __func__, #x);for(size_t i = 0; j; i++){printf("%ld,", (double)(x[i])); fflush(stdin);}printf("\n");}
#define FORp(x,j) {printf("[(%s/%d) %s ]%s[]:", __FILE__, __LINE__, __func__, #x);for(size_t i = 0; j; i++){printf("%ld,", (void *)(x[i])); fflush(stdin);}printf("\n");}
#define FORs(x,j) {printf("[(%s/%d) %s ]%s[]:", __FILE__, __LINE__, __func__, #x);for(size_t i = 0; j; i++){printf("%ld,", (char *)(x[i])); fflush(stdin);}printf("\n");}
#define FORc(x,j) {printf("[(%s/%d) %s ]%s[]:", __FILE__, __LINE__, __func__, #x);for(size_t i = 0; j; i++){printf("%ld,", (char)(x[i])); fflush(stdin);}printf("\n");}
#define TAKE(i,j) {static size_t test_arg = 0; if(!(test_arg % j)){i} test_arg++;}
#define STOP {char c; read(STDIN_FILENO, &c, sizeof(char));}

#endif



/*ショートカット


#include "debug.h"

#ifndef _H 
#include ""
#endif





 */

/* 一定回数でプログラムを強制終了 */

//* test */static	size_t stc = 0;
//* test */const int stc_max = 10000;
//* test */const int onoff = 1;
//* test */stc++;
//* test */if (stc > stc_max && onoff)
//* test */{
//* test */	printf("the number is too big"); TEST
//* test */	exit(0);
//* test */}

/* 時間を計測 */
//* test */	double	time_diff(void)
//* test */	{
//* test */		static struct timespec	p = {0};
//* test */		struct timespec			n;
//* test */		unsigned int			sec;
//* test */		int						nsec;
//* test */
//* test */		if (!p.tv_sec)
//* test */		{
//* test */			clock_gettime(CLOCK_REALTIME, &p);
//* test */			return (0);
//* test */		}
//* test */		clock_gettime(CLOCK_REALTIME, &n);
//* test */		sec = n.tv_sec - p.tv_sec;
//* test */		nsec = n.tv_nsec - p.tv_nsec;
//* test */		p = n;
//* test */		return ((double)sec + (double)nsec / (1000 * 1000 * 1000));
//* test */	}
