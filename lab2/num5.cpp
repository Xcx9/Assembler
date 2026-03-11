#include <stdio.h>
#include <climits>
#include <cfloat>

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

int main()
{
    unsigned u_min = 0u;
    unsigned u_max = UINT_MAX;

    int s_min = INT_MIN;
    int s_max = INT_MAX;

    int x = 5;
    int y = -5;
    int a = 1;
    int b = 2;
    int c = 12345689;
    int d = 123456891;

    float fx = 5.0f;
    float fy = -5.0f;
    float fa = 1.0f;
    float fb = 2.0f;
    float fc = 12345689.0f;
    float fd = 123456891.0f;

    unsigned plus_zero_bits    = 0x00000000u;
    unsigned minus_zero_bits   = 0x80000000u;
    unsigned min_denorm_bits   = 0x00000001u;
    unsigned nan_bits          = 0x7FC00000u;
    unsigned min_norm_bits     = 0x00800000u;
    unsigned before_plus2_bits = 0x3FFFFFFFu;
    unsigned max_norm_bits     = 0x7F7FFFFFu;
    unsigned plus_inf_bits     = 0x7F800000u;

    puts("name              HEX      binary                               unsigned        signed  float-hex           float-exp           float-fix");

    printf("%-18s", "u_min");
    print32(&u_min);

    printf("%-18s", "u_max");
    print32(&u_max);

    printf("%-18s", "s_min");
    print32(&s_min);

    printf("%-18s", "s_max");
    print32(&s_max);

    printf("%-18s", "x (int)");
    print32(&x);

    printf("%-18s", "y (int)");
    print32(&y);

    printf("%-18s", "a (int)");
    print32(&a);

    printf("%-18s", "b (int)");
    print32(&b);

    printf("%-18s", "c (int)");
    print32(&c);

    printf("%-18s", "d (int)");
    print32(&d);

    puts("");

    printf("%-18s", "x (float)");
    print32(&fx);

    printf("%-18s", "y (float)");
    print32(&fy);

    printf("%-18s", "a (float)");
    print32(&fa);

    printf("%-18s", "b (float)");
    print32(&fb);

    printf("%-18s", "c (float)");
    print32(&fc);

    printf("%-18s", "d (float)");
    print32(&fd);

    puts("");

    printf("%-18s", "+0");
    print32(&plus_zero_bits);

    printf("%-18s", "-0");
    print32(&minus_zero_bits);

    printf("%-18s", "min denorm +");
    print32(&min_denorm_bits);

    printf("%-18s", "NaN");
    print32(&nan_bits);

    printf("%-18s", "min norm +");
    print32(&min_norm_bits);

    printf("%-18s", "prev before +2");
    print32(&before_plus2_bits);

    printf("%-18s", "max norm +");
    print32(&max_norm_bits);

    printf("%-18s", "+infinity");
    print32(&plus_inf_bits);

    return 0;
}
