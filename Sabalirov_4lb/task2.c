#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLEN 5000000
#define DEBUG 0

void prefix_func(const char *s, int *p, int len, int fact_len, int pattern_len) {
    p[0] = 0;
    for (int i = 1; i < len; i++){
        // Если текущий символ и следующий за предыдущим собст. префиксом равны,
        // то собст. префикс будет длинны пред. собст префикса + 1, т.к. символы на новых концах равны
        if (DEBUG) {
            printf("\nШаг %d:\n", i);
            printf("Текущий символ s[%d]: %c\n", i, s[i < fact_len ? i : i % fact_len + pattern_len]);
            printf("Предыдущий префикс p[%d]: %d\n", i - 1, p[i - 1]);
        }
        // Циклический просмотр символов, для экономии памяти
        int j = p[i - 1];
        if (DEBUG) {
            printf("Начальное значение j: %d\n", j);
        }

        int k = i < fact_len ? i : i % fact_len + pattern_len;
        int l = j < fact_len ? j : j % fact_len + pattern_len;
        while (j > 0 && s[k] != s[l]) {
            // Иначе стоит посмотреть префикс для пред. префиксa и если следующий символ и текущий совпадут,
            // то мы нашли искомый префикс
            if (DEBUG) {
                printf("Символы s[%d] (%c) и s[%d] (%c) не совпадают. Переход к префиксу p[%d] (%d)\n", k, s[k], l, s[l], j - 1, p[j - 1]);
            }
            j = p[j - 1];
            l = j < fact_len ? j : j % fact_len + pattern_len;
            if (DEBUG) {
                printf("Новое значение j: %d\n", j);
            }
        }

        if (s[k] == s[l]) {
            if (DEBUG) {
                printf("Символы s[%d] (%c) и s[%d] (%c) совпадают. Увеличиваем префикс на 1.\n", k, s[k], l, s[l]);
            }
            j++;
        } else {
            if (DEBUG) {
                printf("Символы s[%d] (%c) и s[%d] (%c) не совпадают. Префикс остается 0.\n", k, s[k], l, s[l]);
            }
        }

        p[i] = j;
        if (DEBUG) {
            printf("Текущий массив префиксов p: ");
            for (int m = 0; m <= i; m++) {
                printf("%d ", p[m]);
            }
            printf("\n");
        }
    }

    if (DEBUG) {
        printf("\nИтоговый массив префиксов p: ");
        for (int m = 0; m < len; m++) {
            printf("%d ", p[m]);
        }
        printf("\n");
    }
}

// Модификация для поиска циклических сдвигов (длины должны быть равны)
int kmp_cyclic(const char *B, const char *A, int len) {
    // abcdef#defabcdefabc
    // При таком задании строки s и условии остуствия сепаратора в исходных строках, очевидно, что
    // p[i] = |pattern| <=> i - (|pattern| - 1) - (|pattern| + 1) -- начало очередного вхождения pattern в text 

    int *p = malloc((3 * len + 1) * sizeof(int));
    char *C = calloc(2 * len + len + 2, sizeof(char));

    C[len] = '#';
    for (int i = 0; i < len; ++i)
        C[i] = B[i];
    for (int i = 0; i < len; ++i)
        C[i + len + 1] = A[i];

    if (DEBUG) {
        printf("\nОбъединенная строка C: %s\n", C);
        printf("Длина pattern: %d\n", len);
    }

    prefix_func(C, p, 3 * len + 1, 2 * len + 1, len + 1);

    if (DEBUG) {
        printf("\nПоиск вхождений pattern в text:\n");
    }

    int j = 0;
    for (int i = 0; i < 3 * len + 1; ++i) {
        if (p[i] == len) {
            if (DEBUG) {
                printf("Найдено вхождение на позиции: %d\n", i - 2 * len);
            }
            free(p);
            free(C);
            return i - 2 * len;
        }
    }

    if (DEBUG) {
        printf("Вхождений не найдено.\n");
    }

    free(p);
    free(C);
    return -1;
}

int main() {
    static char A[MAXLEN + 1], B[MAXLEN + 1];
    scanf("%s", A);
    scanf("%s", B);

    int len_a = strlen(A);

    if (len_a == strlen(B))
        printf("%d\n", kmp_cyclic(B, A, len_a));
    else
        printf("-1\n");

    return 0;
}