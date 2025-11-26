#include "grid.h"
#include "sudokusolver.h"

#include <QGridLayout>
#include <QPainter>
#include <QFrame>
#include <QKeyEvent>

Grid::Grid(const std::vector<std::vector<int>> &matrix, QWidget *parent) : QWidget(parent),
                                                                           m_cells(81),
                                                                           m_lastSelectedCell(nullptr),
                                                                           m_isDragging(false),
                                                                            m_dirty(false)
{
    QGridLayout *layout = new QGridLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(8, 8, 8, 8);

    for (int i = 0; i < 81; i++)
    {
        int row = i / 9;
        int col = i % 9;
        m_cells[i] = new Cell(row, col, this, this);
        layout->addWidget(m_cells[i], row, col);
    }
    fromMatrix(matrix, false);
    m_currentSudoku = toMatrix();
    // setFocusPolicy(Qt::StrongFocus);
    grabKeyboard();
}

std::vector<std::vector<int>> Grid::toMatrix()
{
    std::vector<std::vector<int>> matrix(9, std::vector<int>(9, 0));

    for (const Cell *cell : m_cells)
    {
        matrix[cell->row()][cell->col()] = cell->digit();
    }
    return matrix;
}

void Grid::fromMatrix(const std::vector<std::vector<int>> &matrix, bool isSolution)
{
    for (Cell *cell : m_cells)
    {
        bool isGiven = matrix[cell->row()][cell->col()] > 0 ? true : false;
        cell->setDigit(matrix[cell->row()][cell->col()], isGiven);
    }
}

void Grid::handleMousePress(Cell *cell, bool ctrl)
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
        for (Cell *cell : std::as_const(m_selected))
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

void Grid::mouseReleaseEvent(QMouseEvent *event)
{
    m_isDragging = false;
}

void Grid::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging && event->buttons() & Qt::LeftButton)
    {
        Cell *cell = dynamic_cast<Cell *>(childAt(event->pos()));
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

void Grid::keyPressEvent(QKeyEvent *event)
{
    bool alt = event->modifiers() & Qt::AltModifier;     // corner pencil marks
    bool shift = event->modifiers() & Qt::ShiftModifier; // center pencil marks
    bool ctrl = event->modifiers() & Qt::ControlModifier;

    int key = event->key();

    if (key - Qt::Key_0 >= 1 && key - Qt::Key_0 <= 9)
    {
        for (Cell *cell : std::as_const(m_selected))
        {
            if(!cell->isGiven() || m_editMode == EDIT_CLUES)
            {
                cell->setDigit(key - Qt::Key_0, m_editMode == EDIT_CLUES);
                m_dirty = m_editMode == EDIT_CLUES ? true : false;
            }
        }
        return;
    }
    if (key == Qt::Key_Escape)
    {
        for (Cell *cell : std::as_const(m_selected))
        {
            cell->setSelected(false);
            cell->setCursor(false);
        }
        m_lastSelectedCell = nullptr;
        return;
    }
    if (key == Qt::Key_Delete || key == Qt::Key_Backspace)
    {
        if (m_lastSelectedCell)
        {
            m_lastSelectedCell->setDigit(0, m_editMode == EDIT_CLUES);
            m_lastSelectedCell->clearCornerMarks();
            m_lastSelectedCell->clearCenterMarks();
        }
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
            newCol = std::max(0, col - 1);
            break;
        case Qt::Key_Right:
            newCol = std::min(8, col + 1);
            break;
        case Qt::Key_Up:
            newRow = std::max(0, row - 1);
            break;
        case Qt::Key_Down:
            newRow = std::min(8, row + 1);
            break;
        default:
            return;
        }

        Cell *nextCell = m_cells[newRow * 9 + newCol];
        if (ctrl)
        {
            m_selected.insert(nextCell);
            nextCell->setSelected(true);
        }
        else
        {
            for (Cell *cell : std::as_const(m_selected))
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
    SudokuSolver solver;
    auto sols = solver.getSolutions(m_currentSudoku);
    qDebug() << "Solvaa mua!";
    fromMatrix(sols[0], true);
    update();
}

void Grid::onHideSolution()
{
    fromMatrix(m_currentSudoku, false);
    update();
}
