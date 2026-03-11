#include <stdio.h>

void print64(void *p)
{
    unsigned long long ux = *reinterpret_cast<unsigned long long *>(p);
    long long sx = *reinterpret_cast<long long *>(p);
    double dx = *reinterpret_cast<double *>(p);

    printf("%016llX ", ux);

    for (int i = 63; i >= 0; --i) {
        putchar((ux & (1ULL << i)) ? '1' : '0');
    }
    putchar('\n');

    printf("                 %20llu %+21lld %+20.2A %+20.2e %+20.2f\n",
           ux, sx, dx, dx, dx);
}

int main()
{
    unsigned long long ex1 = 13ULL;
    unsigned long long ex2 = 0x8000000000000000ULL;

    long long x = 5LL;
    long long a = 1LL;

    double fx = 5.0;
    double fa = 1.0;

    puts("13:");
    print64(&ex1);
    puts("");

    puts("0x8000000000000000:");
    print64(&ex2);
    puts("");

    puts("x (long long = 5):");
    print64(&x);
    puts("");

    puts("a (long long = 1):");
    print64(&a);
    puts("");

    puts("x (double = 5.0):");
    print64(&fx);
    puts("");

    puts("a (double = 1.0):");
    print64(&fa);

    return 0;
}
