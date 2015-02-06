//
//  main.cpp
//  Sudoku
//
//  Created by Estanislau Pujadas on 2/5/15.
//  Copyright (c) 2015 stan. All rights reserved.
//

#include <iostream>
#include <stack>
#include <vector>
#include <array>
#include <algorithm>

class Cell {
    std::vector<int> availableValues;

public:
    int rowIndex;
    int colindex;
    Cell (const std::array<bool, 10>, int, int);
    bool emptyValues();
    int popValue();
};

Cell::Cell(const std::array<bool, 10> possibilities, int rowIndex, int colIndex) {
    this->rowIndex = rowIndex;
    this->colindex = colIndex;
    
    for (int i = 1; i < 10; i++) {
        if (possibilities[i]) {
            availableValues.push_back(i);
        }
    }
}

bool Cell::emptyValues() {
    return availableValues.size() == 0;
}

int Cell::popValue() {
    if (emptyValues()) {
        throw "Cell does not contain more values!!";
    }
    int value = availableValues.back();
    availableValues.pop_back();
    return value;
}

class Sudoku {
    int matrix[9][9];
    std::stack<Cell> stack;

public:
    Sudoku (int[9][9]);
    void get_row(std::array<int, 9> &row, int index);
    void get_column(std::array<int, 9>  &col, int index);
    void get_square(std::array<int, 9>  &square, int rowIndex, int colIndex);
    int calculate_possibilities(std::array<bool, 10>  &possibilities, int rowIndex, int colIndex);
    void solve_cell(int rowIndex, int colIndex);
    void set_cell_value(const std::array<bool, 10>  possibilities, int rowIndex, int colIndex);
    void backtrack();
    void show();
    void solve();
    bool isSolved();
    bool completeArray(const std::array<int, 9>);
};

Sudoku::Sudoku (int m[9][9]) {
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            matrix[y][x] = m[y][x];
        }
    }
}

void Sudoku::get_row(std::array<int, 9> &row, int index) {
    for (int i = 0; i < 9; i++) {
        row[i] = matrix[index][i];
    }
}

void Sudoku::get_column(std::array<int, 9> &col, int index) {
    for (int y = 0; y < 9; y++) {
        col[y] = matrix[y][index];
    }
}

void Sudoku::get_square(std::array<int, 9> &square, int rowIndex, int colIndex) {
    int lowRowIndex = (rowIndex / 3)*3;
    int highRowIndex = (1 + (rowIndex / 3)) * 3;
    int lowColIndex = (colIndex / 3)*3;
    int highColIndex = (1 + (colIndex / 3)) * 3;
    int i = 0;

    for (int y = lowRowIndex; y < highRowIndex; y++) {
        for (int x = lowColIndex; x < highColIndex; x++) {
            square[i] = matrix[y][x];
            i++;
        }
    }
}

int Sudoku::calculate_possibilities(std::array<bool, 10> &possibilities, int rowIndex, int colIndex) {
    for (int i = 0; i < 10; i++) {
        possibilities[i] = true;
    }
    
    std::array<int, 9> row;
    std::array<int, 9> column;
    std::array<int, 9> square;
    int counter = 0;

    get_row(row, rowIndex);
    get_column(column, colIndex);
    get_square(square, rowIndex, colIndex);
    for (int i = 0; i < 9; i++) {
        possibilities[row[i]] = false;
        possibilities[column[i]] = false;
        possibilities[square[i]] = false;
    }
    
    // We do not consider value 0 to be a possibility
    for (int i = 1; i < 10; i++) {
        if (possibilities[i]) {
            counter++;
        }
    }
    return counter;
}

void Sudoku::solve_cell(int rowIndex, int colIndex) {
    if (matrix[rowIndex][colIndex] != 0) {
        return;
    }
    std::array<bool, 10> possibilities;
    int lenPossibilities = calculate_possibilities(possibilities, rowIndex, colIndex);
    switch (lenPossibilities) {
        case 0:
            backtrack();
            break;
            
        default:
            set_cell_value(possibilities, rowIndex, colIndex);
            break;
    }
}

void Sudoku::backtrack() {
    while (!stack.empty()) {
        auto cell = stack.top();
        stack.pop();
        matrix[cell.rowIndex][cell.colindex] = 0;
        std::cout << "Bactrack cell: " << cell.rowIndex << ", " << cell.colindex << "\n";
        if (!cell.emptyValues()) {
            int value = cell.popValue();
            std::cout << "Bactrack cell with value: " << cell.rowIndex << ", " << cell.colindex << " : " << value << "\n";
            matrix[cell.rowIndex][cell.colindex] = value;
            stack.push(cell);
            return;
        }
    }
}

void Sudoku::set_cell_value(const std::array<bool, 10> possibilities, int rowIndex, int colIndex){
    Cell cell (possibilities, rowIndex, colIndex);
    
    int value = cell.popValue();
    stack.push(cell);
    matrix[rowIndex][colIndex] = value;
    std::cout << "Set cell: " << rowIndex << ", " << colIndex << " : " << value << "\n";
}

void Sudoku::show() {
    std::cout << "\n";
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            std::cout << matrix[y][x] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Sudoku::solve() {
    int iterations = 100;

    while (iterations > 0 && !isSolved()) {
        for (int y = 0; y < 9; y++) {
            for (int x = 0; x < 9; x++) {
                solve_cell(y, x);
            }
        }
        iterations --;
    }
}

bool Sudoku::isSolved() {
    std::array<int, 9> row;
    std::array<int, 9> column;
    std::array<int, 9> square;
    bool solved = true;
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            get_row(row, y);
            get_column(column, x);
            get_square(square, y, x);
            solved = solved && completeArray(row);
            solved = solved && completeArray(column);
            solved = solved && completeArray(square);
        }
    }
    return solved;
}

bool Sudoku::completeArray(const std::array<int, 9> array) {
    bool completed[10] = {false, false, false, false, false, false, false, false, false, false};
    for (int i = 0; i < 9; i++) {
        completed[array[i]] = true;
    }
    if (completed[0]) {
        return false;
    }
    for (int i = 1; i < 10; i++) {
        if (!completed[i]) {
            return false;
        }
    }
    return true;
}


int main(int argc, const char * argv[]) {

    int matrix[9][9] = {
        {1, 2, 3, 4, 5, 6, 7, 8, 9},
        {4, 5, 6, 7, 8, 9, 0, 0, 3},
        {7, 8, 9, 1, 2, 3, 0, 0, 6},
        
        {0, 0, 0, 0, 6, 4, 8, 0, 0},
        {5, 0, 0, 0, 0, 0, 2, 0, 0},
        {8, 0, 0, 0, 0, 0, 5, 6, 4},
        
        {3, 1, 0, 0, 4, 5, 9, 7, 8},
        {0, 0, 0, 9, 7, 8, 0, 0, 0},
        {9, 0, 0, 3, 1, 0, 0, 0, 0},
    };
    
   
    Sudoku sudoku(matrix);
    std::cout << "Solved: " << sudoku.isSolved();
    sudoku.show();
    sudoku.solve();
    sudoku.show();
    std::cout << "Solved: " << sudoku.isSolved();
    
    return 0;
}

