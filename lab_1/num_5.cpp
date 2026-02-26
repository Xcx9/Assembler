#include <stdio.h>
#include <stddef.h>  // ptrdiff_t

static const size_t N = 5;
static const size_t R = 2;

int main()
{
    /* Вариант 3: Mq (64-битные целые) */
    const unsigned long long xq = 0x000D15A550C1A7EDULL;
    unsigned long long Mq[N] = { xq, xq, xq, xq, xq };

    puts("=== Addresses for array Mq ===");
    printf("Mq (start)     = %p\n", (void*)Mq);
    printf("&Mq[0]         = %p\n", (void*)&Mq[0]);
    printf("&Mq[1]         = %p\n", (void*)&Mq[1]);

    puts("\n=== Compare address differences ===");
    printf("sizeof(Mq[0])  = %zu byte(s)\n", sizeof(Mq[0]));

    /* Разница адресов в байтах (удобно сравнить с sizeof) */
    ptrdiff_t d0 = (char*)&Mq[0] - (char*)Mq;
    ptrdiff_t d1 = (char*)&Mq[1] - (char*)&Mq[0];
    printf("&Mq[0] - Mq    = %td byte(s)\n", d0);
    printf("&Mq[1] - &Mq[0]= %td byte(s)\n", d1);

    puts("\nConclusion: elements of the array are stored consecutively in memory.");

    /* Статическая матрица MM[R][N] того же типа */
    unsigned long long MM[R][N] = { {0} };

    puts("\n=== Addresses for matrix MM[R][N] ===");
    printf("&MM[0][0] = %p\n", (void*)&MM[0][0]);
    printf("&MM[0][1] = %p\n", (void*)&MM[0][1]);
    printf("&MM[1][0] = %p\n", (void*)&MM[1][0]);
    printf("&MM[1][1] = %p\n", (void*)&MM[1][1]);

    puts("\nConclusion: matrix is stored in row-major order (rows go one after another).");

    /* Как воспроизвести структуру матрицы на динамическом массиве M:
       нужен один непрерывный блок R*N элементов того же типа */
    puts("\n=== Dynamic reproduction (flat array) ===");
    printf("Memory to allocate: R * N * sizeof(T) = %zu byte(s)\n",
           (size_t)(R * N * sizeof(unsigned long long)));

    puts("Index mapping for flat array M:");
    puts("idx = i * N + j   (where i=row, j=column)");

    /* Демонстрация формулы индекса (без выделения памяти) */
    {
        size_t i = 1, j = 1;
        size_t idx = i * N + j;
        printf("Example: i=%zu, j=%zu -> idx=%zu\n", i, j, idx);
    }

    return 0;
}
