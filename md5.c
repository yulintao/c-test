#include <openssl/md5.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
    MD5_CTX ctx;
    unsigned char outmd[16]={0};
    int i=0;
    unsigned char *String = "hello\n";
    printf("data=%s\n",String);

    MD5_Init(&ctx); MD5_Update(&ctx,"hel",3);
    MD5_Update(&ctx,"lo\n",3);
    MD5_Final(outmd,&ctx);

    for(i=0;i<16;i++)
	printf("%02x",outmd[i]);
    printf("\n");
    return 0;
}
