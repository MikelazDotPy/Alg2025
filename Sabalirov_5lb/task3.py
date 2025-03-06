from __future__ import annotations


DEBUG = True


class Vertex:
    class Node:
        def __init__(self,
                     val: str,
                     parent: Node = None,
                     is_termial: bool = False) -> None:
            self.val: str = val
            self.next: dict[Node] = dict()
            self.parent: Node = parent
            self.is_terminal: bool = is_termial
            self.word: str = ""
            self.link: Node = None
            self.word_link: Node = None
            self.moves: dict[Node] = dict()

        def get_link(self) -> Node:
            if self.link is not None:
                if DEBUG:
                    print(f"Ссылка для узла '{self.val}' уже вычислена: {self.link.val}\n")
                return self.link
            if self.val is None:
                self.link = self
                if DEBUG:
                    print(f"Узел '{self.val}' является корневым, ссылка на себя\n")
            elif self.parent.val is None:
                self.link = self.parent
                if DEBUG:
                    print(f"Узел '{self.val}' является дочерним корня, ссылка на корень\n")
            else:
                if DEBUG:
                    print(f"Вычисление ссылки для узла '{self.val}' через родителя '{self.parent.val}'\n")
                self.link = self.parent.get_link().get_move(self.val)
                if DEBUG:
                    print(f"Ссылка для узла '{self.val}' установлена на '{self.link.val}'\n")
            return self.link

        def get_move(self, c: str) -> Node:
            if self.moves.get(c, None) is not None:
                if DEBUG:
                    print(f"Переход по символу '{c}' из узла '{self.val}' уже вычислен: {self.moves[c].val}\n")
                return self.moves[c]
            if self.next.get(c, None) is not None:
                self.moves[c] = self.next[c]
                if DEBUG:
                    print(f"Переход по символу '{c}' из узла '{self.val}' найден: {self.moves[c].val}\n")
            elif self.val is not None:
                if DEBUG:
                    print(f"Переход по символу '{c}' из узла '{self.val}' не найден, вычисление через ссылку\n")
                self.moves[c] = self.get_link().get_move(c)
                if DEBUG:
                    print(f"Переход по символу '{c}' из узла '{self.val}' установлен на '{self.moves[c].val}'\n")
            else:
                self.moves[c] = self
                if DEBUG:
                    print(f"Узел '{self.val}' является корневым, переход по символу '{c}' на себя\n")
            return self.moves[c]

        def get_word_link(self) -> Node:
            if self.word_link is not None:
                if DEBUG:
                    print(f"Ссылка на слово для узла '{self.val}' уже вычислена: {self.word_link.val}\n")
                return self.word_link
            link = self.get_link()
            if DEBUG:
                print(f"Вычисление ссылки на слово для узла '{self.val}' через ссылку '{link.val}'\n")
            if link.val is None or link.is_terminal:
                self.word_link = link
                if DEBUG:
                    print(f"Ссылка на слово для узла '{self.val}' установлена на '{self.word_link.val}'\n")
            else:
                if DEBUG:
                    print(f"Рекурсивное вычисление ссылки на слово для узла '{self.val}'\n")
                self.word_link = link.get_word_link()
                if DEBUG:
                    print(f"Ссылка на слово для узла '{self.val}' установлена на '{self.word_link.val}'\n")
            return self.word_link

    def __init__(self) -> None:
        self._root = self.Node(None)
        self._words = dict()
        self._len = 0

    def __len__(self) -> int:
        return self._len

    def append(self, word: str) -> None:
        if word in self._words:
            if DEBUG:
                print(f"Слово '{word}' уже добавлено, пропуск\n")
            return
        node = self._root
        if DEBUG:
            print(f"Добавление слова '{word}'\n")
        for letter in word:
            next_node = node.next.get(letter, None)
            if next_node:
                if DEBUG:
                    print(f"Переход по символу '{letter}' из узла '{node.val}'\n")
                node = next_node
            else:
                if DEBUG:
                    print(f"Создание нового узла для символа '{letter}' в узле '{node.val}'\n")
                node.next[letter] = self.Node(letter)
                node.next[letter].parent = node
                node = node.next[letter]
        node.is_terminal = True
        node.word = word
        self._len += 1
        self._words[word] = self._len
        if DEBUG:
            print(f"Слово '{word}' добавлено, терминальный узел: '{node.val}'\n")

    def extend(self, words: list[str]) -> None:
        if DEBUG:
            print(f"Добавление списка слов: {words}\n")
        for word in words:
            self.append(word)

    def find(self, text) -> list[tuple[int, int]]:
        if DEBUG:
            print(f"Поиск в тексте: '{text}'\n")
        node = self._root
        answer = []
        for i, c in enumerate(text):
            if DEBUG:
                print(f"Обработка символа '{c}' на позиции {i}\n")
            node = node.get_move(c)
            if node.val is None:
                if DEBUG:
                    print(f"Узел '{node.val}' является корневым, пропуск\n")
                continue
            if node.is_terminal:
                if DEBUG:
                    print(f"Найден терминальный узел '{node.val}' для слова '{node.word}'\n")
                answer.append((i + 2 - len(node.word), self._words[node.word]))
            u = node.get_word_link()
            while u.val is not None:
                if DEBUG:
                    print(f"Переход по ссылке на слово для узла '{u.val}'\n")
                answer.append((i + 2 - len(u.word), self._words[u.word]))
                u = u.get_word_link()
        if DEBUG:
            print(f"Результат поиска: {answer}\n")
        return sorted(answer)


