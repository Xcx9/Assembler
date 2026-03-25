#include <stdio.h>   // Нужен для printf, puts, putchar, scanf
#include <stddef.h>  // Нужен для size_t и ptrdiff_t
#include <wchar.h>   // Нужен для wchar_t
#include <climits>   // Нужен для CHAR_MIN, INT_MAX, SHRT_MAX и т.д.
#include <cfloat>    // Нужен для FLT_MIN, DBL_MAX и т.д.
#include <string.h>  // Нужен для memcpy

// Макрос для красивого вывода размера типа.
// #T превращает имя типа в строку.
// sizeof(T) возвращает размер типа в байтах.
#define SHOW_SIZE(T) printf("%-22s : %2zu byte(s)\n", #T, (size_t)sizeof(T))

// ======================================================
// Безопасное чтение значения из памяти
// ======================================================
// Эта шаблонная функция получает адрес p и читает из памяти
// значение типа T через memcpy.
// Это безопаснее, чем напрямую разыменовывать указатель
// другого типа через reinterpret_cast.
template <typename T>
T readValue(const void *p)
{
    T value;                        // Объявляем переменную нужного типа
    memcpy(&value, p, sizeof(T));   // Копируем в неё sizeof(T) байт из памяти по адресу p
    return value;                   // Возвращаем готовое значение
}

// ======================================================
// Общие функции вывода
// ======================================================

// Функция печатает 16 бит в разных видах:
// hex, binary, unsigned short, signed short
void print16(const void *p)
{
    // Читаем те же 2 байта как unsigned short
    unsigned short ux = readValue<unsigned short>(p);

    // Читаем те же 2 байта как short
    short sx = readValue<short>(p);

    // Печатаем число в шестнадцатеричном виде.
    // %04hX означает:
    // 0  -> добавлять нули слева
    // 4  -> ширина 4 символа
    // h  -> short
    // X  -> hex в верхнем регистре
    printf("%04hX ", ux);

    // Цикл идёт по битам от 15 до 0.
    // Так мы печатаем число в двоичном виде.
    for (int i = 15; i >= 0; --i) {
        // 1u << i создаёт маску, где 1 стоит в i-м бите
        // ux & маска проверяет, установлен ли этот бит
        // если установлен -> печатаем '1'
        // если нет -> печатаем '0'
        putchar((ux & (1u << i)) ? '1' : '0');
    }

    // Печатаем это же значение:
    // как unsigned short
    // и как signed short
    printf(" %5hu %+6hd", ux, sx);
}

// Функция печатает 32 бита в разных видах:
// hex, binary, unsigned, int, float
void print32(const void *p)
{
    // Читаем те же 4 байта как unsigned
    unsigned ux = readValue<unsigned>(p);

    // Читаем те же 4 байта как int
    int sx = readValue<int>(p);

    // Читаем те же 4 байта как float
    float fx = readValue<float>(p);

    // Печатаем 32-битное значение в hex.
    // 32 бита = 8 hex-цифр
    printf("%08X ", ux);

    // Печатаем двоичный вид 32-битного значения
    for (int i = 31; i >= 0; --i) {
        putchar((ux & (1u << i)) ? '1' : '0');
    }

    // Печатаем:
    // ux -> как unsigned
    // sx -> как signed int
    // fx -> как float в 3 форматах
    // %A  -> hex-представление float
    // %e  -> экспоненциальная запись
    // %f  -> обычная запись
    printf(" %10u %+11d %+.2A %+.2e %+.2f", ux, sx, fx, fx, fx);
}

// Функция печатает 64 бита в разных видах:
// hex, binary, unsigned long long, long long, double
void print64(const void *p)
{
    // Читаем те же 8 байт как unsigned long long
    unsigned long long ux = readValue<unsigned long long>(p);

    // Читаем те же 8 байт как signed long long
    long long sx = readValue<long long>(p);

    // Читаем те же 8 байт как double
    double dx = readValue<double>(p);

    // Печатаем 64-битное значение в hex.
    // 64 бита = 16 hex-цифр
    printf("%016llX ", ux);

    // Печатаем двоичный вид 64-битного значения
    for (int i = 63; i >= 0; --i) {
        // 1ULL — это 64-битная единица
        putchar((ux & (1ULL << i)) ? '1' : '0');
    }

    // Перевод строки, потому что строка получается длинной
    putchar('\n');

    // Печатаем то же самое число:
    // как unsigned long long
    // как long long
    // как double в трёх форматах
    printf("                 %20llu %+21lld %+20.2A %+20.2e %+20.2f",
           ux, sx, dx, dx, dx);
}

