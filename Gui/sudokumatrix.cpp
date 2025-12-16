#include "sudokumatrix.h"
#include <QDebug>

SudokuMatrix::SudokuMatrix() :
    m_data(9, vector<CellData>(9, CellData{0, false}))
{
    qDebug() << "constructor";
}

void SudokuMatrix::clearUserDigits()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (!m_data[i][j].isGiven)
            {
                m_data[i][j].value = 0;
            }
        }
    }
}

void SudokuMatrix::clearAll()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            m_data[i][j] = CellData{0, false};
        }
    }
}

vector<vector<int>> SudokuMatrix::toIntMatrix() const
{
    vector<vector<int>> matrix(9, vector<int>(9, 0));
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            matrix[i][j] = m_data[i][j].value;
        }
    }
    return matrix;
}

vector<vector<CellData>> SudokuMatrix::toCellDataMatrix() const
{
    return m_data;
}

void SudokuMatrix::applySolution(const vector<vector<int>>& solution)
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (!m_data[i][j].isGiven)
            {
                m_data[i][j].value = solution[i][j];
            }
        }
    }
}

void SudokuMatrix::setGivens(const reader::SudokuGrid& clues)
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            m_data[i][j].value = clues[i][j];
            if(clues[i][j] > 0)
            {
                m_data[i][j].isGiven = true;
            }
        }
    }
}
