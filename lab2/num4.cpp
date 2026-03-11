#include <stdio.h>
#include <climits>

void print16(void *p)
{
    unsigned short ux = *reinterpret_cast<unsigned short *>(p);
    short sx = *reinterpret_cast<short *>(p);

    printf("%04hX ", ux);

    for (int i = 15; i >= 0; --i) {
        putchar((ux & (1u << i)) ? '1' : '0');
    }

    printf(" %5hu %+6hd\n", ux, sx);
}

int main()
{
    unsigned short us_min = 0;
    unsigned short us_max = USHRT_MAX;

    short s_min = SHRT_MIN;
    short s_max = SHRT_MAX;

    short x = 5;
    short y = -5;
    short a = 1;
    short b = 2;

    puts("name     HEX  binary              unsigned signed");

    printf("%-7s", "us_min");
    print16(&us_min);

    printf("%-7s", "us_max");
    print16(&us_max);

    printf("%-7s", "s_min");
    print16(&s_min);

    printf("%-7s", "s_max");
    print16(&s_max);

    printf("%-7s", "x");
    print16(&x);

    printf("%-7s", "y");
    print16(&y);

    printf("%-7s", "a");
    print16(&a);

    printf("%-7s", "b");
    print16(&b);

    return 0;
}
