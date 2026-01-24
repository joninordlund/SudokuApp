#include "sudokuboard.h"
#include <QDebug>

SudokuBoard::SudokuBoard() :
    m_data(9, vector<CellData>(9, CellData{0, false}))
{
}

void SudokuBoard::setCenterMarks(int row, int col, int value)
{
    if (digit(row, col) == 0)
    {
        uint16_t centerMarks = m_data[row][col].centerMarks;
        m_data[row][col] = CellData{0, false, centerMarks ^= (1 << (value - 1)), 0};
        qDebug() << centerMarks;
    }
}

void SudokuBoard::clearUserDigits()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (!m_data[i][j].isGiven)
            {
                m_data[i][j].digit = 0;
                m_data[i][j].cornerMarks = 0;
                m_data[i][j].centerMarks = 0;
            }
        }
    }
}

void SudokuBoard::clearAll()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            m_data[i][j] = CellData{0, false, 0, 0};
        }
    }
}

vector<vector<int>> SudokuBoard::toIntMatrix() const
{
    vector<vector<int>> matrix(9, vector<int>(9, 0));
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            matrix[i][j] = m_data[i][j].digit;
        }
    }
    return matrix;
}


void SudokuBoard::applySolution(const vector<vector<int>>& solution)
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (!m_data[i][j].isGiven)
            {
                m_data[i][j].digit = solution[i][j];
                m_data[i][j].cornerMarks = 0;
                m_data[i][j].centerMarks = 0;
            }
        }
    }
}

void SudokuBoard::setGivens(const reader::SudokuGrid& clues)
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            m_data[i][j].digit = clues[i][j];
            if(clues[i][j] > 0)
            {
                m_data[i][j].isGiven = true;
            }
            else
            {
                m_data[i][j].isGiven = false;
            }
        }
    }
}