def main() -> None:
    T = input()
    P = input()
    joker = input()
    batman = input()

    if DEBUG:
        print(f"Текст T: '{T}'")
        print(f"Шаблон P: '{P}'")
        print(f"Джокер: '{joker}'")
        print(f"Batman: '{batman}'\n")

    Q = []
    L = []
    buffer = ""
    for i, x in enumerate(P):
        if x == joker:
            if buffer != "":
                Q.append(buffer)
                L.append(i - len(buffer) + 1)
                if DEBUG:
                    print(f"Найдена подстрока: '{buffer}' на позиции {L[-1]}\n")
            buffer = ""
            continue
        buffer += x
    if buffer != "":
        Q.append(buffer)
        L.append(len(P) - len(buffer) + 1)
        if DEBUG:
            print(f"Найдена подстрока: '{buffer}' на позиции {L[-1]}\n")

    if DEBUG:
        print(f"Список подстрок Q: {Q}")
        print(f"Список позиций L: {L}\n")

    C = [0] * (len(T) + 1)
    for i, q in enumerate(Q):
        if DEBUG:
            print(f"Обработка подстроки '{q}' (индекс {i})\n")

        vertex = Vertex()
        vertex.append(q)

        if DEBUG:
            print(f"Автомат создан для подстроки '{q}'\n")

        for include in vertex.find(T):
            if DEBUG:
                print(f"Найдено вхождение подстроки '{q}' на позиции {include[0]}\n")

            idx = include[0] - L[i] + 1
            if idx > 0:
                C[include[0] - L[i] + 1] += 1
                if DEBUG:
                    print(f"Обновление массива C: C[{idx}] = {C[idx]}\n")

    if DEBUG:
        print(f"Массив C после обработки всех подстрок: {C}\n")

    idxs = set(map(lambda x: x - 1, L))
    if DEBUG:
        print(f"Множество индексов для проверки: {idxs}\n")
    batmans = set([i for i, x in enumerate(T) if x == batman])
    black_list = batmans - idxs
    for i in range(1, len(C)):
        if C[i] == len(Q) and i + len(P) - 1 <= len(T):
            if DEBUG:
                print(f"Проверка позиции {i} на соответствие всем подстрокам\n")
            if len(set(range(i - 1, i - 1 + len(P))) & black_list) == 0:
                if DEBUG:
                    print(f"Позиция {i} удовлетворяет всем условиям\n")
                print(i)
            elif DEBUG:
                print(f"Найден символ '{batman}' в слове {P[i - 1: i - 1 + len(P)]}, который нарушает условие\n")


if __name__ == "__main__":
    main()