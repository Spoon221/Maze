#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <random>

using namespace std;

// Структура для хранения координат
struct Coordinate {
    int row;
    int col;
};

// Функция для проверки, находится ли клетка в пределах лабиринта
bool isInsideMaze(const vector<vector<char>>& maze, int row, int col) {
    return row >= 0 && row < maze.size() && col >= 0 && col < maze[0].size();
}

// Функция для поиска кратчайшего пути в лабиринте
bool findShortestPath(vector<vector<char>>& maze, Coordinate start, Coordinate end) {
    // Если стартовая клетка - препятствие, то пути нет
    if (maze[start.row][start.col] == 'x') {
        return false;
    }

    // Используем BFS, чтобы найти кратчайший путь
    queue<pair<Coordinate, vector<Coordinate>>> q;  // Храним координату и путь
    q.push({ start, { start } }); // Добавляем стартовую точку в очередь

    // Отметка посещенных клеток
    vector<vector<bool>> visited(maze.size(), vector<bool>(maze[0].size(), false));
    visited[start.row][start.col] = true;

    // Пока очередь не пуста
    while (!q.empty()) {
        Coordinate current = q.front().first;
        vector<Coordinate> path = q.front().second; // Получаем путь из очереди
        q.pop();

        // Если мы достигли финиша, то путь найден
        if (current.row == end.row && current.col == end.col) {
            // Вывод кратчайшего пути
            for (const auto& coord : path) {
                maze[coord.row][coord.col] = '*'; // Заменяем клетки пути на '*'
            }
            return true;
        }

        // Проверяем все четыре соседние клетки
        vector<Coordinate> directions = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
        for (const Coordinate& direction : directions) {
            int nextRow = current.row + direction.row;
            int nextCol = current.col + direction.col;

            // Если клетка находится в пределах лабиринта, свободна и не посещалась
            if (isInsideMaze(maze, nextRow, nextCol) && maze[nextRow][nextCol] != 'x' && !visited[nextRow][nextCol]) {
                // Отмечаем клетку как посещенную
                visited[nextRow][nextCol] = true;
                // Добавляем клетку в очередь
                vector<Coordinate> newPath = path; // Создаем копию пути
                newPath.push_back({ nextRow, nextCol }); // Добавляем новую клетку в путь
                q.push({ { nextRow, nextCol }, newPath }); // Добавляем клетку и путь в очередь
            }
        }
    }

    // Если ни один из путей не привел к успеху, то возвращаем false
    return false;
}

int main() {
    setlocale(LC_ALL, "Russian");
    // Открываем файл input.txt для чтения
    ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        cerr << "Ошибка при открытии файла input.txt" << endl;
        return 1;
    }

    // Считываем лабиринт из файла
    vector<vector<char>> maze;
    string line;
    while (getline(inputFile, line)) {
        maze.push_back(vector<char>(line.begin(), line.end()));
    }
    inputFile.close();

    // Находим координаты стартовой и финишной клеток
    Coordinate start, end;
    for (int i = 0; i < maze.size(); ++i) {
        for (int j = 0; j < maze[i].size(); ++j) {
            if (maze[i][j] == 'S') {
                start = { i, j };
            }
            else if (maze[i][j] == 'F') {
                end = { i, j };
            }
        }
    }

    // Пробуем найти кратчайший путь до 20 раз
    for (int attempt = 0; attempt < 20; ++attempt) {
        // Сбрасываем состояние лабиринта перед каждой попыткой
        for (int i = 0; i < maze.size(); ++i) {
            for (int j = 0; j < maze[i].size(); ++j) {
                if (maze[i][j] == '*') {
                    maze[i][j] = '.'; // Восстанавливаем пустые клетки
                }
            }
        }

        // Ищем кратчайший путь
        if (findShortestPath(maze, start, end)) {
            maze[start.row][start.col] = 'S';
            maze[end.row][end.col] = 'F';
            for (const auto& row : maze) {
                for (const auto& cell : row) {
                    cout << cell;
                }
                cout << endl;
            }
            return 0;
        }
    }

    // Путь не найден после 20 попыток
    cout << "Пути нет." << endl;

    return 0;
}