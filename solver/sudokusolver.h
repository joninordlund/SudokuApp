#pragma once

#include <vector>

using namespace std;
using Matrix = vector<vector<int>>;

class SudokuSolver
{
public:
    SudokuSolver() {}
    vector<Matrix> getSolutions(const Matrix &grid);

private:
    pair<Matrix, vector<tuple<int, int, int>>> makeSudokuMatrix(const Matrix &sudoku);
};
