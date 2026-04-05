#include <cstdio>
#include <cstddef>
#include <cstring>
#include <iostream>

const size_t N = 5;

// ------------------------------------------------------------
// Print unsigned short array in hexadecimal form
// ------------------------------------------------------------
void printMs(const unsigned short a[], size_t n) {
    for (size_t j = 0; j < n; ++j) {
        std::printf("%04hX", static_cast<unsigned int>(a[j]));
        if (j + 1 < n) {
            std::printf(" ");
        }
    }
    std::printf("\n");
}

// ------------------------------------------------------------
// Print unsigned int array in hexadecimal form
// ------------------------------------------------------------
void printMl(const unsigned int a[], size_t n) {
    for (size_t j = 0; j < n; ++j) {
        std::printf("%08X", a[j]);
        if (j + 1 < n) {
            std::printf(" ");
        }
    }
    std::printf("\n");
}

// ------------------------------------------------------------
// Print unsigned long long array in hexadecimal form
// ------------------------------------------------------------
void printMq(const unsigned long long a[], size_t n) {
    for (size_t j = 0; j < n; ++j) {
        std::printf("%016llX", a[j]);
        if (j + 1 < n) {
            std::printf(" ");
        }
    }
    std::printf("\n");
}

// ------------------------------------------------------------
// Print float array as raw hexadecimal bit patterns
// ------------------------------------------------------------
void printMfsHex(const float a[], size_t n) {
    for (size_t j = 0; j < n; ++j) {
        unsigned int bits = 0;
        std::memcpy(&bits, &a[j], sizeof(bits));
        std::printf("%08X", bits);
        if (j + 1 < n) {
            std::printf(" ");
        }
    }
    std::printf("\n");
}

// ------------------------------------------------------------
// Print double array as raw hexadecimal bit patterns
// ------------------------------------------------------------
void printMflHex(const double a[], size_t n) {
    for (size_t j = 0; j < n; ++j) {
        unsigned long long bits = 0;
        std::memcpy(&bits, &a[j], sizeof(bits));
        std::printf("%016llX", bits);
        if (j + 1 < n) {
            std::printf(" ");
        }
    }
    std::printf("\n");
}

// ------------------------------------------------------------
// CPUID with leaf = 1
// Used to detect SSE and AVX support
// ------------------------------------------------------------
void cpuid_leaf1(unsigned int &eax, unsigned int &ebx,
                 unsigned int &ecx, unsigned int &edx) {
    eax = 1;
    asm volatile(
        "cpuid"
        : "+a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
        :
        :
    );
}

// ------------------------------------------------------------
// Read XCR0 using xgetbv
// Needed to check whether the OS enables AVX state
// ------------------------------------------------------------
unsigned long long xgetbv0() {
    unsigned int eax = 0;
    unsigned int edx = 0;
    asm volatile(
        "xgetbv"
        : "=a"(eax), "=d"(edx)
        : "c"(0)
        :
    );
    return (static_cast<unsigned long long>(edx) << 32) | eax;
}

// ------------------------------------------------------------
// Check SSE support
// ------------------------------------------------------------
bool hasSSE() {
    unsigned int eax = 0, ebx = 0, ecx = 0, edx = 0;
    cpuid_leaf1(eax, ebx, ecx, edx);
    return (edx & (1u << 25)) != 0;
}

// ------------------------------------------------------------
// Check AVX support
// ------------------------------------------------------------
bool hasAVX() {
    unsigned int eax = 0, ebx = 0, ecx = 0, edx = 0;
    cpuid_leaf1(eax, ebx, ecx, edx);

    bool avxBit = (ecx & (1u << 28)) != 0;
    if (!avxBit) {
        return false;
    }

    bool osxsaveBit = (ecx & (1u << 27)) != 0;
    if (!osxsaveBit) {
        return false;
    }

    unsigned long long xcr0 = xgetbv0();
    return (xcr0 & 0x6) == 0x6;
}

// ------------------------------------------------------------
// Read index i from a given range
// ------------------------------------------------------------
bool readIndex(size_t &i, size_t left, size_t right) {
    std::cout << "Enter i in range [" << left << ".." << right << "]: ";
    if (!(std::cin >> i) || i < left || i > right) {
        std::cout << "Input error.\n";
        return false;
    }
    return true;
}

// ------------------------------------------------------------
// TASK 1
// Write immediate value 18 into M[i] for Ms, Ml, Mq
// ------------------------------------------------------------
void task1() {
    unsigned short Ms[N] = {
        0xC0DE, 0xC0DE, 0xC0DE, 0xC0DE, 0xC0DE
    };

    unsigned int Ml[N] = {
        0xDEADBEEFU, 0xDEADBEEFU, 0xDEADBEEFU, 0xDEADBEEFU, 0xDEADBEEFU
    };

    unsigned long long Mq[N] = {
        0x000D15A550C1A7EDULL,
        0x000D15A550C1A7EDULL,
        0x000D15A550C1A7EDULL,
        0x000D15A550C1A7EDULL,
        0x000D15A550C1A7EDULL
    };

    size_t i;
    if (!readIndex(i, 0, N - 2)) {
        return;
    }

    std::printf("\nMs before change:\n");
    printMs(Ms, N);
    asm volatile(
        "movw $18, %0"
        : "=m"(Ms[i])
    );
    std::printf("Ms after change:\n");
    printMs(Ms, N);

    std::printf("\nMl before change:\n");
    printMl(Ml, N);
    asm volatile(
        "movl $18, %0"
        : "=m"(Ml[i])
    );
    std::printf("Ml after change:\n");
    printMl(Ml, N);

    std::printf("\nMq before change:\n");
    printMq(Mq, N);
    asm volatile(
        "movq $18, %0"
        : "=m"(Mq[i])
    );
    std::printf("Mq after change:\n");
    printMq(Mq, N);
}

