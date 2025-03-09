SEP_SYMBOL = "#"
DEBUG = False


def prefix_func(s: str) -> list[int]:
    p = [0]*len(s)
    for i in range(1, len(s)):
        """
        Если текущий символ и следующий за предыдущим собст. префиксом равны,
        то собст. префикс будет длинны пред. собст префикса + 1, т.к. символы на новых концах равны
        """
        if DEBUG:
            print(f"\nШаг {i}:")
            print(f"Строка s: {s}")
            print(f"Текущий символ s[i]: {s[i]}")
            print(f"Предыдущий префикс p[i-1]: {p[i-1]}")

        j = p[i - 1]
        if DEBUG:
            print(f"Начальное значение j: {j}")

        while j > 0 and s[i] != s[j]:
            """
            Иначе стоит посмотреть префикс для пред. префиксa и если следующий символ и текущий совпадут,
            то мы нашли искомый префикс
            """
            if DEBUG:
                print(f"Символы s[i] ({s[i]}) и s[j] ({s[j]}) не совпадают. Переход к префиксу p[j-1] ({p[j-1]})")
            j = p[j - 1]
            if DEBUG:
                print(f"Новое значение j: {j}")

        if (s[i] == s[j]):
            if DEBUG:
                print(f"Символы s[i] ({s[i]}) и s[j] ({s[j]}) совпадают. Увеличиваем префикс на 1.")
            j += 1
        else:
            if DEBUG:
                print(f"Символы s[i] ({s[i]}) и s[j] ({s[j]}) не совпадают. Префикс остается 0.")
        p[i] = j

        if DEBUG:
            print(f"Текущий массив префиксов p: {p}")

    if DEBUG:
        print(f"\nИтоговый массив префиксов p: {p}")
    return p


def kmp(pattern: str, text: str) -> list[int]:
    """
    ab#aab
    При таком задании строки s и условии остуствия сепаратора в исходных строках, очевидно, что
    p[i] = |pattern| <=> i - (|pattern| - 1) - (|pattern| + 1) -- начало очередного вхождения pattern в text 
    """
    s = pattern + SEP_SYMBOL + text
    if DEBUG:
        print(f"\nОбъединенная строка s: {s}")
        print(f"Длина pattern: {len(pattern)}")

    p = prefix_func(s)

    if DEBUG:
        print("\nПоиск вхождений pattern в text:")

    ans = [
        i - 2*len(pattern)
        for i, x in enumerate(p) if x == len(pattern)
    ]

    if DEBUG:
        print(f"Найденные индексы вхождений: {ans if ans else [-1]}")

    return ans if ans else [-1]


def main() -> None:
    pattern = input()
    text = input()
    print(",".join(map(str, kmp(pattern, text))))


if __name__ == "__main__":
    main()