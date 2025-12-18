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
    m_isPeeking(false),
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
    connect(&m_history, &History::historyStateChanged, this, &Grid::handleHistoryChanged);
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
        CellData data = m_board.data(cell->row(), cell->col());
        cell->setDigit(data.digit, data.isGiven);
        cell->setCenterMarks(data.centerMarks);
        cell->setCornerMarks(data.cornerMarks);
    }
}

void Grid::applyStateChange(int x, int y, const CellData& newData)
{
    Cell* cell = m_cells[x * 9 + y];
    m_board.setCellData(x, y, newData);
    cell->setDigit(newData.digit, newData.isGiven);
    cell->setCornerMarks(newData.cornerMarks);
    cell->setCenterMarks(newData.centerMarks);
    cell->update();
}

void Grid::newSudoku(const reader::SudokuGrid& grid)
{
    qDebug() << "got sudoku";
    m_board.setGivens(grid);
    updateUI();
}

void Grid::handleMousePress(Cell* cell, bool ctrl)
{
    m_isDragging = true;
    if (ctrl)
    {
        if (m_selectedCellSet.contains(cell))
        {
            m_selectedCellSet.remove(cell);
            cell->setSelected(false);
        }
        else
        {
            m_selectedCellSet.insert(cell);
            cell->setSelected(true);
        }
    }
    else
    {
        for (Cell* cell : as_const(m_selectedCellSet))
        {
            cell->setSelected(false);
        }
        m_selectedCellSet.clear();
        m_selectedCellSet.insert(cell);
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
            m_selectedCellSet.insert(cell);
            m_lastSelectedCell->setCursor(false);
            cell->setSelected(true);
            cell->setCursor(true);
            m_lastSelectedCell = cell;
        }
    }
}

void Grid::keyPressEvent(QKeyEvent* event)
{
    if (m_isPeeking)
    {
        event->ignore();
        return;
    }

    bool alt = event->modifiers() & Qt::AltModifier; // corner pencil marks
    bool shift = event->modifiers() & Qt::ShiftModifier;
    bool ctrl = event->modifiers() & Qt::ControlModifier; // center pencil marks

    int key = event->key();

    int digit = -1;

    if (key >= Qt::Key_1 && key <= Qt::Key_9)
    {
        digit = key - Qt::Key_1 + 1;
    }
    if (digit != -1)
    {
        EMarkType type = ctrl ? CENTERMARK : (alt ? CORNERMARK : DIGIT);
        enterDigit(digit, type);
        return;
    }

    if (key == Qt::Key_Delete || key == Qt::Key_Backspace)
    {
        deleteCell();
    }
    if (key == Qt::Key_Escape)
    {
        for (Cell* cell : as_const(m_selectedCellSet))
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
            m_selectedCellSet.insert(nextCell);
            nextCell->setSelected(true);
        }
        else
        {
            for (Cell* cell : as_const(m_selectedCellSet))
            {
                cell->setSelected(false);
            }
            m_selectedCellSet.clear();
            m_selectedCellSet.insert(nextCell);
            nextCell->setSelected(true);
        }

        m_lastSelectedCell->setCursor(false);
        m_lastSelectedCell = nextCell;
        m_lastSelectedCell->setCursor(true);
    }
}

void Grid::onShowSolution()
{
    m_isPeeking = true;
    m_board.saveData();
    m_board.clearUserDigits();

    m_solutionSet.updateSolutions(m_board.toIntMatrix());
    if (m_solutionSet.count() > 0)
    {
        m_board.applySolution(m_solutionSet.next());
        qDebug() << "solution count:" << m_solutionSet.count();
    }
    else
    {
        qDebug() << "sudoku has no solutions!";
    }
    updateUI();
}

void Grid::onHideSolution()
{
    m_isPeeking = false;
    m_board.restoreData();
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
            m_board.clearUserDigits();
        }
        else
        {
            m_board.clearAll();
        }
        updateUI();
    }
}

void Grid::onRandom()
{
}

void Grid::handleHistoryChanged()
{
    qDebug() << "SOl COJSGH";
    m_solutionSet.updateSolutions(m_board.toIntMatrix());
    emit solutionCountChanged(m_solutionSet.count(), m_solutionSet.maxCount());
}

void Grid::enterDigit(int digit, EMarkType type)
{
    if (m_editMode == EDIT_CLUES)
    {
        type = DIGIT;
    }
    changeCell(digit, type, m_editMode == EDIT_CLUES);
}

void Grid::deleteCell()
{
    changeCell(0, DIGIT, false);
}

void Grid::changeCell(int digit, EMarkType type, bool isGiven)
{
    vector<CellChange> command;
    for (Cell* cell : as_const(m_selectedCellSet))
    {
        if (!cell->isGiven() || m_editMode == EDIT_CLUES)
        {
            int x = cell->row();
            int y = cell->col();
            CellData oldState = m_board.data(x, y);
            CellData newState = handleNumberInput(x, y, digit, type);
            if (oldState == newState)
            {
                continue;
            }
            applyStateChange(x, y, newState);
            command.push_back(CellChange{x, y, oldState, newState});
        }
    }
    if (!command.empty())
    {
        m_history.setNewCommand(command);
    }
}

CellData Grid::handleNumberInput(int x, int y, int digit, EMarkType type)
{
    CellData newState = m_board.data(x, y);
    switch (type)
    {
        case DIGIT:
            newState.digit = digit;
            newState.isGiven = (m_editMode == EDIT_CLUES);
            newState.cornerMarks = 0;
            newState.centerMarks = 0;
            break;
        case CORNERMARK:
            if (newState.digit == 0)
            {
                newState.cornerMarks ^= (1 << (digit - 1));
            }
            break;
        case CENTERMARK:
            if (newState.digit == 0)
            {
                newState.centerMarks ^= (1 << (digit - 1));
            }
            break;
    };
    return newState;
}
