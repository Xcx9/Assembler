#include <stdio.h>
#include <stddef.h>
#include <wchar.h>
#include <type_traits>

static const size_t N = 5;
static const size_t I = 2;
static const size_t R = 2;

/* ---------- Common helpers ---------- */

static void clear_stdin_line()
{
    int c;
    do { c = getchar(); } while (c != '\n' && c != EOF);
}

template <typename T>
static void print_binary_value(T v)
{
    const size_t bits = 8u * sizeof(T);
    unsigned long long u = (unsigned long long)(typename std::make_unsigned<T>::type)v;

    char s[65];
    size_t i = 0;
    for (; i < bits && i < sizeof(s) - 1; ++i) {
        size_t shift = (bits - 1u) - i;
        s[i] = ((u >> shift) & 1ULL) ? '1' : '0';
    }
    s[i] = '\0';
    printf("%s", s);
}

static void print_binary_ull(unsigned long long v)
{
    const size_t bits = 8u * sizeof(unsigned long long);
    for (size_t i = 0; i < bits; ++i) {
        size_t shift = (bits - 1u) - i;
        putchar(((v >> shift) & 1ULL) ? '1' : '0');
    }
}

template <typename T>
static void print_array_hex(const char* name, const T* a, size_t n, const char* mod)
{
    const int w = (int)(2u * sizeof(T));
    char fmt[32];

    if (mod[0] == '\0')
        snprintf(fmt, sizeof(fmt), "%%0%dX", w);
    else
        snprintf(fmt, sizeof(fmt), "%%0%d%sX", w, mod);

    printf("%s (HEX X): ", name);
    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");
        if constexpr (sizeof(T) <= sizeof(unsigned int))
            printf(fmt, (unsigned int)a[i]);
        else
            printf(fmt, (unsigned long long)a[i]);
    }
    printf("\n");
}

template <typename T>
static void print_array_bin(const char* name, const T* a, size_t n)
{
    printf("%s (BIN b): ", name);
    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");
        print_binary_value(a[i]);
    }
    printf("\n");
}

template <typename T>
static void print_array_unsigned(const char* name, const T* a, size_t n, const char* mod)
{
    char fmt[16];

    if (mod[0] == '\0')
        snprintf(fmt, sizeof(fmt), "%%u");
    else
        snprintf(fmt, sizeof(fmt), "%%%su", mod);

    printf("%s (DEC u): ", name);
    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");
        if constexpr (sizeof(T) <= sizeof(unsigned int))
            printf(fmt, (unsigned int)a[i]);
        else
            printf(fmt, (unsigned long long)a[i]);
    }
    printf("\n");
}

template <typename T>
static void print_array_signed_plus(const char* name, const T* a, size_t n, const char* mod)
{
    using S = typename std::make_signed<T>::type;
    char fmt[16];

    if (mod[0] == '\0')
        snprintf(fmt, sizeof(fmt), "%%+d");
    else
        snprintf(fmt, sizeof(fmt), "%%+%sd", mod);


    printf("%s (DEC d with +): ", name);
    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");
        S sv = (S)a[i];
        if constexpr (sizeof(T) <= sizeof(int))
            printf(fmt, (int)sv);
        else
            printf(fmt, (long long)sv);
    }
    printf("\n");
}

static void print_array_chars(const char* name, const unsigned char* a, size_t n)
{
    printf("%s (CHAR c): ", name);
    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");
        printf("%c", (int)a[i]);
    }
    printf("\n");
}

static void width_demo_unsigned_short(const unsigned short* a, size_t n)
{
    const int w = 10;
    puts("\n--- Width demo for Ms with format %hu ---");
    puts("w <= w0 : looks the same.");
    puts("w >  w0 : padded with spaces (right-aligned).");
    puts("with '-' : left-aligned.");

    printf("Ms right aligned: ");
    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");
        printf("%10hu", a[i]);
    }
    printf("\n");

    printf("Ms left  aligned: ");
    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");
        printf("%-10hu", a[i]);
    }
    printf("\n");
}

static void print_float_array_forms(const char* name, const float* a, size_t n)
{
    printf("%s (HEX exp A): ", name);
    for (size_t i = 0; i < n; ++i) { if (i) printf(" "); printf("%A", a[i]); }
    printf("\n");

    printf("%s (DEC exp e): ", name);
    for (size_t i = 0; i < n; ++i) { if (i) printf(" "); printf("%e", a[i]); }
    printf("\n");

    printf("%s (DEC f): ", name);
    for (size_t i = 0; i < n; ++i) { if (i) printf(" "); printf("%f", a[i]); }
    printf("\n");
}