// ------------------------------------------------------------
// TASK 2
// Variant 8 -> Ml
// Write immediate value -1 into Ml[i]
// Base address and index are passed in GPRs
// ------------------------------------------------------------
void task2() {
    unsigned int Ml[N] = {
        0xDEADBEEFU, 0xDEADBEEFU, 0xDEADBEEFU, 0xDEADBEEFU, 0xDEADBEEFU
    };

    size_t i;
    if (!readIndex(i, 0, N - 1)) {
        return;
    }

    std::printf("\nMl before change:\n");
    printMl(Ml, N);

    asm volatile(
        "movl $-1, (%[base], %[index], 4)"
        :
        : [base] "r"(Ml), [index] "r"(i)
        : "memory"
    );

    std::printf("Ml after change:\n");
    printMl(Ml, N);
}

// ------------------------------------------------------------
// TASK 3
// Variant 8 -> fifth byte of Mq[i], low byte is byte 0
// Write immediate value 0x55 using one movb
// ------------------------------------------------------------
void task3() {
    unsigned long long Mq[N] = {
        0x000D15A550C1A7EDULL,
        0x000D15A550C1A7EDULL,
        0x000D15A550C1A7EDULL,
        0x000D15A550C1A7EDULL,
        0x000D15A550C1A7EDULL
    };

    size_t i;
    if (!readIndex(i, 0, N - 1)) {
        return;
    }

    std::printf("\nMq before change:\n");
    printMq(Mq, N);

    asm volatile(
        "movb $0x55, 5(%[base], %[index], 8)"
        :
        : [base] "r"(Mq), [index] "r"(i)
        : "memory"
    );

    std::printf("Mq after change:\n");
    printMq(Mq, N);
}

// ------------------------------------------------------------
// TASK 4
// Variant 8 -> Ml
// Temporary register A -> eax
// x is passed as a memory input operand
// ------------------------------------------------------------
void task4() {
    unsigned int Ml[N] = {
        0xDEADBEEFU, 0xDEADBEEFU, 0xDEADBEEFU, 0xDEADBEEFU, 0xDEADBEEFU
    };

    volatile unsigned int x = 0x12345678U;

    size_t i;
    if (!readIndex(i, 0, N - 1)) {
        return;
    }

    std::printf("\nInitial data:\n");
    std::printf("x = %08X\n", x);
    std::printf("Ml before change:\n");
    printMl(Ml, N);

    asm volatile(
        "movl %[src], %%eax\n\t"
        "movl %%eax, (%[base], %[index], 4)"
        :
        : [src] "m"(x),
          [base] "r"(Ml),
          [index] "r"(i)
        : "eax", "memory"
    );

    std::printf("Ml after change:\n");
    printMl(Ml, N);
}

// ------------------------------------------------------------
// TASK 5
// Same as task 4, but &x is passed into the asm block
// ------------------------------------------------------------
void task5() {
    unsigned int Ml[N] = {
        0xDEADBEEFU, 0xDEADBEEFU, 0xDEADBEEFU, 0xDEADBEEFU, 0xDEADBEEFU
    };

    unsigned int x = 0x12345678U;

    size_t i;
    if (!readIndex(i, 0, N - 1)) {
        return;
    }

    std::printf("\nInitial data:\n");
    std::printf("x = %08X\n", x);
    std::printf("Ml before change:\n");
    printMl(Ml, N);

    asm volatile(
        "movl (%[px]), %%eax\n\t"
        "movl %%eax, (%[base], %[index], 4)"
        :
        : [px] "r"(&x),
          [base] "r"(Ml),
          [index] "r"(i)
        : "eax", "memory"
    );

    std::printf("Ml after change:\n");
    printMl(Ml, N);
}

// ------------------------------------------------------------
// TASK 6
// Variant 8 -> 16-bit integers
// Compute z = x + y and w = x - y
// ------------------------------------------------------------
void task6() {
    short x = 1000;
    short y = 1500;
    short z = 0;
    short w = 0;

    std::printf("\nBefore asm:\n");
    std::printf("x = %hd, y = %hd, z = %hd, w = %hd\n", x, y, z, w);

    asm volatile(
        "movw %[x], %%ax\n\t"
        "addw %[y], %%ax\n\t"
        "movw %%ax, %[z]\n\t"
        "movw %[x], %%ax\n\t"
        "subw %[y], %%ax\n\t"
        "movw %%ax, %[w]\n\t"
        : [z] "=m"(z), [w] "=m"(w)
        : [x] "m"(x), [y] "m"(y)
        : "ax", "cc"
    );

    std::printf("After asm:\n");
    std::printf("x = %hd, y = %hd, z = %hd, w = %hd\n", x, y, z, w);
}

