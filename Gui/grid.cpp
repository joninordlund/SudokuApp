#include "grid.h"
#include "cellchange.h"
#include "sudokusolver.h"

#include <QFrame>
#include <QGridLayout>
#include <QKeyEvent>
#include <QMessageBox>
#include <QPainter>
#include <solutionset.h>

Grid::Grid(QWidget* parent) :
    QWidget(parent),
    m_history(this),
    m_cells(81),
    m_lastSelectedCell(nullptr),
    m_isDragging(false),
    m_dirty(false),
    m_editMode(SOLVE)
{
    QGridLayout* layout = new QGridLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(8, 8, 8, 8);

    for (int i = 0; i < 81; i++)
    {
        int row = i / 9;
        int col = i % 9;
        m_cells[i] = new Cell(row, col, this);
        connect(m_cells[i], &Cell::cellClicked, this, &Grid::handleMousePress);
        layout->addWidget(m_cells[i], row, col);
    }

    updateUI();
    grabKeyboard();
}

// vector<vector<CellData>> Grid::toMatrix()
// {
//     vector<vector<CellData>> matrix(9, vector<CellData>(9, CellData{}));
//     for (const Cell* cell : m_cells)
//     {
//         matrix[cell->row()][cell->col()] = CellData{cell->digit(), cell->isGiven()};
//     }
//     return matrix;
// }

void Grid::updateUI()
{
    for (Cell* cell : m_cells)
    {
        CellData data = m_currentSudoku.data(cell->row(), cell->col());
        cell->setDigit(data.value, data.isGiven);
    }
}

void Grid::applyStateChange(int x, int y, int digit)
{
    Cell* cell = m_cells[x * 9 + y];
    m_currentSudoku.setDigit(x, y, digit, m_editMode == EDIT_CLUES);
    cell->setDigit(digit, m_editMode == EDIT_CLUES);
}

void Grid::newSudoku(const reader::SudokuGrid& grid)
{
    qDebug() << "got sudoku";
    m_currentSudoku.setGivens(grid);
    updateUI();
}

void Grid::handleMousePress(Cell* cell, bool ctrl)
{
    m_isDragging = true;
    if (ctrl)
    {
        if (m_selected.contains(cell))
        {
            m_selected.remove(cell);
            cell->setSelected(false);
        }
        else
        {
            m_selected.insert(cell);
            cell->setSelected(true);
        }
    }
    else
    {
        for (Cell* cell : as_const(m_selected))
        {
            cell->setSelected(false);
        }
        m_selected.clear();
        m_selected.insert(cell);
        cell->setSelected(true);
    }
    if (m_lastSelectedCell)
    {
        m_lastSelectedCell->setCursor(false);
    }

    m_lastSelectedCell = cell;
    m_lastSelectedCell->setCursor(true);
}

void Grid::mouseReleaseEvent(QMouseEvent* event)
{
    m_isDragging = false;
}

void Grid::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isDragging && event->buttons() & Qt::LeftButton)
    {
        Cell* cell = dynamic_cast<Cell*>(childAt(event->pos()));
        if (cell)
        {
            m_selected.insert(cell);
            m_lastSelectedCell->setCursor(false);
            cell->setSelected(true);
            cell->setCursor(true);
            m_lastSelectedCell = cell;
        }
    }
}

