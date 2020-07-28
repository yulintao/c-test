#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#if 0
#define container_of(ptr, type, member) ({			\
	const typeof(((type *)0)->member) * __mptr = (ptr);	\
	(type *)((char *)__mptr - offsetof(type, member)); })
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
struct a{
	int a;
	int c;
};

int main(void)
{
	struct a abc;
	//container_of(hash, struct nf_conn,tuplehash[hash->tuple.dst.dir]);
	//const typeof(((type *)0)->member) * __mptr = (ptr);	\
	//(type *)((char *)__mptr - offsetof(type, member)); 
        typeof(((struct a *)0)->c) *mptr = &abc.c;
        printf("mptr:%p %p abc.c:%p abc:%p,%p\n",
		&mptr,mptr,&abc.c,&abc,(char *)mptr - ((size_t)&((struct a*)0)->c));
	
}

#endif

#if 1
#define container_of(ptr, type, member) \
    (type *)((char *)(ptr) - (char *) &((type *)0)->member)
#else

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({			\
	const typeof(((type *)0)->member) * __mptr = (ptr);	\
	(type *)((char *)__mptr - offsetof(type, member)); })
#endif

struct aaa{

    char buf[100];
    int b;
    void *key;
};

void
mmalloc(void *key )
{
    struct aaa ac, *bc;


    ac.key = key;
    bc = container_of(ac.key, struct aaa, b);
    printf("bc:%d %p\n", bc->b, bc);


}

int main(void)
{
    int **base;
    struct aaa *bbb, *ac;

    ac = malloc(sizeof(*ac));
    printf("ac:%p\n",ac);

    memcpy(ac->buf, "adafasdfasdf", 10);

    ac->b = 1000;
    ac->key = malloc(100);
    printf("ac->key:%p\n",ac->key);

    memcpy(ac->key, "yes,find!", 9);

    bbb = container_of(&ac->b, struct aaa, b);
    printf("bbb:%d %p\n", bbb->b, bbb);
    mmalloc(&ac->b);

}