static void print_double_array_forms_default(const char* name, const double* a, size_t n)
{
    printf("%s (HEX exp A): ", name);
    for (size_t i = 0; i < n; ++i) { if (i) printf(" "); printf("%A", a[i]); }
    printf("\n");

    printf("%s (DEC exp e): ", name);
    for (size_t i = 0; i < n; ++i) { if (i) printf(" "); printf("%e", a[i]); }
    printf("\n");

    printf("%s (DEC f): ", name);
    for (size_t i = 0; i < n; ++i) { if (i) printf(" "); printf("%f", a[i]); }
    printf("\n");
}

static void print_double_array_forms_prec2(const char* name, const double* a, size_t n)
{
    puts("\n--- Precision demo (.2) for Mfl (A/e/f) ---");

    printf("%s (HEX exp A, .2): ", name);
    for (size_t i = 0; i < n; ++i) { if (i) printf(" "); printf("%.2A", a[i]); }
    printf("\n");

    printf("%s (DEC exp e, .2): ", name);
    for (size_t i = 0; i < n; ++i) { if (i) printf(" "); printf("%.2e", a[i]); }
    printf("\n");

    printf("%s (DEC f, .2): ", name);
    for (size_t i = 0; i < n; ++i) { if (i) printf(" "); printf("%.2f", a[i]); }
    printf("\n");

    puts("Precision .2 changes digits after decimal point (rounding).");
}

/* For task 4 printing all integer arrays in all forms */
static void print_int_all_formats_Mb(const unsigned char* a)
{
    print_array_hex("Mb", a, N, "hh");
    print_array_bin("Mb", a, N);
    print_array_unsigned("Mb", a, N, "hh");
    print_array_signed_plus("Mb", a, N, "hh");
    print_array_chars("Mb", a, N);
}

static void print_int_all_formats_Ms(const unsigned short* a)
{
    print_array_hex("Ms", a, N, "h");
    print_array_bin("Ms", a, N);
    print_array_unsigned("Ms", a, N, "h");
    print_array_signed_plus("Ms", a, N, "h");
}

static void print_int_all_formats_Ml(const unsigned int* a)
{
    print_array_hex("Ml", a, N, "");
    print_array_bin("Ml", a, N);
    print_array_unsigned("Ml", a, N, "");
    print_array_signed_plus("Ml", a, N, "");
}

static void print_int_all_formats_Mq(const unsigned long long* a)
{
    print_array_hex("Mq", a, N, "ll");
    print_array_bin("Mq", a, N);
    print_array_unsigned("Mq", a, N, "ll");
    print_array_signed_plus("Mq", a, N, "ll");
}

