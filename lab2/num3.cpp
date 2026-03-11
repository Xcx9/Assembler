#include <stdio.h>
#include <climits>
#include <cfloat>

int main()
{
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

    return 0;
}
