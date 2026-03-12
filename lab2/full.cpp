#include <stdio.h>      // printf, scanf, puts, snprintf, getchar, putchar
#include <stddef.h>     // size_t, ptrdiff_t
#include <wchar.h>      // wchar_t
#include <type_traits>  // make_unsigned, make_signed

// Именованная константа длины массивов.
// Это важно, потому что в условии есть штраф,
// если вместо именованной константы используется литерал 5.
static const size_t N = 5;

// Индекс элемента для задания №4.
// По условию нужно менять именно M[i], где i = 2.
static const size_t I = 2;

// Число строк матрицы для задания №5.
// Выбрано 2, потому что в условии нужно вывести адреса
// MM[0][0], MM[0][1], MM[1][0], MM[1][1].
static const size_t R = 2;

/* =========================================================
   ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ
   ========================================================= */

// Функция очищает остаток текущей строки во входном потоке.
// Нужна после scanf, чтобы убрать '\n' и возможный лишний ввод.
static void clear_stdin_line()
{
    int c;  // getchar() возвращает int, а не char

    // Читаем символы, пока не встретим перевод строки или конец файла.
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

// Шаблонная функция печати одного значения в двоичном виде.
// Используется для целочисленных массивов в задании №3 и №4.
template <typename T>
static void print_binary_value(T v)
{
    // Количество бит = 8 * sizeof(T)
    const size_t bits = 8u * sizeof(T);

    // Преобразуем значение к беззнаковому типу такого же размера,
    // затем к unsigned long long, чтобы удобно работать с битами.
    unsigned long long u =
        (unsigned long long)(typename std::make_unsigned<T>::type)v;

    // Буфер под строку из 0 и 1.
    // Максимум нам нужно 64 символа + '\0'.
    char s[65];

    // Счётчик символов.
    size_t i = 0;

    // Проходим по битам от старшего к младшему.
    for (; i < bits && i < sizeof(s) - 1; ++i) {
        size_t shift = (bits - 1u) - i;             // сколько сдвинуть вправо
        s[i] = ((u >> shift) & 1ULL) ? '1' : '0';   // получаем один бит
    }

    // Завершаем строку.
    s[i] = '\0';

    // Печатаем двоичную строку.
    printf("%s", s);
}

// Отдельная функция печати unsigned long long в двоичном виде.
// Используется в задании №6 для массива Mq.
static void print_binary_ull(unsigned long long v)
{
    const size_t bits = 8u * sizeof(unsigned long long);

    for (size_t i = 0; i < bits; ++i) {
        size_t shift = (bits - 1u) - i;
        putchar(((v >> shift) & 1ULL) ? '1' : '0');
    }
}

// Печать массива в HEX-формате с ведущими нулями.
// name - имя массива для подписи
// a    - указатель на массив
// n    - длина массива
// mod  - модификатор размера: "hh", "h", "", "ll"
template <typename T>
static void print_array_hex(const char* name, const T* a, size_t n, const char* mod)
{
    // Ширина поля в hex-цифрах: 2 цифры на 1 байт.
    const int w = (int)(2u * sizeof(T));

    // Буфер для строки формата printf.
    char fmt[32];

    // Если модификатор пустой, например для unsigned int,
    // делаем формат вида %08X.
    if (mod[0] == '\0')
        snprintf(fmt, sizeof(fmt), "%%0%dX", w);
    else
        // Иначе, например, %04hX или %016llX.
        snprintf(fmt, sizeof(fmt), "%%0%d%sX", w, mod);

    // Печатаем подпись.
    printf("%s (HEX X): ", name);

    // Печатаем все элементы массива.
    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");  // между элементами пробел

        // Для маленьких типов передаём unsigned int,
        // для 64-битных — unsigned long long.
        if constexpr (sizeof(T) <= sizeof(unsigned int))
            printf(fmt, (unsigned int)a[i]);
        else
            printf(fmt, (unsigned long long)a[i]);
    }

    printf("\n");
}

// Печать массива в двоичном виде.
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


