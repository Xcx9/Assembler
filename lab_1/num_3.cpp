#include <stdio.h>
#include <stddef.h>
#include <wchar.h>
#include <type_traits>

static const size_t N = 5;

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

template <typename T>
static void print_array_hex(const char* name, const T* a, size_t n, const char* mod)
{
    const int w = (int)(2u * sizeof(T));
    char fmt[32];

    /* "%0<width><mod>X" */
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

    /* "%<mod>u" */
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

    /* "%+<mod>d" */
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
    /* Демонстрация w > w0 и "-" для одной пары массив+формат */
    const int w = 10; /* заведомо больше типичной ширины */
    puts("\n--- Width demo for Ms with format %hu ---");
    puts("If w <= w0: output looks the same (no extra spaces).");
    puts("If w > w0: field is padded with spaces (default right-aligned).");
    puts("With '-' : left-aligned inside the field.");


    printf("Ms with width (right aligned): ");
    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");
        printf("%10hu", a[i]);
    }
    printf("\n");

    printf("Ms with width and '-' (left aligned): ");
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

    puts("Precision .2 changes the number of digits after the decimal point (rounding is applied).");
}

int main()
{
    /* x заданные значениями */
    const unsigned char xb = 0xA7;
    const unsigned short xs = 0xC0DE;
    const unsigned int xl = 0xDEADBEEFu;
    const unsigned long long xq = 0x000D15A550C1A7EDULL;
    const float xfs = -3.0f / 7.0f;
    const double xfl = -3.0 / 7.0;

    /* Массивы N элементов */
    unsigned char Mb[N] = { xb, xb, xb, xb, xb };
    unsigned short Ms[N] = { xs, xs, xs, xs, xs };
    unsigned int Ml[N] = { xl, xl, xl, xl, xl };
    unsigned long long Mq[N] = { xq, xq, xq, xq, xq };
    float Mfs[N] = { xfs, xfs, xfs, xfs, xfs };
    double Mfl[N] = { xfl, xfl, xfl, xfl, xfl };

    puts("=== Integer arrays (each printed 4 times: X, b, u, d) ===");
    /* Mb: unsigned char -> modifier "hh" */
    print_array_hex("Mb", Mb, N, "hh");
    print_array_bin("Mb", Mb, N);
    print_array_unsigned("Mb", Mb, N, "hh");
    print_array_signed_plus("Mb", Mb, N, "hh");

    /* Ms: unsigned short -> modifier "h" */
    print_array_hex("Ms", Ms, N, "h");
    print_array_bin("Ms", Ms, N);
    print_array_unsigned("Ms", Ms, N, "h");
    print_array_signed_plus("Ms", Ms, N, "h");

    /* Ml: unsigned int -> no modifier */
    print_array_hex("Ml", Ml, N, "");
    print_array_bin("Ml", Ml, N);
    print_array_unsigned("Ml", Ml, N, "");
    print_array_signed_plus("Ml", Ml, N, "");

    /* Mq: unsigned long long -> modifier "ll" */
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

    puts("\nNote: Binary output here is produced manually because %b is often not supported by MinGW printf.");
    return 0;
}
