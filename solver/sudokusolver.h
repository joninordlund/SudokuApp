#pragma once

#include <vector>
using Matrix = std::vector<std::vector<int>>;
using Cell = std::tuple<int, int, int>;

// Sudokupohja:
// https://sudokupad.app/yttrio/sightline-sum-whisper-loop
class SudokuSolver
{
public:
    SudokuSolver() {}
    std::pair<Matrix, std::vector<Cell>> makeSudokuMatrix(const Matrix &sudoku);
};