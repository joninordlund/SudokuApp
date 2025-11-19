#pragma once

#include <vector>

using Matrix = std::vector<std::vector<int>>;
// using Cewll = std::tuple<int, int, int>;

// Sudokupohja:
// https://sudokupad.app/yttrio/sightline-sum-whisper-loop
class SudokuSolver
{
public:
    SudokuSolver() {}
    std::pair<Matrix, std::vector<std::tuple<int, int, int>>> makeSudokuMatrix(const Matrix &sudoku);
};
