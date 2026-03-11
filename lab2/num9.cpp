#include <stdio.h>

void print16(void *p)
{
    unsigned short ux = *reinterpret_cast<unsigned short *>(p);
    short sx = *reinterpret_cast<short *>(p);

    printf("%04hX ", ux);

    for (int i = 15; i >= 0; --i) {
        putchar((ux & (1u << i)) ? '1' : '0');
    }

    printf(" %5hu %+6hd", ux, sx);
}

void ab16(void *p)
{
    unsigned short ux = *reinterpret_cast<unsigned short *>(p);
    short sx = *reinterpret_cast<short *>(p);

    puts("x:");
    print16(p);
    putchar('\n');

    {
        unsigned short a = static_cast<unsigned short>(ux * (unsigned short)2);
        unsigned short b = static_cast<unsigned short>(ux << 1);

        puts("a1(x): unsigned multiply by 2    | b1(x): unsigned shift left by 1");
        print16(&a);
        printf("    |    ");
        print16(&b);
        putchar('\n');
    }

    {
        short a = static_cast<short>(sx * (short)2);
        short b = static_cast<short>(sx << 1);

        puts("a2(x): signed multiply by 2      | b2(x): signed shift left by 1");
        print16(&a);
        printf("    |    ");
        print16(&b);
        putchar('\n');
    }

    {
        unsigned short a = static_cast<unsigned short>(ux / (unsigned short)2);
        unsigned short b = static_cast<unsigned short>(ux >> 1);

        puts("a3(x): unsigned divide by 2      | b3(x): unsigned shift right by 1");
        print16(&a);
        printf("    |    ");
        print16(&b);
        putchar('\n');
    }

    {
        short a = static_cast<short>(sx / (short)2);
        short b = static_cast<short>(sx >> 1);

        puts("a4(x): signed divide by 2        | b4(x): signed shift right by 1");
        print16(&a);
        printf("    |    ");
        print16(&b);
        putchar('\n');
    }

    {
        unsigned short a = static_cast<unsigned short>(ux % (unsigned short)16);
        unsigned short b = static_cast<unsigned short>(ux & (unsigned short)15);

        puts("a5(x): unsigned remainder /16    | b5(x): x & 15");
        print16(&a);
        printf("    |    ");
        print16(&b);
        putchar('\n');
    }

    {
        unsigned short a = static_cast<unsigned short>((ux / (unsigned short)16) * (unsigned short)16);
        unsigned short b = static_cast<unsigned short>(ux & (unsigned short)-16);

        puts("a6(x): floor to multiple of 16   | b6(x): x & -16");
        print16(&a);
        printf("    |    ");
        print16(&b);
        putchar('\n');
    }
}

int main()
{
    short m = 21;
    short n = -37;

    puts("m:");
    ab16(&m);
    puts("");

    puts("n:");
    ab16(&n);
    puts("");

    puts("Total number of operations described in task L2.9: 12.");

    return 0;
}
