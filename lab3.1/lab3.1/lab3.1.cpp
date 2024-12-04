#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <set>
#include <random>

using namespace std;

// Определение структуры точки
struct Point {
	double x, y; // Координаты точки
};

// Определение структуры ребра
struct Edge {
	int u, v;       // Два конца ребра
	double weight;  // Вес ребра 
	// Оператор сравнения для сортировки по весу
	bool operator<(const Edge& other) const {
		return weight < other.weight;
	}
};

// Вычисление евклидова расстояния между двумя точками
double distance(const Point& a, const Point& b) {
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// Класс объединенных наборов (для проверки связности)
class UnionFind {
public:
	vector<int> parent, rank; // parent - родительские узлы, rank - ранг (глубина дерева)

	// Инициализация объединенных наборов, каждый узел является своим родителем, ранг равен 0
	UnionFind(int n) : parent(n), rank(n, 0) {
		iota(parent.begin(), parent.end(), 0); // Инициализация родительских узлов от 0 до n-1
	}

	// Поиск корня для узла (сжатие пути)
	int find(int x) {
		if (parent[x] != x) {
			parent[x] = find(parent[x]);
		}
		return parent[x];
	}

	// Объединение двух множеств
	void unite(int x, int y) {
		int rootX = find(x);
		int rootY = find(y);
		if (rootX != rootY) { // Если два узла находятся в разных множествах
			if (rank[rootX] > rank[rootY]) {
				parent[rootY] = rootX;
			}
			else if (rank[rootX] < rank[rootY]) {
				parent[rootX] = rootY;
			}
			else {
				parent[rootY] = rootX;
				rank[rootX]++;
			}
		}
	}
};

// Генерация случайных точек
vector<Point> generateRandomPoints(int n) {
	vector<Point> points;
	random_device rd;                  // Генератор случайных чисел
	mt19937 gen(rd());                 // Двигатель случайных чисел
	uniform_real_distribution<> dis(0, 100); // Равномерное распределение в пределах [0, 100]

	for (int i = 0; i < n; i++) {
		points.push_back({ dis(gen), dis(gen) }); // Генерация случайных точек
	}
	return points;
}

// Алгоритм Краскала для построения минимального остова
vector<Edge> kruskalMST(int n, const vector<Edge>& edges) {
	UnionFind uf(n);       // Инициализация объединенных наборов
	vector<Edge> mst;      // Список рёбер минимального остова

	for (const auto& edge : edges) {
		// Если два конца не связаны, добавляем ребро в остов
		if (uf.find(edge.u) != uf.find(edge.v)) {
			mst.push_back(edge);
			uf.unite(edge.u, edge.v); // Объединяем множества
		}
	}
	return mst;
}

// Разделение на компоненты связности по N-K рёбрам
vector<set<int>> findConnectedComponents(int n, const vector<Edge>& edges) {
	UnionFind uf(n); // Инициализация объединенных наборов
	for (const auto& edge : edges) {
		uf.unite(edge.u, edge.v); // Объединение узлов, соединённых ребром
	}

	// Поиск каждой компоненты связности
	vector<set<int>> components(n);
	for (int i = 0; i < n; i++) {
		components[uf.find(i)].insert(i); // Добавление узла в соответствующую компоненту
	}

	// Удаление пустых компонентов
	vector<set<int>> result;
	for (auto& comp : components) {
		if (!comp.empty()) {
			result.push_back(comp);
		}
	}
	return result;
}

// Анализ свойств каждой компоненты связности
void analyzeComponents(const vector<set<int>>& components, const vector<Point>& points) {
	for (const auto& comp : components) {
		int size = comp.size(); // Размер компоненты (количество точек)
		double minX = INFINITY, maxX = -INFINITY; // Минимальное и максимальное значения по X
		double minY = INFINITY, maxY = -INFINITY; // Минимальное и максимальное значения по Y
		double centroidX = 0, centroidY = 0;     // Координаты центра масс

		for (int v : comp) {
			minX = min(minX, points[v].x);
			maxX = max(maxX, points[v].x);
			minY = min(minY, points[v].y);
			maxY = max(maxY, points[v].y);
			centroidX += points[v].x;
			centroidY += points[v].y;
		}

		centroidX /= size;
		centroidY /= size;

		cout << "Размер компоненты: " << size << endl;
		cout << "Ограничивающий прямоугольник: [" << minX << ", " << maxX << "] x [" << minY << ", " << maxY << "]" << endl;
		cout << "Центроид: (" << centroidX << ", " << centroidY << ")" << endl;
		cout << endl;
	}
}

int main() {
	int N, K;
	cout << "Введите количество точек (N): ";
	cin >> N; // Ввод количества точек
	cout << "Введите количество кластеров (K): ";
	cin >> K; // Ввод количества кластеров

	// Шаг 1: Генерация случайных точек
	vector<Point> points = generateRandomPoints(N);

	// Шаг 2: Строительство полного взвешенного графа с рёбрами
	vector<Edge> edges;
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			edges.push_back({ i, j, distance(points[i], points[j]) }); // Расстояние между точками как вес рёбер
		}
	}

	// Шаг 3: Сортировка рёбер по весу
	sort(edges.begin(), edges.end());

	// Шаг 4: Применение алгоритма Краскала для извлечения минимального остова
	vector<Edge> mst = kruskalMST(N, edges);

	// Шаг 5: Сортировка рёбер минимального остова по весу
	sort(mst.begin(), mst.end());

	// Шаг 6: Выбор N-K рёбер для формирования кластеров
	vector<Edge> reducedEdges(mst.begin(), mst.begin() + (N - K));

	// Шаг 7: Нахождение всех компонент связности
	vector<set<int>> components = findConnectedComponents(N, reducedEdges);

	// Шаг 8: Анализ свойств каждой компоненты
	analyzeComponents(components, points);

	return 0;
}