// Печать массива как unsigned decimal.
template <typename T>
static void print_array_unsigned(const char* name, const T* a, size_t n, const char* mod)
{
    char fmt[16];

    // Строим формат:
    // %u, %hu, %hhu, %llu ...
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

// Печать массива как signed decimal со знаком '+' у положительных чисел.
template <typename T>
static void print_array_signed_plus(const char* name, const T* a, size_t n, const char* mod)
{
    // S — знаковая версия типа T.
    using S = typename std::make_signed<T>::type;

    char fmt[16];

    // Строим формат:
    // %+d, %+hd, %+hhd, %+lld ...
    if (mod[0] == '\0')
        snprintf(fmt, sizeof(fmt), "%%+d");
    else
        snprintf(fmt, sizeof(fmt), "%%+%sd", mod);

    printf("%s (DEC d with +): ", name);

    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");

        // Интерпретируем тот же набор бит как signed-тип.
        S sv = (S)a[i];

        if constexpr (sizeof(T) <= sizeof(int))
            printf(fmt, (int)sv);
        else
            printf(fmt, (long long)sv);
    }

    printf("\n");
}

// Печать массива Mb в символьном виде (%c).
// Нужна для задания №3.
static void print_array_chars(const char* name, const unsigned char* a, size_t n)
{
    printf("%s (CHAR c): ", name);

    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");
        printf("%c", (int)a[i]);
    }

    printf("\n");
}

// Демонстрация ширины поля и флага '-'.
// Используется в задании №3.
static void width_demo_unsigned_short(const unsigned short* a, size_t n)
{
    const int w = 10;  // заведомо больше обычной длины числа

    puts("\n--- Width demo for Ms with format %hu ---");
    puts("w <= w0 : looks the same.");
    puts("w >  w0 : padded with spaces (right-aligned).");
    puts("with '-' : left-aligned.");

    // По умолчанию выравнивание вправо.
    printf("Ms right aligned: ");
    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");
        printf("%10hu", a[i]);
    }
    printf("\n");

    // С флагом '-' выравнивание влево.
    printf("Ms left  aligned: ");
    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");
        printf("%-10hu", a[i]);
    }
    printf("\n");
}

// Печать массива float в форматах A, e, f.
static void print_float_array_forms(const char* name, const float* a, size_t n)
{
    printf("%s (HEX exp A): ", name);
    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");
        printf("%A", a[i]);
    }
    printf("\n");

    printf("%s (DEC exp e): ", name);
    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");
        printf("%e", a[i]);
    }
    printf("\n");

    printf("%s (DEC f): ", name);
    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");
        printf("%f", a[i]);
    }
    printf("\n");
}

// Печать массива double в форматах A, e, f с обычной точностью.
static void print_double_array_forms_default(const char* name, const double* a, size_t n)
{
    printf("%s (HEX exp A): ", name);
    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");
        printf("%A", a[i]);
    }
    printf("\n");

    printf("%s (DEC exp e): ", name);
    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");
        printf("%e", a[i]);
    }
    printf("\n");

    printf("%s (DEC f): ", name);
    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");
        printf("%f", a[i]);
    }
    printf("\n");
}

// Печать массива double в форматах A, e, f с точностью .2.
// Это выполняет отдельное требование задания №3.
static void print_double_array_forms_prec2(const char* name, const double* a, size_t n)
{
    puts("\n--- Precision demo (.2) for Mfl (A/e/f) ---");


    printf("%s (HEX exp A, .2): ", name);
    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");
        printf("%.2A", a[i]);
    }
    printf("\n");

    printf("%s (DEC exp e, .2): ", name);
    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");
        printf("%.2e", a[i]);
    }
    printf("\n");

    printf("%s (DEC f, .2): ", name);
    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");
        printf("%.2f", a[i]);
    }
    printf("\n");

    puts("Precision .2 changes digits after decimal point (rounding).");
}

/* =========================================================
   ГРУППИРОВКА ВЫВОДА ДЛЯ ЗАДАНИЯ №4
   ========================================================= */

// Печать Mb во всех нужных форматах.
static void print_int_all_formats_Mb(const unsigned char* a)
{
    print_array_hex("Mb", a, N, "hh");
    print_array_bin("Mb", a, N);
    print_array_unsigned("Mb", a, N, "hh");
    print_array_signed_plus("Mb", a, N, "hh");
    print_array_chars("Mb", a, N);
}

// Печать Ms во всех нужных форматах.
static void print_int_all_formats_Ms(const unsigned short* a)
{
    print_array_hex("Ms", a, N, "h");
    print_array_bin("Ms", a, N);
    print_array_unsigned("Ms", a, N, "h");
    print_array_signed_plus("Ms", a, N, "h");
}

