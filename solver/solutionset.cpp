#include "solutionset.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>

void SolutionSet::updateSolutions(const Matrix& matrix, int maxCount)
{
    // m_maxSolCount = maxCount;
    // m_solutions.clear();
    // m_solutions = m_solver.getSolutions(matrix, false, 1);
    m_maxSolCount = maxCount;
    // m_solutions = m_solver.getSolutions(matrix, true, m_maxSolCount);
    m_solutions = getRandomSudokuSet(matrix, maxCount);
    m_index = 0;
    // for (int i = 0; i < maxCount; ++i)
    // {

    //     vector<Matrix> sol = m_solver.getSolutions(matrix, false, 1);
    //     if (!sol.empty())
    //     {
    //         m_solutions.push_back((sol[0]));
    //     }
    // }
}

vector<Matrix> SolutionSet::getRandomSudokuSet(const Matrix& sudoku, int count)
{
    return m_solver.getRandomSudokuSet(sudoku, count);
}

bool SolutionSet::isUnique(const Matrix& matrix)
{
    auto sols = m_solver.getSolutions(matrix, false, 2);
    return sols.size() == 1;
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

Matrix SolutionSet::generate(int target)
{
    Matrix zeroMatrix(9, vector<int>(9, 0));
    vector<Matrix> set = getRandomSudokuSet(zeroMatrix, 10);

    Matrix bestResult;
    int maxRemoved = 0;
    std::mt19937 rndGen(std::random_device{}());

    for (Matrix grid : set)
    {
        std::vector<int> cells(81);
        std::iota(cells.begin(), cells.end(), 0);
        std::shuffle(cells.begin(), cells.end(), rndGen);

        int removedCount = 0;

        for (int cellIdx : cells)
        {
            int r = cellIdx / 9;
            int c = cellIdx % 9;

            if (grid[r][c] == 0)
            {
                continue;
            }
            int backup = grid[r][c];
            int backup2 = grid[8 - r][8 - c];

            grid[r][c] = 0;
            grid[8 - r][8 - c] = 0;

            if (isUnique(grid))
            {
                if (r == 4 && c == 4)
                {
                    removedCount += 1;
                }
                else
                {
                    removedCount += 2;
                }

                if (removedCount > maxRemoved)
                {
                    maxRemoved = removedCount;
                    bestResult = grid;
                }
            }
            else
            {
                grid[r][c] = backup;
                grid[8 - r][8 - c] = backup2;
            }

            if (removedCount >= target)
            {
                std::cout << "POISTETTU: " << maxRemoved << std::endl;
                return grid;
            }
        }
    }
    std::cout << "POISTETTU: " << maxRemoved << std::endl;
    return bestResult;
}
