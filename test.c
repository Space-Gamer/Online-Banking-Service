#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// struct test{
//     int a;
//     int b;
// };
// void ch(struct test *tp)
// {
//     tp->a = 5;
//     tp->b = 6;
// }

// int main()
// {
//     struct test t;
//     t.a = 0;
//     t.b = 0;
//     printf("a=%d b=%d\n",t.a,t.b);
//     ch(&t);
//     printf("a=%d b=%d\n",t.a,t.b);
// }

float float_rand( float min, float max )
{
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * ( max - min );      /* [min, max] */
}

int main()
{
    srand(time(0));
    // for (int i = 0; i < 10; i++)
    // {
    //     printf("%f\n", float_rand(-5, 5));
    // }
    float_rand(-5, 5);
    printf("%f\n", float_rand(-5, 5));
}