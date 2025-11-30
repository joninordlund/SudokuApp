#pragma once

#include "cell.h"
#include "sudokumatrix.h"
#include <QVector>
#include <QWidget>
#include <solutionset.h>
#include <vector>

enum EEditMode {
    SOLVE = 1,
    EDIT_CLUES
};

using namespace std;
using SudokuGrid = QVector<Cell *>;

class Grid : public QWidget
{
    Q_OBJECT

  public:
    explicit Grid(QWidget* parent = nullptr);

    vector<vector<CellData>> toMatrix();
    void fromMatrix(const vector<vector<CellData>>& matrix);
    void updateUI();

  public slots:
    void handleMousePress(Cell *cell, bool ctrl);
    void onShowSolution();
    void onHideSolution();
    void onEditModeChanged(bool checked);
    void onClearSolution();
    void onRandom();

  private:
    QVector<Cell *> m_cells;
    QSet<Cell *> m_selected;
    Cell* m_lastSelectedCell;

    bool m_isDragging;
    EEditMode m_editMode;
    bool m_dirty;
    SudokuMatrix m_currentSudoku;
    SolutionSet m_solutionSet;

  protected:
    void keyPressEvent(QKeyEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};
