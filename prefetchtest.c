#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

#define ARRAY_LEN  ((64*1024/4))

int a[ARRAY_LEN] = {0};
int b[ARRAY_LEN] = {0};
int c[ARRAY_LEN] = {0};
int d[ARRAY_LEN] = {0};
int main()
{
    struct timeval start_time;
    struct timeval end_time;
    long long  cost_time = 0;
    int i = 0;
    int k = 0;
    int *p = NULL;
    long long sumA = 0;
    long long sumB = 0;
    long long sumC = 0;
    long long sumD = 0;

    for (i = 0; i < ARRAY_LEN; i++)
    {
        a[i] = i;
    }
    for (i = 0; i < ARRAY_LEN; i++)
    {
        b[i] = i;
    }
    for (i = 0; i < ARRAY_LEN; i++)
    {
        c[i] = i;
    }
        for (i = 0; i < ARRAY_LEN; i++)
    {
        d[i] = i;
    }
    (void)gettimeofday(&start_time, NULL);
    for (i = 0; i < ARRAY_LEN; i++)
    {
        sumA = sumA + a[i];
    }
    for (i = 0; i < ARRAY_LEN; i++)
    {
        sumB = sumB + b[i];
    }
    (void)gettimeofday(&end_time, NULL);
    cost_time = (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec  - start_time.tv_usec);
    printf("No prefethc cost time:%lld.\r\n", cost_time);

    (void)gettimeofday(&start_time, NULL);
    i = ARRAY_LEN;
    p = c;
    while(i > 0)
    {
        __builtin_prefetch(&p[0], 0, 3);
        __builtin_prefetch(&p[1], 0, 3);
        __builtin_prefetch(&p[2], 0, 3);
        __builtin_prefetch(&p[3], 0, 3);
        sumC = sumC + p[0];
        sumC = sumC + p[1];
        sumC = sumC + p[2];
        sumC = sumC + p[3];
        i -= 4;
        p += 4;

    }
    i = ARRAY_LEN;
    p = d;
    while(i > 0)
    {
        __builtin_prefetch(&p[0], 0, 3);
        __builtin_prefetch(&p[1], 0, 3);
        __builtin_prefetch(&p[2], 0, 3);
        __builtin_prefetch(&p[3], 0, 3);
        sumC = sumC + p[0];
        sumC = sumC + p[1];
        sumC = sumC + p[2];
        sumC = sumC + p[3];
        i -= 4;
        p += 4;
    }
    (void)gettimeofday(&end_time, NULL);
    cost_time = (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec  - start_time.tv_usec); 
    printf("prefethc cost time:%lld.\r\n", cost_time);

    return 0;
}