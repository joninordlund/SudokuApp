#pragma once
#include <vector>

#include "sudokusolver.h"

using namespace std;

class SolutionSet
{
  public:
    SolutionSet() : m_index(0), m_solutions{{}}{}

    void updateSolutions(const Matrix &matrix, int maxCount = 64);
    Matrix getRandomSudoku(const Matrix &matrix);
    Matrix next();
    Matrix prev();

    int count() const { return m_solutions.size(); }

  private:
    SudokuSolver m_solver;
    vector<vector<vector<int>>> m_solutions;
    int m_index;
    int m_maxSolCount;
};

