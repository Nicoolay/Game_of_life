#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>

bool comparison(std::string** arr, std::string** temp, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (arr[i][j] != temp[i][j])
                return false;
        }
    }
    return true;
} //сравнение основного массива и временного

int count_alive_cells(std::string** arr, int rows, int cols) {
    int live_cells = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (arr[i][j] == "*") {
                ++live_cells;
            }
        }
    }
    return live_cells;
} // считаем живые клетки

int count_alive_neighbor(std::string** arr, int i, int j, int rows, int cols) {
    int alive_neighbor = 0;

    if (i > 0) {
        if (arr[i - 1][j] == "*")
            alive_neighbor++;
    }
    if (i < rows - 1) {
        if (arr[i + 1][j] == "*")
            alive_neighbor++;
    }
    if (j > 0) {
        if (arr[i][j - 1] == "*")
            alive_neighbor++;
    }
    if (j < cols - 1) {
        if (arr[i][j + 1] == "*")
            alive_neighbor++;
    }
    //углы
    if (i > 0 && j > 0) { // лево верх
        if (arr[i - 1][j - 1] == "*")
            alive_neighbor++;
    }
    if (i > 0 && j < cols - 1) { // право верх
        if (arr[i - 1][j + 1] == "*")
            alive_neighbor++;
    }
    if (i < rows - 1 && j > 0) { // низ лево
        if (arr[i + 1][j - 1] == "*")
            alive_neighbor++;
    }
    if (i < rows - 1 && j < cols - 1) { // низ право
        if (arr[i + 1][j + 1] == "*")
            alive_neighbor++;
    }

    return alive_neighbor;
}
//считаем количество живых соседей

void print_arr(std::string** arr, int rows, int cols, int iter) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << arr[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "Количество живых клеток: " << count_alive_cells(arr, rows, cols) << " Количество циклов: " << iter << std::endl;
} // печать массива

void change_cell(std::string** arr, std::string** temp, int i, int j, int rows, int cols) {

    if (arr[i][j] == "*") {
        if (count_alive_neighbor(arr, i, j, rows, cols) < 2 || count_alive_neighbor(arr, i, j, rows, cols) > 3) {
            temp[i][j] = "-";
        }
        else {
            temp[i][j] = "*";
        }
    }
    else if (arr[i][j] == "-") {
        if (count_alive_neighbor(arr, i, j, rows, cols) == 3) {
            temp[i][j] = "*";
        }
        else {
            temp[i][j] = "-";
        }
    }
}

int main(int argc, char* argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::ifstream file("in.txt");
    int rows, cols;
    file >> rows;
    file >> cols;

    std::string** arr = new std::string * [rows]; // Создаём основной массив
    std::string** temp = new std::string * [rows]; // и для времнного хранения
    for (int i = 0; i < rows; i++) {
        arr[i] = new std::string[cols];
        for (int j = 0; j < cols; j++) {
            arr[i][j] = "-";
        }
    } // Заполняю его "-"

    int iter = 1;
    int i1, j1;
    while (file >> i1 >> j1) {
        if (i1 >= 0 && i1 < rows && j1 >= 0 && j1 < cols) {
            arr[i1][j1] = "*";
        }
    } // Загружаю данные из файла в массив и меняю с "-" на "*" в соответсвии
      // адресам из файла
    file.close();

    for (int i = 0; i < rows; i++) {
        temp[i] = new std::string[cols];
        for (int j = 0; j < cols; j++) {
            temp[i][j] = arr[i][j];
        }
    }


    do {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                arr[i][j] = temp[i][j];
            }
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                change_cell(arr, temp, i, j, rows, cols);
            }
        }
        print_arr(arr, rows, cols, iter);
        iter++;
        Sleep(1000);


    } while (count_alive_cells(arr, rows, cols) != 0 && comparison(arr, temp, rows, cols) == false);


    if (count_alive_cells(arr, rows, cols) == 0) // проверка условий игры
        std::cout << "Живых клеток не осталось! Конец игры!\n " << std::endl;
    else if (comparison(arr, temp, rows, cols) == 1)
        std::cout << "Стагнация! Конец игры!\n" << std::endl;

    for (int i = 0; i < rows;
        i++) { // очистка памяти, в функцию выводить отдельно не посчитал нужным.
        delete[] arr[i];
        delete[] temp[i];
    }
    delete[] arr;
    delete[] temp;

    return 0;
}