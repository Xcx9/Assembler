#include <stdio.h>

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

void c16to32(void *p)
{
    short s16 = *reinterpret_cast<short *>(p);
    unsigned short u16 = *reinterpret_cast<unsigned short *>(p);

    int s32 = static_cast<int>(s16);
    unsigned u32 = static_cast<unsigned>(u16);

    puts("16-bit source:");
    print16(p);

    puts("32-bit extension as signed (short -> int):");
    print32(&s32);

    puts("32-bit extension as unsigned (unsigned short -> unsigned int):");
    print32(&u32);
}

int main()
{
    short m = 21;
    short n = -37;

    puts("m:");
    c16to32(&m);
    puts("");

    puts("n:");
    c16to32(&n);

    return 0;
}
