#pragma once
#include <vector>
#include "sudokureader.h"

using namespace std;

struct CellData {
    int value = 0;
    bool isGiven = false;
};

class SudokuMatrix
{
  public:
    SudokuMatrix();
    SudokuMatrix(const vector<vector<int>>& puzzle);

    CellData data(int row, int col) const { return m_data[row][col]; }
    int digit(int row, int col) const { return m_data[row][col].value; }
    bool isGiven(int row, int col) const { return m_data[row][col].isGiven; }

    void setDigit(int row, int col, int digit, bool given) { m_data[row][col] = CellData{digit, given}; }

    void clearUserDigits();
    void clearAll();

    vector<vector<int>> toIntMatrix() const;
    vector<vector<CellData>> toCellDataMatrix() const;

    void saveData() { m_tmpData = m_data; }
    void restoreData() { m_data = m_tmpData; }

    void applySolution(const vector<vector<int>>& sol);

    void setGivens(const reader::SudokuGrid &clues);
  private:
    vector<vector<CellData>> m_data;
    vector<vector<CellData>> m_tmpData;
};