void Grid::keyPressEvent(QKeyEvent* event)
{
    bool alt = event->modifiers() & Qt::AltModifier;     // corner pencil marks
    bool shift = event->modifiers() & Qt::ShiftModifier; // center pencil marks
    bool ctrl = event->modifiers() & Qt::ControlModifier;

    int key = event->key();

    int digit = key - Qt::Key_0;
    if (digit >= 1 && digit <= 9)
    {
        vector<CellChange> command;
        for (Cell* cell : as_const(m_selected))
        {
            CellChange cc;
            if (!cell->isGiven() || m_editMode == EDIT_CLUES)
            {
                int x = cell->row();
                int y = cell->col();
                if (digit != m_currentSudoku.digit(x, y))
                {
                    // save old state for the history
                    cc.x = x;
                    cc.y = y;
                    cc.oldState.digit = m_currentSudoku.digit(x, y);
                    applyStateChange(x, y, digit);
                    // set new state for the history
                    cc.newState.digit = digit;
                    command.push_back(cc);
                }
            }
        }
        if (!command.empty())
        {
            m_history.setNewCommand(command);
        }

        return;
    }
    if (key == Qt::Key_Delete || key == Qt::Key_Backspace)
    {
        if (m_lastSelectedCell)
        {
            if (!m_lastSelectedCell->isGiven() || m_editMode == EDIT_CLUES)
            {
                m_lastSelectedCell->setDigit(0, false);
                m_lastSelectedCell->clearCornerMarks();
                m_lastSelectedCell->clearCenterMarks();
                int row = m_lastSelectedCell->row();
                int col = m_lastSelectedCell->col();
                m_currentSudoku.setDigit(row, col, 0, false);
            }
        }
        return;
    }
    if (key == Qt::Key_Escape)
    {
        for (Cell* cell : as_const(m_selected))
        {
            cell->setSelected(false);
            cell->setCursor(false);
        }
        m_lastSelectedCell = nullptr;
        return;
    }
    if (m_lastSelectedCell)
    {
        int row, col, newRow, newCol;
        row = newRow = m_lastSelectedCell->row();
        col = newCol = m_lastSelectedCell->col();

        switch (key)
        {
            case Qt::Key_Left:
                newCol = max(0, col - 1);
                break;
            case Qt::Key_Right:
                newCol = min(8, col + 1);
                break;
            case Qt::Key_Up:
                newRow = max(0, row - 1);
                break;
            case Qt::Key_Down:
                newRow = min(8, row + 1);
                break;
            default:
                return;
        }

        Cell* nextCell = m_cells[newRow * 9 + newCol];
        if (ctrl)
        {
            m_selected.insert(nextCell);
            nextCell->setSelected(true);
        }
        else
        {
            for (Cell* cell : as_const(m_selected))
            {
                cell->setSelected(false);
            }
            m_selected.clear();
            m_selected.insert(nextCell);
            nextCell->setSelected(true);
        }

        m_lastSelectedCell->setCursor(false);
        m_lastSelectedCell = nextCell;
        m_lastSelectedCell->setCursor(true);
    }
}

void Grid::onShowSolution()
{
    SolutionSet set;
    m_currentSudoku.saveData();
    m_currentSudoku.clearUserDigits();
    set.updateSolutions(m_currentSudoku.toIntMatrix());
    if (set.count() > 0)
    {
        m_currentSudoku.applySolution(set.next());
        qDebug() << "solution count:" << set.count();
    }
    else
    {
        qDebug() << "sudoku has no solutions!";
    }
    updateUI();
}

void Grid::onHideSolution()
{
    m_currentSudoku.restoreData();
    updateUI();
}

void Grid::onEditModeChanged(bool checked)
{
    m_editMode = checked ? EDIT_CLUES : SOLVE;
}

void Grid::onClearSolution()
{
    QString text;
    QString info;
    if (m_editMode == SOLVE)
    {
        text = "You are about clear all but the clues. Are you sure?";
        info = "If you want to clear the clues also, switch the entering clues on.";
    }
    else
    {
        text = "You are about clear everything. Are you sure?";
        info = "If you don't want to clear the clues, switch the solving mode on.";
    }
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.setInformativeText(info);
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Ok)
    {
        if (m_editMode == SOLVE)
        {
            m_currentSudoku.clearUserDigits();
        }
        else
        {
            m_currentSudoku.clearAll();
        }
        updateUI();
    }
}

void Grid::onRandom()
{
}
