#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>

// Класс SList представляет собой список указателей на строки (char*)
class SList {
private:
	std::vector<char*> list; // Вектор для хранения указателей на строки
	int comparisonCount; // Подсчет количества сравнений

public:
	// Конструктор
	SList() {
		comparisonCount = 0;
	}

	// Деструктор: освобождает память для всех строк в списке
	~SList() {
		for (char* str : list) {
			delete[] str;
		}
	}

	// Проверяет, содержит ли список указанную строку
	bool contains(const char* str) {
		for (const char* item : list) {
			comparisonCount++; // Увеличиваем счетчик сравнений при каждом сравнении
			if (strcmp(item, str) == 0) { // Если строка найдена
				return true;
			}
		}
		return false;
	}

	// Добавляет строку в конец списка
	void push_back(const char* str) {
		// Выделяем память и копируем содержимое строки
		char* newStr = new char[strlen(str) + 1];
		strcpy_s(newStr, strlen(str) + 1, str); // Используем strcpy_s, указав размер буфера
		list.push_back(newStr); // Добавляем строку в список
	}

	// Конкатенирует другой список SList в текущий список
	void concatenate(const SList& other) {
		for (const char* str : other.list) {
			push_back(str); // Добавляем каждую строку из другого списка в текущий
		}
	}

	// Получает общее количество сравнений строк
	int getComparisonCount() const {
		return comparisonCount;
	}

	// Получает количество уникальных строк (т.е. количество элементов в списке)
	int getUniqueCount() const {
		return list.size();
	}

	std::vector<char*> getList() const {
		return list;  // list - это внутренний контейнер для хранения указателей на строки
	}
};

// Класс HashTable представляет собой хеш-таблицу с разрешением коллизий методом цепочек
class HashTable {
private:
	std::vector<SList> table; // Хеш-таблица, использующая массив списков SList для разрешения коллизий
	int q; // Размер хеш-таблицы, простое число

	// Вычисляет хеш-значение строки с использованием правила Хорнера
	int hash(const char* str) {
		int hashValue = 0;
		for (int i = 0; str[i] != '\0'; i++) {
			hashValue = (hashValue * 31 + str[i]) % q; // Обновляем хеш-значение
		}
		return hashValue;
	}

public:
	// Конструктор, инициализирует хеш-таблицу и задает размер q
	HashTable(int prime) : q(prime) {
		table.resize(q); // Устанавливаем размер хеш-таблицы в q
	}

	// Добавляет уникальную строку в хеш-таблицу
	void add(const char* str) {
		int index = hash(str); // Вычисляем индекс хеш-таблицы
		// Если строка еще не содержится в списке по этому индексу, добавляем её
		if (!table[index].contains(str)) {
			table[index].push_back(str);
		}
	}

	// Конкатенирует все списки хеш-таблицы в один общий список
	SList concatenateAll() {
		SList concatenatedList;
		// Проходим по всем спискам хеш-таблицы и добавляем их в concatenatedList
		for (const SList& slist : table) {
			concatenatedList.concatenate(slist);
		}
		return concatenatedList;
	}

	// Вычисляет общее количество сравнений во всей хеш-таблице
	int getTotalComparisons() const {
		int total = 0;
		for (const SList& slist : table) {
			total += slist.getComparisonCount(); // Суммируем количество сравнений каждого списка
		}
		return total;
	}
};

// Генерирует случайную строку заданной длины
char* generateRandomString(int length) {
	char* str = new char[length + 1]; // Выделяем память для строки
	const char charset[] = "abcde"; // Набор символов
	for (int i = 0; i < length; i++) {
		str[i] = charset[rand() % (sizeof(charset) - 1)]; // Случайный выбор символа
	}
	str[length] = '\0'; // Добавляем завершающий нулевой символ
	return str;
}

int main() {
	srand(static_cast<unsigned>(time(0))); // Инициализируем генератор случайных чисел

	int n = 1000; // Количество строк
	int strLength = 5; // Длина каждой строки
	int primeQ = 1009; // Выбираем простое число в качестве размера хеш-таблицы

	// Инициализируем хеш-таблицу
	HashTable hashTable(primeQ);

	// Генерируем случайные строки и добавляем их в хеш-таблицу
	for (int i = 0; i < n; i++) {
		char* randomString = generateRandomString(strLength); // Генерируем случайную строку
		hashTable.add(randomString); // Добавляем в хеш-таблицу
		delete[] randomString; // Освобождаем память для строки
	}

	// Получаем уникальные строки и статистику
	SList uniqueList = hashTable.concatenateAll();            // Получаем все уникальные строки
	int uniqueCount = uniqueList.getUniqueCount();            // Считаем количество уникальных строк
	int totalComparisons = hashTable.getTotalComparisons();   // Получаем общее количество сравнений строк

	// Выводим все уникальные строки
	std::cout << "Unique strings: " << std::endl;
	for (const char* str : uniqueList.getList()) {
		std::cout << str << std::endl;  // Выводим каждую уникальную строку
	}

	// Выводим результаты
	std::cout << "Unique strings count: " << uniqueCount << std::endl;
	std::cout << "Total string comparisons: " << totalComparisons << std::endl;

	return 0;
}
