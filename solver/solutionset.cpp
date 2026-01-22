#include "solutionset.h"

#include <algorithm>
#include <numeric>
#include <random>

void SolutionSet::updateSolutions(const Matrix& matrix, int maxCount)
{
    m_maxSolCount = maxCount;
    m_solutions = m_solver.getSolutions(matrix, false, maxCount);
}

vector<Matrix> SolutionSet::getRandomSudokuSet(const Matrix& sudoku, int count)
{
    return m_solver.getRandomSudokuSet(sudoku, count);
}

Matrix SolutionSet::next()
{
    if (m_solutions.empty())
    {
        return Matrix();
    }

    m_index = (m_index + 1) % m_solutions.size();
    return m_solutions[m_index];
}

Matrix SolutionSet::prev()
{
    if (m_solutions.empty())
    {
        return Matrix();
    };

    m_index = (m_index - 1 + m_solutions.size()) % m_solutions.size();
    return m_solutions[m_index];
}

Matrix SolutionSet::generate(int cellCount)
{
    Matrix nullMatrix;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            nullMatrix[i][j] = 0;
        }
    }
    vector<Matrix> set = getRandomSudokuSet(nullMatrix, 70);

    Matrix result;
    for (Matrix grid : set)
    {

        std::mt19937 rndGen(std::random_device{}());
        std::vector<int> cells(81);
        std::iota(cells.begin(), cells.end(), 0);
        std::shuffle(cells.begin(), cells.end(), rndGen);

        int removedCount = 0;

        for (int cellIdx : cells)
        {
            int r = cellIdx / 9;
            int c = cellIdx % 9;
            int backup = grid[r][c];

            grid[r][c] = 0;

            updateSolutions(grid, 2);

            if (count() == 1)
            {
                removedCount++;
            }
            else
            {
                grid[r][c] = backup;
            }

            result = grid;

            if (removedCount >= cellCount)
            {
                goto end;
            }
        }
    }
end:
    return result;
}
