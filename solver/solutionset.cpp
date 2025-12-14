#include "solutionset.h"
#include <iostream>

// SolutionSet::SolutionSet()
// {
// }

void SolutionSet::updateSolutions(const Matrix& matrix, int maxCount)
{
    m_solutions = m_solver.getSolutions(matrix, false, maxCount);
}

Matrix SolutionSet::getRandomSudoku(const Matrix &sudoku)
{
    return m_solver.getRandomSudoku(sudoku);
}

vector<vector<int> > SolutionSet::next()
{
    if(count() > 0)
    {
        Matrix solution = m_solutions[m_index];
        m_index = min(m_index + 1, min(m_maxSolCount, count()));
        return solution;
    }
    cout << "Sudoku has no solutions";
    return{{}};
}

vector<vector<int> > SolutionSet::prev()
{
    m_index = max(0, m_index - 1);
    return m_solutions[m_index];
}