/* Task 6 array output for Mq */
static void print_Mq_all(const unsigned long long* a)
{
    puts("Mq (HEX X, with leading zeros):");
    for (size_t i = 0; i < N; ++i) {
        if (i) printf(" ");
        printf("%016llX", a[i]);
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

/* ---------- Tasks ---------- */

static void task1()
{
    puts("Gruppa: 8\n"
         "Laboratornaya rabota №1\n"
         "Sostav komandy:\n"
         "Ivanov I.I.\n"
         "Petrov P.P.");
}

static void task2()
{
    puts("=== sizeof() for required types ===");
#define SHOW_SIZE(T)  printf("%-22s : %zu byte(s)\n", #T, (size_t)sizeof(T))
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
#undef SHOW_SIZE
}

static void task3()
{
    const unsigned char xb = 0xA7;
    const unsigned short xs = 0xC0DE;
    const unsigned int xl = 0xDEADBEEFu;
    const unsigned long long xq = 0x000D15A550C1A7EDULL;
    const float xfs = -3.0f / 7.0f;
    const double xfl = -3.0 / 7.0;

    unsigned char Mb[N] = { xb, xb, xb, xb, xb };
    unsigned short Ms[N] = { xs, xs, xs, xs, xs };
    unsigned int Ml[N] = { xl, xl, xl, xl, xl };
    unsigned long long Mq[N] = { xq, xq, xq, xq, xq };
    float Mfs[N] = { xfs, xfs, xfs, xfs, xfs };
    double Mfl[N] = { xfl, xfl, xfl, xfl, xfl };

    puts("=== Integer arrays (each printed 4 times: X, b, u, d) ===");
    print_array_hex("Mb", Mb, N, "hh");
    print_array_bin("Mb", Mb, N);
    print_array_unsigned("Mb", Mb, N, "hh");
    print_array_signed_plus("Mb", Mb, N, "hh");

    print_array_hex("Ms", Ms, N, "h");
    print_array_bin("Ms", Ms, N);
    print_array_unsigned("Ms", Ms, N, "h");
    print_array_signed_plus("Ms", Ms, N, "h");

    print_array_hex("Ml", Ml, N, "");
    print_array_bin("Ml", Ml, N);
    print_array_unsigned("Ml", Ml, N, "");
    print_array_signed_plus("Ml", Ml, N, "");

    print_array_hex("Mq", Mq, N, "ll");
    print_array_bin("Mq", Mq, N);
    print_array_unsigned("Mq", Mq, N, "ll");
    print_array_signed_plus("Mq", Mq, N, "ll");

    puts("\n=== Mb printed 5th time as characters (%c) ===");
    print_array_chars("Mb", Mb, N);

    width_demo_unsigned_short(Ms, N);

    puts("\n=== Floating point arrays (each printed 3 times: A, e, f) ===");
    print_float_array_forms("Mfs", Mfs, N);

    puts("\nMfl default:");
    print_double_array_forms_default("Mfl", Mfl, N);

    print_double_array_forms_prec2("Mfl", Mfl, N);

    puts("\nNote: Binary output is manual because %b is often not supported by MinGW printf.");
}

static void task4()
{
    const unsigned char xb = 0xA7;
    const unsigned short xs = 0xC0DE;
    const unsigned int xl = 0xDEADBEEFu;
    const unsigned long long xq = 0x000D15A550C1A7EDULL;
    const float xfs = -3.0f / 7.0f;
    const double xfl = -3.0 / 7.0;

    unsigned char Mb[N] = { xb, xb, xb, xb, xb };
    unsigned short Ms[N] = { xs, xs, xs, xs, xs };
    unsigned int Ml[N] = { xl, xl, xl, xl, xl };
    unsigned long long Mq[N] = { xq, xq, xq, xq, xq };
    float Mfs[N] = { xfs, xfs, xfs, xfs, xfs };
    double Mfl[N] = { xfl, xfl, xfl, xfl, xfl };


    puts("=== BEFORE INPUT ===");
    print_int_all_formats_Mb(Mb);
    print_int_all_formats_Ms(Ms);
    print_int_all_formats_Ml(Ml);
    print_int_all_formats_Mq(Mq);
    print_float_array_forms("Mfs", Mfs, N);
    print_double_array_forms_default("Mfl", Mfl, N);
    print_double_array_forms_prec2("Mfl", Mfl, N);
    width_demo_unsigned_short(Ms, N);

    puts("\n=== INPUT NEW VALUES INTO M[i], i=2 (NO TEMP VARIABLES) ===");

    printf("Enter new HEX value for Mb[2] (no 0x): ");
    if (scanf("%hhx", &Mb[I]) != 1) { puts("Input error for Mb[2]."); clear_stdin_line(); }
    else clear_stdin_line();

    printf("Enter new HEX value for Ms[2] (no 0x): ");
    if (scanf("%hx", &Ms[I]) != 1) { puts("Input error for Ms[2]."); clear_stdin_line(); }
    else clear_stdin_line();

    printf("Enter new HEX value for Ml[2] (no 0x): ");
    if (scanf("%x", &Ml[I]) != 1) { puts("Input error for Ml[2]."); clear_stdin_line(); }
    else clear_stdin_line();

    printf("Enter new HEX value for Mq[2] (no 0x): ");
    if (scanf("%llx", &Mq[I]) != 1) { puts("Input error for Mq[2]."); clear_stdin_line(); }
    else clear_stdin_line();

    printf("Enter new value for Mfs[2] (float): ");
    if (scanf("%f", &Mfs[I]) != 1) { puts("Input error for Mfs[2]."); clear_stdin_line(); }
    else clear_stdin_line();

    printf("Enter new value for Mfl[2] (double): ");
    if (scanf("%lf", &Mfl[I]) != 1) { puts("Input error for Mfl[2]."); clear_stdin_line(); }
    else clear_stdin_line();

    puts("\n=== AFTER INPUT ===");
    print_int_all_formats_Mb(Mb);
    print_int_all_formats_Ms(Ms);
    print_int_all_formats_Ml(Ml);
    print_int_all_formats_Mq(Mq);
    print_float_array_forms("Mfs", Mfs, N);
    print_double_array_forms_default("Mfl", Mfl, N);
    print_double_array_forms_prec2("Mfl", Mfl, N);

    puts("\nNote: Binary output is manual because %b is often not supported by MinGW printf.");
}

static void task5()
{
    const unsigned long long xq = 0x000D15A550C1A7EDULL;
    unsigned long long Mq[N] = { xq, xq, xq, xq, xq };

    puts("=== Addresses for array Mq ===");
    printf("Mq (start)     = %p\n", (void*)Mq);
    printf("&Mq[0]         = %p\n", (void*)&Mq[0]);
    printf("&Mq[1]         = %p\n", (void*)&Mq[1]);

    puts("\n=== Compare address differences ===");
    printf("sizeof(Mq[0])  = %zu byte(s)\n", sizeof(Mq[0]));

    ptrdiff_t d0 = (char*)&Mq[0] - (char*)Mq;
    ptrdiff_t d1 = (char*)&Mq[1] - (char*)&Mq[0];
    printf("&Mq[0] - Mq     = %td byte(s)\n", d0);
    printf("&Mq[1] - &Mq[0] = %td byte(s)\n", d1);

    puts("\nConclusion: elements of the array are stored consecutively in memory.");

    unsigned long long MM[R][N] = { {0} };

    puts("\n=== Addresses for matrix MM[R][N] ===");
    printf("&MM[0][0] = %p\n", (void*)&MM[0][0]);
    printf("&MM[0][1] = %p\n", (void*)&MM[0][1]);
    printf("&MM[1][0] = %p\n", (void*)&MM[1][0]);
    printf("&MM[1][1] = %p\n", (void*)&MM[1][1]);

    puts("\nConclusion: matrix is stored in row-major order (rows are contiguous).");

    puts("\n=== Dynamic reproduction (flat array) ===");
    printf("Memory to allocate: R * N * sizeof(T) = %zu byte(s)\n",
           (size_t)(R * N * sizeof(unsigned long long)));

    puts("Index mapping for flat array M:");
    puts("idx = i * N + j");

    {
        size_t i = 1, j = 1;
        size_t idx = i * N + j;
        printf("Example: i=%zu, j=%zu -> idx=%zu\n", i, j, idx);
    }
}

static void task6()
{
    const unsigned long long xq = 0x000D15A550C1A7EDULL;
    unsigned long long Mq[N] = { xq, xq, xq, xq, xq };

    unsigned long long before[N];
    for (size_t i = 0; i < N; ++i) before[i] = Mq[i];

    puts("=== BEFORE INPUT ===");
    print_Mq_all(Mq);

    puts("\nEnter 5 HEX values for Mq[0..4] (no 0x), separated by spaces:");
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

    {
        size_t changed = 0;
        for (size_t i = 0; i < N; ++i)
            if (Mq[i] != before[i]) ++changed;
        printf("\nChanged elements: %zu of %zu\n", changed, N);
    }
}

static void task7()
{
    char s1[64];
    char s2[16];
    char s3[128];

    puts("Enter s1 (one word, no spaces):");
    if (scanf("%s", s1) != 1) {
        puts("Input error for s1");
        return;
    }

    puts("Enter s2 (one word, max 15 chars):");
    if (scanf("%15s", s2) != 1) {
        puts("Input error for s2");
        return;
    }

    puts("Enter s3 (a whole line, may contain spaces):");
    if (scanf(" %127[^\n]", s3) != 1) {
        puts("Input error for s3");
        return;
    }

    printf("***%s***\n", s1);
    printf("***%s***\n", s2);
    printf("***%s***\n", s3);
}

/* ---------- Main menu ---------- */

int main()
{
    int n = 0;

    puts("Enter task number (1-7):");
    if (scanf("%d", &n) != 1) {
        puts("Input error.");
        return 0;
    }
    clear_stdin_line();

    switch (n) {
    case 1: task1(); break;
    case 2: task2(); break;
    case 3: task3(); break;
    case 4: task4(); break;
    case 5: task5(); break;
    case 6: task6(); break;
    case 7: task7(); break;
    default: puts("No such task."); break;
    }

    return 0;
}
