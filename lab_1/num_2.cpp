#include <stdio.h>
#include <stddef.h>   // size_t, ptrdiff_t
#include <wchar.h>    // wchar_t

#define SHOW_SIZE(T)  printf("%-22s : %zu byte(s)\n", #T, (size_t)sizeof(T))

int main()
{
    puts("=== sizeof() for required types ===");

    SHOW_SIZE(void*);
    SHOW_SIZE(char);
    SHOW_SIZE(signed char);
    SHOW_SIZE(unsigned char);
    SHOW_SIZE(char*);
    SHOW_SIZE(wchar_t);
    SHOW_SIZE(wchar_t*);
    SHOW_SIZE(short);
    SHOW_SIZE(unsigned short);
    SHOW_SIZE(short*);
    SHOW_SIZE(int);
    SHOW_SIZE(unsigned int);
    SHOW_SIZE(int*);
    SHOW_SIZE(long);
    SHOW_SIZE(unsigned long);
    SHOW_SIZE(long long);
    SHOW_SIZE(unsigned long long);
    SHOW_SIZE(float);
    SHOW_SIZE(double);
    SHOW_SIZE(long double);
    SHOW_SIZE(double*);
    SHOW_SIZE(size_t);
    SHOW_SIZE(ptrdiff_t);

    return 0;
}
