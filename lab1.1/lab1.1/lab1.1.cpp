#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <chrono>

// Класс Point, представляет точку на плоскости
class Point {
public:
	int x, y;

	// Конструктор для случайного создания точки
	Point() : x(rand() % 1000), y(rand() % 1000) {}

	// Конструктор, который инициализирует координаты
	Point(int x, int y) : x(x), y(y) {}

	// Оператор сравнения, сортировка по X-координате, если X равны, то по Y
	bool operator<(const Point& other) const {
		if (x == other.x) {
			return y < other.y;
		}
		return x < other.x;
	}

	// Оператор "больше"
	bool operator>(const Point& other) const {
		if (x == other.x) {
			return y > other.y;
		}
		return x > other.x;
	}

	// Вывод точки
	friend std::ostream& operator<<(std::ostream& os, const Point& p) {
		os << "(" << p.x << ", " << p.y << ")";
		return os;
	}
};

// Шаблонный класс Sort, для реализации алгоритмов сортировки
template <typename T>
class Sort {
public:
	// получать и возвращать массивы указателей
	// Сортировка Шелла, возвращает отсортированный массив указателей
	static T** shellSort(T** arr, int size) {
		T** sortedArr = arr; // Работаем с переданным массивом указателей

		for (int gap = size / 2; gap > 0; gap /= 2) {
			for (int i = gap; i < size; i++) {
				T* temp = sortedArr[i];
				int j;
				for (j = i; j >= gap && *sortedArr[j - gap] > *temp; j -= gap) {
					sortedArr[j] = sortedArr[j - gap];
				}
				sortedArr[j] = temp;
			}
		}

		return sortedArr; // Возвращаем отсортированный массив указателей
	}

	// Пирамидальная сортировка, возвращает отсортированный массив указателей
	static void heapify(T** arr, int n, int i) {//пирамида(堆)
		int largest = i;
		int left = 2 * i + 1;
		int right = 2 * i + 2;

		if (left < n && *arr[left] > *arr[largest])//Значение родительского узла больше значения дочернего узла
			largest = left;

		if (right < n && *arr[right] > *arr[largest])
			largest = right;

		if (largest != i) {
			std::swap(arr[i], arr[largest]);
			heapify(arr, n, largest);
		}
	}

	static T** heapSort(T** arr, int n) {
		T** sortedArr = arr; // Работаем с переданным массивом указателей

		// Построение кучи
		for (int i = n / 2 - 1; i >= 0; i--)
			heapify(sortedArr, n, i);

		// Извлечение элементов из кучи
		for (int i = n - 1; i >= 0; i--) {
			std::swap(sortedArr[0], sortedArr[i]);
			heapify(sortedArr, i, 0);
		}

		return sortedArr; // Возвращаем отсортированный массив указателей
	}

	// Проверка, отсортирован ли массив 
	static bool isSorted(T** arr, int size) {
		for (int i = 1; i < size; i++) {
			if (*arr[i - 1] > *arr[i]) {
				return false;
			}
		}
		return true;
	}
};

int main() {
	srand(static_cast<unsigned int>(time(0)));  // Инициализация случайного генератора

	const int size = 10;  // Задать размер массива

	// Создаем динамические объекты Point и сохраняем их в массив указателей
	Point* pointArray[size];
	for (int i = 0; i < size; ++i) {
		pointArray[i] = new Point();
	}

	// Вывод случайно созданных точек
	std::cout << "Случайно созданные точки:" << std::endl;
	for (int i = 0; i < size; ++i) {
		std::cout << *pointArray[i] << std::endl;
	}

	// Сортировка объектов Point с помощью сортировки Шелла
	auto start = std::chrono::high_resolution_clock::now();
	Point** sortedShell = Sort<Point>::shellSort(pointArray, size);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	std::cout << "Время сортировки Шелла: " << elapsed.count() << " секунд" << std::endl;

	// Проверка, отсортирован ли массив
	if (Sort<Point>::isSorted(sortedShell, size)) {
		std::cout << "Массив Point был успешно отсортирован с помощью сортировки Шелла." << std::endl;
	}
	else {
		std::cout << "Массив Point не был отсортирован правильно." << std::endl;
	}

	// Заново случайным образом инициализируем данные точек (для повторной сортировки)
	for (int i = 0; i < size; ++i) {
		delete pointArray[i]; // Удаляем старые объекты
		pointArray[i] = new Point(); // Создаем новые случайные точки
	}

	// Сортировка объектов Point с помощью пирамидальной сортировки
	start = std::chrono::high_resolution_clock::now();
	Point** sortedHeap = Sort<Point>::heapSort(pointArray, size);
	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;
	std::cout << "Время пирамидальной сортировки: " << elapsed.count() << " секунд" << std::endl;

	// Проверка, отсортирован ли массив
	if (Sort<Point>::isSorted(sortedHeap, size)) {
		std::cout << "Массив Point был успешно отсортирован с помощью пирамидальной сортировки." << std::endl;
	}
	else {
		std::cout << "Массив Point не был отсортирован правильно." << std::endl;
	}

	// Создание трех массивов целых чисел для сортировки: по возрастанию, убыванию и случайного массива
	int* ascArray[size];
	int* descArray[size];
	int* randArray[size];

	for (int i = 0; i < size; ++i) {
		ascArray[i] = new int(i);  // Массив по возрастанию
		descArray[i] = new int(size - i);  // Массив по убыванию
		randArray[i] = new int(rand() % 1000);  // Случайный массив
	}

	// Сортировка случайного массива с помощью сортировки Шелла
	start = std::chrono::high_resolution_clock::now();
	Sort<int>::shellSort(randArray, size);
	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;
	std::cout << "Время сортировки Шелла для случайного массива: " << elapsed.count() << " секунд" << std::endl;

	// Сортировка массива по убыванию с помощью пирамидальной сортировки
	start = std::chrono::high_resolution_clock::now();
	Sort<int>::heapSort(descArray, size);
	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;
	std::cout << "Время пирамидальной сортировки для массива по убыванию: " << elapsed.count() << " секунд" << std::endl;

	// Освобождение динамически выделенной памяти
	for (int i = 0; i < size; ++i) {
		delete pointArray[i];
		delete ascArray[i];
		delete descArray[i];
		delete randArray[i];
	}

	return 0;
}