// Функция печатает дамп памяти побайтно.
// То есть показывает каждый байт в hex.
void printDump(const void *p, size_t N)
{
    // Рассматриваем память как массив байтов
    const unsigned char *p1 = (const unsigned char *)p;

    // Идём по всем байтам от 0 до N-1
    for (size_t i = 0; i < N; ++i) {
        // Между байтами печатаем пробел
        if (i) putchar(' ');

        // Печатаем один байт как две hex-цифры
        printf("%02hhX", p1[i]);
    }

    // Перевод строки
    putchar('\n');
}

// ======================================================
// Л2.2 - работа с указателями
// ======================================================

// Функция показывает адреса указателей и как они меняются при +1
void viewPointer(void *p)
{
    // Один и тот же адрес p рассматриваем как указатели разных типов
    char *p1 = (char *)p;                     // Шаг 1 байт
    unsigned short *p2 = (unsigned short *)p; // Шаг 2 байта
    double *p3 = (double *)p;                 // Шаг 8 байт

    // Печатаем сами адреса
    printf("p  = %p\n", p);
    printf("p1 = %p\n", (void *)p1);
    printf("p2 = %p\n", (void *)p2);
    printf("p3 = %p\n", (void *)p3);

    // Печатаем адреса после прибавления 1
    // Важно: +1 к указателю означает переход к следующему
    // объекту этого типа, а не просто +1 байт
    printf("p1 + 1 = %p\n", (void *)(p1 + 1));
    printf("p2 + 1 = %p\n", (void *)(p2 + 1));
    printf("p3 + 1 = %p\n", (void *)(p3 + 1));

    // Считаем реальную разницу между адресами в байтах
    printf("diff(p1, p1+1) = %td byte(s), sizeof(char) = %zu\n",
           (ptrdiff_t)((char *)(p1 + 1) - (char *)p1), sizeof(char));

    printf("diff(p2, p2+1) = %td byte(s), sizeof(unsigned short) = %zu\n",
           (ptrdiff_t)((char *)(p2 + 1) - (char *)p2), sizeof(unsigned short));

    printf("diff(p3, p3+1) = %td byte(s), sizeof(double) = %zu\n",
           (ptrdiff_t)((char *)(p3 + 1) - (char *)p3), sizeof(double));

    // Это GNU-расширение.
    // В стандартном C/C++ арифметика для void* не разрешена.
#if defined(__GNUC__) && !defined(__STRICT_ANSI__)
    void *pv1 = p + 1;
    printf("p + 1  = %p\n", pv1);
    printf("diff(p, p+1) = %td byte(s)\n",
           (ptrdiff_t)((char *)pv1 - (char *)p));
#else
    puts("p + 1 for void* is not available under current compiler settings.");
#endif
}

// Функция показывает, как одна и та же память читается
// как байты, как unsigned short и как double
void printPointer(const void *p)
{
    // Представляем память как массив байтов
    const unsigned char *bytes = (const unsigned char *)p;

    // Читаем первый и второй байт
    unsigned char b0 = readValue<unsigned char>(bytes);
    unsigned char b1 = readValue<unsigned char>(bytes + 1);

    // Читаем первые 2 байта и следующие 2 байта как unsigned short
    unsigned short s0 = readValue<unsigned short>(bytes);
    unsigned short s1 = readValue<unsigned short>(bytes + sizeof(unsigned short));

    // Читаем первые 8 байт и следующие 8 байт как double
    double d0 = readValue<double>(bytes);
    double d1 = readValue<double>(bytes + sizeof(double));

    // Печатаем результаты
    printf("*p1       = 0x%02X\n", b0);
    printf("*(p1 + 1) = 0x%02X\n", b1);

    printf("*p2       = 0x%04hX\n", s0);
    printf("*(p2 + 1) = 0x%04hX\n", s1);

    printf("*p3       = %A\n", d0);
    printf("*(p3 + 1) = %A\n", d1);
}

// ======================================================
// Л2.8 - расширение 16 бит до 32 бит
// ======================================================

