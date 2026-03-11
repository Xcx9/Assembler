#include <stdio.h>
#include <stddef.h>

void viewPointer(void *p)
{
    char *p1 = reinterpret_cast<char *>(p);
    unsigned short *p2 = reinterpret_cast<unsigned short *>(p);
    double *p3 = reinterpret_cast<double *>(p);

    printf("p  = %p\n", p);
    printf("p1 = %p\n", (void *)p1);
    printf("p2 = %p\n", (void *)p2);
    printf("p3 = %p\n", (void *)p3);

    printf("p1 + 1 = %p\n", (void *)(p1 + 1));
    printf("p2 + 1 = %p\n", (void *)(p2 + 1));
    printf("p3 + 1 = %p\n", (void *)(p3 + 1));

    printf("diff(p1, p1+1) = %td byte(s), sizeof(char) = %zu\n",
           (ptrdiff_t)((char *)(p1 + 1) - (char *)p1), sizeof(char));

    printf("diff(p2, p2+1) = %td byte(s), sizeof(unsigned short) = %zu\n",
           (ptrdiff_t)((char *)(p2 + 1) - (char *)p2), sizeof(unsigned short));

    printf("diff(p3, p3+1) = %td byte(s), sizeof(double) = %zu\n",
           (ptrdiff_t)((char *)(p3 + 1) - (char *)p3), sizeof(double));

#if defined(__GNUC__) && !defined(__STRICT_ANSI__)
    void *pv1 = p + 1;
    printf("p + 1  = %p\n", pv1);
    printf("diff(p, p+1) = %td byte(s)\n",
           (ptrdiff_t)((char *)pv1 - (char *)p));
#else
    puts("p + 1 for void* is not available under current compiler settings.");
#endif
}

void printPointer(void *p)
{
    char *p1 = reinterpret_cast<char *>(p);
    unsigned short *p2 = reinterpret_cast<unsigned short *>(p);
    double *p3 = reinterpret_cast<double *>(p);

    printf("*p1       = 0x%02X\n", (unsigned char)(*p1));
    printf("*(p1 + 1) = 0x%02X\n", (unsigned char)(*(p1 + 1)));

    printf("*p2       = 0x%04hX\n", *p2);
    printf("*(p2 + 1) = 0x%04hX\n", *(p2 + 1));

    printf("*p3       = %A\n", *p3);
    printf("*(p3 + 1) = %A\n", *(p3 + 1));
}

int main()
{
    long long xbuf[2] = { 0x1122334455667788LL, 0LL };
    char s[] = "0123456789abcdef";

    puts("=== Test 1: long long value 0x1122334455667788 ===");
    viewPointer(&xbuf[0]);
    printPointer(&xbuf[0]);

    puts("");

    puts("=== Test 2: char s[] = \"0123456789abcdef\" ===");
    viewPointer(s);
    printPointer(s);

    puts("");
    puts("Can void* be dereferenced directly?");
    puts("No. In C/C++, *p for void* is not allowed because void has no size.");
    puts("First you must cast void* to a typed pointer.");

    return 0;
}