// Печать Ml во всех нужных форматах.
static void print_int_all_formats_Ml(const unsigned int* a)
{
    print_array_hex("Ml", a, N, "");
    print_array_bin("Ml", a, N);
    print_array_unsigned("Ml", a, N, "");
    print_array_signed_plus("Ml", a, N, "");
}

// Печать Mq во всех нужных форматах.
static void print_int_all_formats_Mq(const unsigned long long* a)
{
    print_array_hex("Mq", a, N, "ll");
    print_array_bin("Mq", a, N);
    print_array_unsigned("Mq", a, N, "ll");
    print_array_signed_plus("Mq", a, N, "ll");
}

// Отдельная функция вывода Mq для задания №6.
static void print_Mq_all(const unsigned long long* a)
{
    puts("Mq (HEX X, with leading zeros):");
    for (size_t i = 0; i < N; ++i) {
        if (i) printf(" ");
        printf("%016llX", a[i]);  // 16 hex-цифр для 8 байт
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

/* =========================================================
   ЗАДАНИЕ №1
   ========================================================= */

static void task1()
{
    // Один вызов puts().
    // Внутри строки есть \n, поэтому группа, номер работы и состав
    // выводятся на разных строках.
    puts("Gruppa: 8\n"
         "Laboratornaya rabota №1\n"
         "Sostav komandy:\n"
         "Ivanov I.I.\n"
         "Petrov P.P.");
}

/* =========================================================
   ЗАДАНИЕ №2
   ========================================================= */

static void task2()
{
    puts("=== sizeof() for required types ===");

    // Макрос печатает имя типа и его размер в байтах.
    // Это даёт бонус: каждое имя типа в коде встречается один раз.
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


/* =========================================================
   ЗАДАНИЕ №3
   ========================================================= */

static void task3()
{
    // Исходные значения x для массивов.
    const unsigned char xb = 0xA7;
    const unsigned short xs = 0xC0DE;
    const unsigned int xl = 0xDEADBEEFu;
    const unsigned long long xq = 0x000D15A550C1A7EDULL;
    const float xfs = -3.0f / 7.0f;
    const double xfl = -3.0 / 7.0;

    // Создание шести массивов длины N = 5.
    unsigned char Mb[N] = { xb, xb, xb, xb, xb };
    unsigned short Ms[N] = { xs, xs, xs, xs, xs };
    unsigned int Ml[N] = { xl, xl, xl, xl, xl };
    unsigned long long Mq[N] = { xq, xq, xq, xq, xq };
    float Mfs[N] = { xfs, xfs, xfs, xfs, xfs };
    double Mfl[N] = { xfl, xfl, xfl, xfl, xfl };

    puts("=== Integer arrays (each printed 4 times: X, b, u, d) ===");

    // Mb
    print_array_hex("Mb", Mb, N, "hh");
    print_array_bin("Mb", Mb, N);
    print_array_unsigned("Mb", Mb, N, "hh");
    print_array_signed_plus("Mb", Mb, N, "hh");

    // Ms
    print_array_hex("Ms", Ms, N, "h");
    print_array_bin("Ms", Ms, N);
    print_array_unsigned("Ms", Ms, N, "h");
    print_array_signed_plus("Ms", Ms, N, "h");

    // Ml
    print_array_hex("Ml", Ml, N, "");
    print_array_bin("Ml", Ml, N);
    print_array_unsigned("Ml", Ml, N, "");
    print_array_signed_plus("Ml", Ml, N, "");

    // Mq
    print_array_hex("Mq", Mq, N, "ll");
    print_array_bin("Mq", Mq, N);
    print_array_unsigned("Mq", Mq, N, "ll");
    print_array_signed_plus("Mq", Mq, N, "ll");

    // Mb дополнительно как символы.
    puts("\n=== Mb printed 5th time as characters (%c) ===");
    print_array_chars("Mb", Mb, N);

    // Демонстрация ширины поля и флага '-'.
    width_demo_unsigned_short(Ms, N);

    puts("\n=== Floating point arrays (each printed 3 times: A, e, f) ===");

    // Mfs
    print_float_array_forms("Mfs", Mfs, N);

    // Mfl с обычной точностью
    puts("\nMfl default:");
    print_double_array_forms_default("Mfl", Mfl, N);

    // Mfl с точностью .2
    print_double_array_forms_prec2("Mfl", Mfl, N);

    // Пояснение про отсутствие %b в MinGW.
    puts("\nNote: Binary output is manual because %b is often not supported by MinGW printf.");
}

/* =========================================================
   ЗАДАНИЕ №4
   ========================================================= */

static void task4()
{
    // Те же исходные значения, что и в задании №3.
    const unsigned char xb = 0xA7;
    const unsigned short xs = 0xC0DE;
    const unsigned int xl = 0xDEADBEEFu;
    const unsigned long long xq = 0x000D15A550C1A7EDULL;
    const float xfs = -3.0f / 7.0f;
    const double xfl = -3.0 / 7.0;

    // Исходные массивы.
    unsigned char Mb[N] = { xb, xb, xb, xb, xb };
    unsigned short Ms[N] = { xs, xs, xs, xs, xs };
    unsigned int Ml[N] = { xl, xl, xl, xl, xl };
    unsigned long long Mq[N] = { xq, xq, xq, xq, xq };
    float Mfs[N] = { xfs, xfs, xfs, xfs, xfs };
    double Mfl[N] = { xfl, xfl, xfl, xfl, xfl };

    // Вывод до ввода.
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

    // Ввод нового значения прямо в Mb[2].
    printf("Enter new HEX value for Mb[2] (no 0x): ");
    if (scanf("%hhx", &Mb[I]) != 1) {
        puts("Input error for Mb[2].");
        clear_stdin_line();
    } else {
        clear_stdin_line();
    }

    // Ввод нового значения прямо в Ms[2].
    printf("Enter new HEX value for Ms[2] (no 0x): ");
    if (scanf("%hx", &Ms[I]) != 1) {
        puts("Input error for Ms[2].");
        clear_stdin_line();
    } else {
        clear_stdin_line();
    }


    // Ввод нового значения прямо в Ml[2].
    printf("Enter new HEX value for Ml[2] (no 0x): ");
    if (scanf("%x", &Ml[I]) != 1) {
        puts("Input error for Ml[2].");
        clear_stdin_line();
    } else {
        clear_stdin_line();
    }

    // Ввод нового значения прямо в Mq[2].
    printf("Enter new HEX value for Mq[2] (no 0x): ");
    if (scanf("%llx", &Mq[I]) != 1) {
        puts("Input error for Mq[2].");
        clear_stdin_line();
    } else {
        clear_stdin_line();
    }

    // Ввод нового значения прямо в Mfs[2].
    printf("Enter new value for Mfs[2] (float): ");
    if (scanf("%f", &Mfs[I]) != 1) {
        puts("Input error for Mfs[2].");
        clear_stdin_line();
    } else {
        clear_stdin_line();
    }

    // Ввод нового значения прямо в Mfl[2].
    printf("Enter new value for Mfl[2] (double): ");
    if (scanf("%lf", &Mfl[I]) != 1) {
        puts("Input error for Mfl[2].");
        clear_stdin_line();
    } else {
        clear_stdin_line();
    }

    // Вывод после ввода.
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

/* =========================================================
   ЗАДАНИЕ №5
   ========================================================= */

static void task5()
{
    // По варианту используется массив Mq.
    const unsigned long long xq = 0x000D15A550C1A7EDULL;
    unsigned long long Mq[N] = { xq, xq, xq, xq, xq };

    puts("=== Addresses for array Mq ===");

    // Адрес начала массива.
    printf("Mq (start)     = %p\n", (void*)Mq);

    // Адрес первого элемента.
    printf("&Mq[0]         = %p\n", (void*)&Mq[0]);

    // Адрес второго элемента.
    printf("&Mq[1]         = %p\n", (void*)&Mq[1]);

    puts("\n=== Compare address differences ===");

    // Размер одного элемента массива.
    printf("sizeof(Mq[0])  = %zu byte(s)\n", sizeof(Mq[0]));

    // Разности адресов в байтах.
    ptrdiff_t d0 = (char*)&Mq[0] - (char*)Mq;
    ptrdiff_t d1 = (char*)&Mq[1] - (char*)&Mq[0];

    printf("&Mq[0] - Mq     = %td byte(s)\n", d0);
    printf("&Mq[1] - &Mq[0] = %td byte(s)\n", d1);

    puts("\nConclusion: elements of the array are stored consecutively in memory.");

    // Статическая матрица R x N того же типа.
    unsigned long long MM[R][N] = { {0} };

    puts("\n=== Addresses for matrix MM[R][N] ===");

    // Адреса элементов матрицы.
    printf("&MM[0][0] = %p\n", (void*)&MM[0][0]);
    printf("&MM[0][1] = %p\n", (void*)&MM[0][1]);
    printf("&MM[1][0] = %p\n", (void*)&MM[1][0]);
    printf("&MM[1][1] = %p\n", (void*)&MM[1][1]);

    puts("\nConclusion: matrix is stored in row-major order (rows are contiguous).");

    // Объяснение динамического воспроизведения той же структуры.
    puts("\n=== Dynamic reproduction (flat array) ===");

    // Сколько памяти нужно.
    printf("Memory to allocate: R * N * sizeof(T) = %zu byte(s)\n",
           (size_t)(R * N * sizeof(unsigned long long)));

    // Формула линейного индекса.
    puts("Index mapping for flat array M:");
    puts("idx = i * N + j");

    // Пример вычисления idx.
    {
        size_t i = 1;
        size_t j = 1;
        size_t idx = i * N + j;
        printf("Example: i=%zu, j=%zu -> idx=%zu\n", i, j, idx);
    }
}

/* =========================================================
   ЗАДАНИЕ №6
   ========================================================= */

static void task6()
{
    // Исходный массив Mq.
    const unsigned long long xq = 0x000D15A550C1A7EDULL;
    unsigned long long Mq[N] = { xq, xq, xq, xq, xq };

    // Копия массива до ввода, чтобы потом посчитать,
    // сколько элементов реально изменилось.
    unsigned long long before[N];
    for (size_t i = 0; i < N; ++i) {
        before[i] = Mq[i];
    }


    // Вывод до ввода.
    puts("=== BEFORE INPUT ===");
    print_Mq_all(Mq);

    puts("\nEnter 5 HEX values for Mq[0..4] (no 0x), separated by spaces:");

    // Один вызов scanf() для всех пяти элементов массива.
    int k = scanf("%llx %llx %llx %llx %llx",
                  &Mq[0], &Mq[1], &Mq[2], &Mq[3], &Mq[4]);

    // Проверяем, сколько элементов реально введено.
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

    // Вывод после ввода.
    puts("\n=== AFTER INPUT ===");
    print_Mq_all(Mq);

    // Считаем, сколько элементов изменилось.
    {
        size_t changed = 0;

        for (size_t i = 0; i < N; ++i) {
            if (Mq[i] != before[i]) {
                ++changed;
            }
        }

        printf("\nChanged elements: %zu of %zu\n", changed, N);
    }
}

/* =========================================================
   ЗАДАНИЕ №7
   ========================================================= */

static void task7()
{
    // Буфер под s1 — обычное слово.
    char s1[64];

    // Буфер под s2. Размер 16, значит безопасно вводить максимум 15 символов.
    char s2[16];

    // Буфер под s3 — строка с пробелами.
    char s3[128];

    // Ввод s1: слово без пробелов.
    puts("Enter s1 (one word, no spaces):");
    if (scanf("%s", s1) != 1) {
        puts("Input error for s1");
        return;
    }

    // Ввод s2: слово с ограничением длины, чтобы не переполнить буфер.
    puts("Enter s2 (one word, max 15 chars):");
    if (scanf("%15s", s2) != 1) {
        puts("Input error for s2");
        return;
    }

    // Ввод s3: целая строка, может содержать пробелы.
    // Ведущий пробел в формате пропускает оставшийся '\n'.
    puts("Enter s3 (a whole line, may contain spaces):");
    if (scanf(" %127[^\n]", s3) != 1) {
        puts("Input error for s3");
        return;
    }

    // Вывод введённых строк между *** и ***.
    printf("***%s***\n", s1);
    printf("***%s***\n", s2);
    printf("***%s***\n", s3);
}

/* =========================================================
   ГЛАВНАЯ ФУНКЦИЯ
   ========================================================= */

int main()
{
    // Номер задания, которое выберет пользователь.
    int n = 0;

    // Просим ввести номер задания.
    puts("Enter task number (1-7):");

    // Считываем номер.
    if (scanf("%d", &n) != 1) {
        puts("Input error.");
        return 0;
    }

    // Убираем перевод строки после ввода числа.
    clear_stdin_line();

    // Запускаем нужное задание.
    switch (n) {
        case 1:
            task1();
            break;

        case 2:
            task2();
            break;

        case 3:
            task3();
            break;

        case 4:
            task4();
            break;

        case 5:
            task5();
            break;

        case 6:
            task6();
            break;

        case 7:
            task7();
            break;

        default:
            puts("No such task.");
            break;
    }

    // Завершение программы.
    return 0;
}
