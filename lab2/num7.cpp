#include <stdio.h>
#include <wchar.h>

void print32(void *p)
{
    unsigned ux = *reinterpret_cast<unsigned *>(p);
    int sx = *reinterpret_cast<int *>(p);
    float fx = *reinterpret_cast<float *>(p);

    printf("%08X ", ux);

    for (int i = 31; i >= 0; --i) {
        putchar((ux & (1u << i)) ? '1' : '0');
    }

    printf(" %10u %+11d %+.2A %+.2e %+.2f\n", ux, sx, fx, fx, fx);
}

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

void printDump(void *p, size_t N)
{
    unsigned char *p1 = reinterpret_cast<unsigned char *>(p);

    for (size_t i = 0; i < N; ++i) {
        if (i) putchar(' ');
        printf("%02hhX", p1[i]);
    }
    putchar('\n');
}

int main()
{
    int x_int = 5;
    double x_double = 5.0;

    char s1[] = "abc012";
    char s2[] = "абв012";

    wchar_t ws1[] = L"abc012";
    wchar_t ws2[] = L"абв012";

    puts("=== int x = 5 ===");
    print32(&x_int);
    puts("dump:");
    printDump(&x_int, sizeof(x_int));

    if (*reinterpret_cast<unsigned char *>(&x_int) == 0x05)
        puts("Byte order in words on this platform: Little-Endian.");
    else
        puts("Byte order in words on this platform: Big-Endian.");

    puts("");
    puts("=== double x = 5.0 ===");
    print64(&x_double);
    puts("dump:");
    printDump(&x_double, sizeof(x_double));

    puts("");
    puts("=== char s1[] = \"abc012\" ===");
    puts("dump:");
    printDump(s1, sizeof(s1));

    puts("");
    puts("=== char s2[] = \"абв012\" ===");
    puts("dump:");
    printDump(s2, sizeof(s2));

    puts("");
    puts("=== wchar_t ws1[] = L\"abc012\" ===");
    puts("dump:");
    printDump(ws1, sizeof(ws1));

    puts("");
    puts("=== wchar_t ws2[] = L\"абв012\" ===");
    puts("dump:");
    printDump(ws2, sizeof(ws2));

    return 0;
}
