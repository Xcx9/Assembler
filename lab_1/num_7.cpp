#include <stdio.h>

int main()
{
    /* Буферы */
    char s1[64];
    char s2[16];   /* k = 16 -> вводим не более 15 символов */
    char s3[128];  /* для строки с пробелами */

    /* a) слово s1 (без пробелов) — одним scanf */
    if (scanf("%s", s1) != 1) {
        puts("Input error for s1");
        return 0;
    }

    /* b) слово s2 с ограничением по ширине (k-1) — одним scanf */
    if (scanf("%15s", s2) != 1) {
        puts("Input error for s2");
        return 0;
    }

    /* c) строка s3 с пробелами: scanset до '\n' — одним scanf
       Ведущий пробел в формате пропускает предыдущие переводы строки. */
    if (scanf(" %127[^\n]", s3) != 1) {
        puts("Input error for s3");
        return 0;
    }

    /* Вывод: "***s***" */
    printf("***%s***\n", s1);
    printf("***%s***\n", s2);
    printf("***%s***\n", s3);

    return 0;
}
