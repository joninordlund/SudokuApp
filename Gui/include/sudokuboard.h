#pragma once
#include "cellchange.h"
#include "sudokureader.h"
#include <vector>

using namespace std;

class SudokuBoard
{
  public:
    SudokuBoard();
    SudokuBoard(const vector<vector<int>>& puzzle);

    CellData data(int row, int col) const { return m_data[row][col]; }
    int digit(int row, int col) const { return m_data[row][col].digit; }
    bool isGiven(int row, int col) const { return m_data[row][col].isGiven; }

    void setCellData(int row, int col, CellData data) { m_data[row][col] = data; }
    void setDigit(int row, int col, int digit, bool given) { m_data[row][col] = CellData{digit, given, 0, 0}; }
    void setCenterMarks(int row, int col, int digit);
    void clearUserDigits();
    void clearAll();

    vector<vector<int>> toIntMatrix() const;
    vector<vector<CellData>> toCellDataMatrix() const { return m_data; }

    void saveData() { m_snapShot = m_data; }
    void restoreData() { m_data = m_snapShot; }

    void applySolution(const vector<vector<int>>& sol);

    void setGivens(const reader::SudokuGrid &clues);
  private:
    vector<vector<CellData>> m_data;
    vector<vector<CellData>> m_snapShot;
};