// Функция показывает, как 16-битное число расширяется до 32 бит
// как signed и как unsigned
void c16to32(const void *p)
{
    // Читаем исходные 16 бит как short
    short s16 = readValue<short>(p);

    // Читаем те же 16 бит как unsigned short
    unsigned short u16 = readValue<unsigned short>(p);

    // Знаковое расширение short -> int
    int s32 = (int)s16;

    // Беззнаковое расширение unsigned short -> unsigned
    unsigned u32 = (unsigned)u16;

    puts("16-bit source:");
    print16(p);
    putchar('\n');

    puts("32-bit extension as signed (short -> int):");
    print32(&s32);
    putchar('\n');

    puts("32-bit extension as unsigned (unsigned short -> unsigned int):");
    print32(&u32);
    putchar('\n');
}

// ======================================================
// Л2.9 - сравнение арифметических и побитовых операций
// ======================================================

// Функция сравнивает обычные арифметические операции
// и похожие на них побитовые операции
void ab16(const void *p)
{
    // Читаем число как unsigned short
    unsigned short ux = readValue<unsigned short>(p);

    // Читаем те же биты как short
    short sx = readValue<short>(p);

    puts("x:");
    print16(p);
    putchar('\n');

    // a1 / b1
    // unsigned multiply by 2
    // unsigned shift left by 1
    {
        unsigned short a = (unsigned short)(ux * (unsigned short)2);
        unsigned short b = (unsigned short)(ux << 1);

        puts("a1(x): unsigned multiply by 2    | b1(x): unsigned shift left by 1");
        print16(&a);
        printf("    |    ");
        print16(&b);
        putchar('\n');
    }

    // a2 / b2
    // signed multiply by 2
    // и безопасный сдвиг влево на уровне битов
    {
        short a = (short)(sx * (short)2);

        // Чтобы избежать неопределённого поведения
        // для signed left shift, делаем сдвиг над unsigned-копией битов
        unsigned short temp = (unsigned short)sx;
        unsigned short shifted = (unsigned short)(temp << 1);

        // Теперь трактуем результат как short
        short b = readValue<short>(&shifted);

        puts("a2(x): signed multiply by 2      | b2(x): bit pattern after left shift by 1");
        print16(&a);
        printf("    |    ");
        print16(&b);
        putchar('\n');
    }

    // a3 / b3
    // unsigned divide by 2
    // unsigned shift right by 1
    {
        unsigned short a = (unsigned short)(ux / (unsigned short)2);
        unsigned short b = (unsigned short)(ux >> 1);

        puts("a3(x): unsigned divide by 2      | b3(x): unsigned shift right by 1");
        print16(&a);
        printf("    |    ");
        print16(&b);
        putchar('\n');
    }

    // a4 / b4
    // signed divide by 2
    // signed shift right by 1
    {
        short a = (short)(sx / (short)2);

        int temp = (int)sx;
        int shifted = temp >> 1;
        short b = (short)shifted;

        puts("a4(x): signed divide by 2        | b4(x): signed shift right by 1");
        print16(&a);
        printf("    |    ");
        print16(&b);
        putchar('\n');
    }

    // a5 / b5
    // remainder by 16
    // and x & 15
    {
        unsigned short a = (unsigned short)(ux % (unsigned short)16);
        unsigned short b = (unsigned short)(ux & (unsigned short)15);

        puts("a5(x): unsigned remainder /16    | b5(x): x & 15");
        print16(&a);
        printf("    |    ");
        print16(&b);
        putchar('\n');
    }

    // a6 / b6
    // округление вниз до кратного 16
    // и x & -16
    {
        unsigned short a = (unsigned short)((ux / (unsigned short)16) * (unsigned short)16);
        unsigned short b = (unsigned short)(ux & (unsigned short)-16);

        puts("a6(x): floor to multiple of 16   | b6(x): x & -16");
        print16(&a);
        printf("    |    ");
        print16(&b);
        putchar('\n');
    }
}

// ======================================================
// Функции запуска отдельных заданий
// ======================================================

// Задание 1
void runTask1()
{
    puts("==================================================");
    puts("L2.1 - Sizes of standard C/C++ types");
    puts("==================================================");

    // Печатаем размеры типов
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

    puts("");
    puts("Note: for the full bonus report, task L2.1 should also include");
    puts("a comparison table for several platforms.");
}

