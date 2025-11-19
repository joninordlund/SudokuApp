#include "sudokusolver.h"
#include <vector>
#include <iostream>

std::pair<Matrix, std::vector<std::tuple<int, int, int>>> SudokuSolver::makeSudokuMatrix(const Matrix &sudoku)
{
    const int M = 9;

    Matrix mat;
    mat.reserve(M * M * M);

    std::vector<std::tuple<int, int, int>> rowMapping;

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            for (int n = 1; n <= M; n++)
            {
                std::vector<int> row;
                row.resize(4 * M * M);
                if (sudoku[i][j] == 0 || sudoku[i][j] == n)
                {
                    // constraints
                    int box = (i / 3) * 3 + (j / 3);
                    row[i * M + j] = 1;                     // cell
                    row[M * M + i * M + (n - 1)] = 1;       // row
                    row[2 * M * M + j * M + (n - 1)] = 1;   // column
                    row[3 * M * M + box * M + (n - 1)] = 1; // box

                    mat.push_back(std::move(row));
                    rowMapping.emplace_back(i, j, n);
                }
            }
        }
    }
    auto res = std::make_pair(mat, rowMapping);
    return res;
}