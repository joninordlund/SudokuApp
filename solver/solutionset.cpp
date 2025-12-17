#include "solutionset.h"

void SolutionSet::updateSolutions(const Matrix& matrix, int maxCount)
{
    m_maxSolCount = maxCount;
    m_solutions = m_solver.getSolutions(matrix, false, maxCount);
}

Matrix SolutionSet::getRandomSudoku(const Matrix &sudoku)
{
    return m_solver.getRandomSudoku(sudoku);
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
