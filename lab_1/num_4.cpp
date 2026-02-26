#include <stdio.h>
#include <stddef.h>
#include <wchar.h>
#include <type_traits>

static const size_t N = 5;
static const size_t I = 2; /* индекс i=2 */

static void clear_stdin_line()
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
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
    puts("--- Precision .2 for Mfl (A/e/f) ---");

    printf("%s (HEX exp A, .2): ", name);
    for (size_t i = 0; i < n; ++i) { if (i) printf(" "); printf("%.2A", a[i]); }
    printf("\n");

    printf("%s (DEC exp e, .2): ", name);
    for (size_t i = 0; i < n; ++i) { if (i) printf(" "); printf("%.2e", a[i]); }
    printf("\n");

    printf("%s (DEC f, .2): ", name);
    for (size_t i = 0; i < n; ++i) { if (i) printf(" "); printf("%.2f", a[i]); }
    printf("\n");
}

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

int main()
{
    /* начальные x из Л1.№3 */
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
    return 0;
}