// Задание 2
void runTask2()
{
    puts("==================================================");
    puts("L2.2 - Pointer arithmetic and dereferencing");
    puts("==================================================");

    // Буфер из двух long long
    // Первый элемент содержит число
    // Второй нужен, чтобы второй double не вышел за границы памяти
    long long xbuf[2] = { 0x1122334455667788LL, 0LL };

    // Строка для второго теста
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
}

// Задание 3
void runTask3()
{
    puts("==================================================");
    puts("L2.3 - Min / Max values and theory");
    puts("==================================================");

    puts("=== Integer types: min / max ===");

    printf("char                : min = %d, max = %d\n", CHAR_MIN, CHAR_MAX);
    printf("unsigned char       : min = 0, max = %u\n", UCHAR_MAX);

    printf("short               : min = %d, max = %d\n", SHRT_MIN, SHRT_MAX);
    printf("unsigned short      : min = 0, max = %u\n", USHRT_MAX);

    printf("int                 : min = %d, max = %d\n", INT_MIN, INT_MAX);
    printf("unsigned int        : min = 0, max = %u\n", UINT_MAX);

    printf("long long           : min = %lld, max = %lld\n", LLONG_MIN, LLONG_MAX);
    printf("unsigned long long  : min = 0, max = %llu\n", ULLONG_MAX);

    puts("");
    puts("=== Floating-point types: min / max ===");

    printf("float               : min = %e, max = %e\n", FLT_MIN, FLT_MAX);
    printf("double              : min = %e, max = %e\n", DBL_MIN, DBL_MAX);

    puts("");
    puts("=== Answers for integer types ===");

    puts("1) Number of values:");
    puts("   Unsigned N-bit type can store 2^N different values.");
    puts("   Signed N-bit type also stores 2^N different values.");
    puts("   Yes, this is directly related to N: one more bit doubles the number of possible bit patterns.");

    puts("");
    puts("2) Unsigned N-bit representation:");
    puts("   Every integer x in [0, 2^N) has its own N-bit representation.");
    puts("   Every sequence of N bits can be interpreted as an unsigned integer in [0, 2^N).");
    puts("   The correspondence is one-to-one.");

    puts("");
    puts("3) Signed N-bit representation:");
    puts("   In two's complement, every integer x in [-2^(N-1), 2^(N-1)) has its own N-bit representation.");
    puts("   Every sequence of N bits can be interpreted as exactly one signed integer from this range.");
    puts("   The correspondence is one-to-one.");

    puts("");
    puts("=== Answers for float / double ===");

    puts("1) Does every real x in [min, max] have its own IEEE 754 representation?");
    puts("   No. Real numbers are infinite, but float/double representations are finite.");
    puts("   Therefore only some real numbers are represented exactly.");

    puts("");
    puts("2) Is the correspondence between real numbers and IEEE 754 representations one-to-one?");
    puts("   No, not for all real numbers. Many real numbers do not have exact representation.");
    puts("   But each stored finite machine value has its own bit representation.");

    puts("");
    puts("3) Can every 32-bit or 64-bit sequence be interpreted as float/double?");
    puts("   Yes. Every such bit sequence can be interpreted as a float/double value.");
    puts("   But the result is not always a number: there are +infinity, -infinity and NaN.");

    puts("");
    puts("4) Which set of numeric values is larger?");
    puts("   Numeric 32-bit integer values are more numerous than numeric float values.");
    puts("   Numeric 64-bit integer values are more numerous than numeric double values.");
    puts("   Reason: among float/double bit patterns there are infinities, NaNs, and two zeros (+0 and -0).");
}

// Задание 4
void runTask4()
{
    puts("==================================================");
    puts("L2.4 - print16(void*)");
    puts("==================================================");

    unsigned short us_min = 0;
    unsigned short us_max = USHRT_MAX;

    short s_min = SHRT_MIN;
    short s_max = SHRT_MAX;

    // Для группы 8, вариант 2
    short x16 = 5;
    short y16 = -5;
    short a16 = 1;
    short b16 = 2;

    puts("name     HEX  binary              unsigned signed");

    printf("%-7s", "us_min");
    print16(&us_min);
    putchar('\n');

    printf("%-7s", "us_max");
    print16(&us_max);
    putchar('\n');

    printf("%-7s", "s_min");
    print16(&s_min);
    putchar('\n');

    printf("%-7s", "s_max");
    print16(&s_max);
    putchar('\n');

    printf("%-7s", "x");
    print16(&x16);
    putchar('\n');

    printf("%-7s", "y");
    print16(&y16);
    putchar('\n');

    printf("%-7s", "a");
    print16(&a16);
    putchar('\n');

    printf("%-7s", "b");
    print16(&b16);
    putchar('\n');
}

