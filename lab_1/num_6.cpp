#include <stdio.h>
#include <stddef.h>
#include <type_traits>

static const size_t N = 5;

static void clear_stdin_line()
{
    int c;
    do { c = getchar(); } while (c != '\n' && c != EOF);
}

static void print_binary_ull(unsigned long long v)
{
    const size_t bits = 8u * sizeof(unsigned long long);
    for (size_t i = 0; i < bits; ++i) {
        size_t shift = (bits - 1u) - i;
        putchar(((v >> shift) & 1ULL) ? '1' : '0');
    }
}

static void print_Mq_all(const unsigned long long* a)
{
    puts("Mq (HEX X, with leading zeros):");
    for (size_t i = 0; i < N; ++i) {
        if (i) printf(" ");
        printf("%016llX", a[i]); /* 2*sizeof(ull)=16 hex digits */
    }
    printf("\n");

    puts("Mq (BIN b, with leading zeros):");
    for (size_t i = 0; i < N; ++i) {
        if (i) printf(" ");
        print_binary_ull(a[i]);
    }
    printf("\n");

    puts("Mq (DEC u):");
    for (size_t i = 0; i < N; ++i) {
        if (i) printf(" ");
        printf("%llu", a[i]);
    }
    printf("\n");

    puts("Mq (DEC d with +):");
    for (size_t i = 0; i < N; ++i) {
        if (i) printf(" ");
        printf("%+lld", (long long)a[i]);
    }
    printf("\n");
}

int main()
{
    const unsigned long long xq = 0x000D15A550C1A7EDULL;
    unsigned long long Mq[N] = { xq, xq, xq, xq, xq };

    /* Копия для подсчёта изменившихся элементов */
    unsigned long long before[N];
    for (size_t i = 0; i < N; ++i) before[i] = Mq[i];

    puts("=== BEFORE INPUT ===");
    print_Mq_all(Mq);

    puts("\nEnter 5 HEX values for Mq[0..4] (no 0x), separated by spaces:");
    /* ВАЖНО: один вызов scanf и адреса элементов массива */
    int k = scanf("%llx %llx %llx %llx %llx",
                  &Mq[0], &Mq[1], &Mq[2], &Mq[3], &Mq[4]);

    if (k != (int)N) {
        int entered = (k < 0) ? 0 : k;
        int not_entered = (int)N - entered;
        printf("Input error: entered %d element(s), not entered %d element(s).\n",
               entered, not_entered);
        clear_stdin_line();
    } else {
        clear_stdin_line();
        puts("Input OK: all 5 elements were read.");
    }

    puts("\n=== AFTER INPUT ===");
    print_Mq_all(Mq);

    /* Сколько реально изменилось */
    {
        size_t changed = 0;
        for (size_t i = 0; i < N; ++i) {
            if (Mq[i] != before[i]) ++changed;
        }
        printf("\nChanged elements: %zu of %zu\n", changed, N);
    }

    return 0;
}