// ------------------------------------------------------------
// TASK 7
// Variant 8 -> Mfs
// x = -3/7
// Temporary SIMD register xmm1
// If AVX is available -> use AVX only
// Otherwise use SSE only
// ------------------------------------------------------------
void task7() {
    double Mfl[N] = {
        -3.0 / 7.0, -3.0 / 7.0, -3.0 / 7.0, -3.0 / 7.0, -3.0 / 7.0
    };

    float Mfs[N] = {
        -3.0f / 7.0f, -3.0f / 7.0f, -3.0f / 7.0f, -3.0f / 7.0f, -3.0f / 7.0f
    };

    float x = -4.0f / 7.0f;

    bool sse = hasSSE();
    bool avx = hasAVX();

    size_t i;
    if (!readIndex(i, 0, N - 1)) {
        return;
    }

    std::printf("\nExtension availability:\n");
    std::printf("SSE: %s\n", sse ? "yes" : "no");
    std::printf("AVX: %s\n", avx ? "yes" : "no");

    if (!sse && !avx) {
        std::printf("Error: neither SSE nor AVX is available on this platform.\n");
        return;
    }

    std::printf("\nInitial data:\n");
    std::printf("x = %f\n", x);
    std::printf("Mfs before change:\n");
    printMfsHex(Mfs, N);

    (void)Mfl;

    if (avx) {
        std::printf("Using AVX: vmovss, register xmm1.\n");

        asm volatile(
            "vmovss %[src], %%xmm1\n\t"
            "vmovss %%xmm1, (%[base], %[index], 4)"
            :
            : [src] "m"(x),
              [base] "r"(Mfs),
              [index] "r"(i)
            : "xmm1", "memory"
        );
    } else {
        std::printf("Using SSE: movss, register xmm1.\n");

        asm volatile(
            "movss %[src], %%xmm1\n\t"
            "movss %%xmm1, (%[base], %[index], 4)"
            :
            : [src] "m"(x),
              [base] "r"(Mfs),
              [index] "r"(i)
            : "xmm1", "memory"
        );
    }

    std::printf("Mfs after change:\n");
    printMfsHex(Mfs, N);
}

// ------------------------------------------------------------
// TASK 8
// Variant 8 -> Mfs
// Convert integer x to float and store into Mfs[i]
// Use cvtsi2ss / vcvtsi2ss
// ------------------------------------------------------------
void task8() {
    double Mfl[N] = {
        -3.0 / 7.0, -3.0 / 7.0, -3.0 / 7.0, -3.0 / 7.0, -3.0 / 7.0
    };

    float Mfs[N] = {
        -3.0f / 7.0f, -3.0f / 7.0f, -3.0f / 7.0f, -3.0f / 7.0f, -3.0f / 7.0f
    };

    int x = -7;

    bool sse = hasSSE();
    bool avx = hasAVX();

    size_t i;
    if (!readIndex(i, 0, N - 1)) {
        return;
    }

    std::printf("\nExtension availability:\n");
    std::printf("SSE: %s\n", sse ? "yes" : "no");
    std::printf("AVX: %s\n", avx ? "yes" : "no");

    if (!sse && !avx) {
        std::printf("Error: neither SSE nor AVX is available on this platform.\n");
        return;
    }

    std::printf("\nInitial data:\n");
    std::printf("x = %d\n", x);
    std::printf("Mfs before change:\n");
    printMfsHex(Mfs, N);

    (void)Mfl;

    if (avx) {
        std::printf("Using AVX: vcvtsi2ssl + vmovss, register xmm1.\n");

        asm volatile(
            "vcvtsi2ssl %[src], %%xmm1, %%xmm1\n\t"
            "vmovss %%xmm1, (%[base], %[index], 4)"
            :
            : [src] "m"(x),
              [base] "r"(Mfs),
              [index] "r"(i)
            : "xmm1", "memory"
        );
    } else {
        std::printf("Using SSE: cvtsi2ssl + movss, register xmm1.\n");

        asm volatile(
            "cvtsi2ssl %[src], %%xmm1\n\t"
            "movss %%xmm1, (%[base], %[index], 4)"
            :
            : [src] "m"(x),
              [base] "r"(Mfs),
              [index] "r"(i)
            : "xmm1", "memory"
        );
    }

    std::printf("Mfs after change:\n");
    printMfsHex(Mfs, N);
}

// ------------------------------------------------------------
// Main function
// User selects task number 1..8
// ------------------------------------------------------------
int main() {
    int taskNumber;

    std::cout << "Enter Lab 3 task number (1-8): ";
    if (!(std::cin >> taskNumber)) {
        std::cout << "Input error.\n";
        return 1;
    }

    switch (taskNumber) {
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
        case 8:
            task8();
            break;
        default:
            std::cout << "You must enter a number from 1 to 8.\n";
            return 1;
    }

    return 0;
}