// Задание 5
void runTask5()
{
    puts("==================================================");
    puts("L2.5 - print32(void*)");
    puts("==================================================");

    unsigned u_min = 0u;
    unsigned u_max = UINT_MAX;

    int s32_min = INT_MIN;
    int s32_max = INT_MAX;

    // Значения варианта 2
    int x32 = 5;
    int y32 = -5;
    int a32 = 1;
    int b32 = 2;
    int c32 = 12345689;
    int d32 = 123456891;

    // Те же значения как float
    float fx32 = 5.0f;
    float fy32 = -5.0f;
    float fa32 = 1.0f;
    float fb32 = 2.0f;
    float fc32 = 12345689.0f;
    float fd32 = 123456891.0f;

    // Специальные битовые шаблоны float
    unsigned plus_zero_bits    = 0x00000000u;
    unsigned minus_zero_bits   = 0x80000000u;
    unsigned min_denorm_bits   = 0x00000001u;
    unsigned nan_bits          = 0x7FC00000u;
    unsigned min_norm_bits     = 0x00800000u;
    unsigned before_plus2_bits = 0x3FFFFFFFu;
    unsigned max_norm_bits     = 0x7F7FFFFFu;
    unsigned plus_inf_bits     = 0x7F800000u;

    puts("name              HEX      binary                               unsigned        signed  float-hex           float-exp           float-fix");

    printf("%-18s", "u_min");
    print32(&u_min);
    putchar('\n');

    printf("%-18s", "u_max");
    print32(&u_max);
    putchar('\n');

    printf("%-18s", "s_min");
    print32(&s32_min);
    putchar('\n');

    printf("%-18s", "s_max");
    print32(&s32_max);
    putchar('\n');

    printf("%-18s", "x (int)");
    print32(&x32);
    putchar('\n');

    printf("%-18s", "y (int)");
    print32(&y32);
    putchar('\n');

    printf("%-18s", "a (int)");
    print32(&a32);
    putchar('\n');

    printf("%-18s", "b (int)");
    print32(&b32);
    putchar('\n');

    printf("%-18s", "c (int)");
    print32(&c32);
    putchar('\n');

    printf("%-18s", "d (int)");
    print32(&d32);
    putchar('\n');

    puts("");

    printf("%-18s", "x (float)");
    print32(&fx32);
    putchar('\n');

    printf("%-18s", "y (float)");
    print32(&fy32);
    putchar('\n');

    printf("%-18s", "a (float)");
    print32(&fa32);
    putchar('\n');

    printf("%-18s", "b (float)");
    print32(&fb32);
    putchar('\n');

    printf("%-18s", "c (float)");
    print32(&fc32);
    putchar('\n');

    printf("%-18s", "d (float)");
    print32(&fd32);
    putchar('\n');

    puts("");

    printf("%-18s", "+0");
    print32(&plus_zero_bits);
    putchar('\n');

    printf("%-18s", "-0");
    print32(&minus_zero_bits);
    putchar('\n');

    printf("%-18s", "min denorm +");
    print32(&min_denorm_bits);
    putchar('\n');

    printf("%-18s", "NaN");
    print32(&nan_bits);
    putchar('\n');

    printf("%-18s", "min norm +");
    print32(&min_norm_bits);
    putchar('\n');

    printf("%-18s", "prev before +2");
    print32(&before_plus2_bits);
    putchar('\n');

    printf("%-18s", "max norm +");
    print32(&max_norm_bits);
    putchar('\n');

    printf("%-18s", "+infinity");
    print32(&plus_inf_bits);
    putchar('\n');
}

// Задание 6
void runTask6()
{
    puts("==================================================");
    puts("L2.6 - print64(void*)");
    puts("==================================================");

    unsigned long long ex1 = 13ULL;
    unsigned long long ex2 = 0x8000000000000000ULL;

    long long x64 = 5LL;
    long long a64 = 1LL;

    double fx64 = 5.0;
    double fa64 = 1.0;

    puts("13:");
    print64(&ex1);
    puts("");

    puts("0x8000000000000000:");
    print64(&ex2);
    puts("");

    puts("x (long long = 5):");
    print64(&x64);
    puts("");

    puts("a (long long = 1):");
    print64(&a64);
    puts("");

    puts("x (double = 5.0):");
    print64(&fx64);
    puts("");

    puts("a (double = 1.0):");
    print64(&fa64);
    puts("");
}

