#include <stdio.h>

struct test{
    int a;
    int b;
};
void ch(struct test *tp)
{
    tp->a = 5;
    tp->b = 6;
}

int main()
{
    struct test t;
    t.a = 0;
    t.b = 0;
    printf("a=%d b=%d\n",t.a,t.b);
    ch(&t);
    printf("a=%d b=%d\n",t.a,t.b);
}