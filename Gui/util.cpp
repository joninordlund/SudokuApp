#include "DLXBuilder.h"
#include "sudokusolver.h"
#include "util.h"

vector<Matrix> Util::convertSolutions(const Matrix &grid)
{
    SudokuSolver solver;
    std::pair<Matrix, vector<std::tuple<int, int, int>>> sudoku = solver.makeSudokuMatrix(grid);
    DLXBuilder dlx(sudoku.first);
    auto solutions = dlx.findSolutions();

    Matrix solved(9, vector<int>(9, 0));
    vector<Matrix> res;
    for (int r = 0; r < solutions.size(); r++)
    {
        for (int rowID : solutions[r])
        {
            auto [i, j, n] = sudoku.second[rowID - 1];
            solved[i][j] = n;
        }
        res.push_back(solved);
    }
    return res;
}