// Задание 7
void runTask7()
{
    puts("==================================================");
    puts("L2.7 - printDump(void*, size_t)");
    puts("==================================================");

    int x_int = 5;
    double x_double = 5.0;

    char s1[] = "abc012";
    char s2[] = "абв012";

    wchar_t ws1[] = L"abc012";
    wchar_t ws2[] = L"абв012";

    puts("=== int x = 5 ===");
    print32(&x_int);
    putchar('\n');
    puts("dump:");
    printDump(&x_int, sizeof(x_int));

    // Проверка порядка байтов
    if (*((unsigned char *)&x_int) == 0x05)
        puts("Byte order in words on this platform: Little-Endian.");
    else
        puts("Byte order in words on this platform: Big-Endian.");

    puts("");
    puts("=== double x = 5.0 ===");
    print64(&x_double);
    putchar('\n');
    puts("dump:");
    printDump(&x_double, sizeof(x_double));

    puts("");
    puts("=== char s1[] = \"abc012\" ===");
    puts("dump:");
    printDump(s1, sizeof(s1));

    puts("");
    puts("=== char s2[] = \"абв012\" ===");
    puts("dump:");
    printDump(s2, sizeof(s2));

    puts("");
    puts("=== wchar_t ws1[] = L\"abc012\" ===");
    puts("dump:");
    printDump(ws1, sizeof(ws1));

    puts("");
    puts("=== wchar_t ws2[] = L\"абв012\" ===");
    puts("dump:");
    printDump(ws2, sizeof(ws2));
}

// Задание 8
void runTask8()
{
    puts("==================================================");
    puts("L2.8 - c16to32(void*)");
    puts("==================================================");

    // Для группы 8, вариант 2
    short m = 21;
    short n = -37;

    puts("m:");
    c16to32(&m);
    puts("");

    puts("n:");
    c16to32(&n);
}

// Задание 9
void runTask9()
{
    puts("==================================================");
    puts("L2.9 - ab16(void*)");
    puts("==================================================");

    // Для группы 8, вариант 2
    short m = 21;
    short n = -37;

    puts("m:");
    ab16(&m);
    puts("");

    puts("n:");
    ab16(&n);
    puts("");

    puts("Total number of operations described in task L2.9: 12.");
}

// Функция запуска всех заданий сразу
void runAllTasks()
{
    runTask1();
    puts("");

    runTask2();
    puts("");

    runTask3();
    puts("");

    runTask4();
    puts("");

    runTask5();
    puts("");

    runTask6();
    puts("");

    runTask7();
    puts("");

    runTask8();
    puts("");

    runTask9();
}

// ======================================================
// main
// ======================================================
int main()
{
    // Переменная для выбора задания
    int choice = -1;

    // Печатаем меню
    puts("==============================================");
    puts("Laboratory work #2");
    puts("Choose task number to run:");
    puts("0 - run all tasks");
    puts("1 - L2.1");
    puts("2 - L2.2");
    puts("3 - L2.3");
    puts("4 - L2.4");
    puts("5 - L2.5");
    puts("6 - L2.6");
    puts("7 - L2.7");
    puts("8 - L2.8");
    puts("9 - L2.9");
    puts("==============================================");
    printf("Enter number: ");

    // Считываем число с клавиатуры
    if (scanf("%d", &choice) != 1) {
        puts("Input error. Please enter an integer from 0 to 9.");
        return 1;
    }

    puts("");

    // В зависимости от введённого числа запускаем нужное задание
    switch (choice) {
        case 0:
            runAllTasks();
            break;
        case 1:
            runTask1();
            break;
        case 2:
            runTask2();
            break;
        case 3:
            runTask3();
            break;
        case 4:
            runTask4();
            break;
        case 5:
            runTask5();
            break;
        case 6:
            runTask6();
            break;
        case 7:
            runTask7();
            break;
        case 8:
            runTask8();
            break;
        case 9:
            runTask9();
            break;
        default:
            puts("There is no such task number. Enter a number from 0 to 9.");
            return 1;
    }

    // Успешное завершение программы
    return 0;
}
