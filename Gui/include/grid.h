#pragma once

#include "cell.h"
#include "history.h"
#include "solutionset.h"
#include "sudokumatrix.h"
#include "sudokureader.h"

#include <QVector>
#include <QWidget>
#include <vector>

enum EEditMode {
    SOLVE = 1,
    EDIT_CLUES
};

using namespace std;
using SudokuGrid = QVector<Cell*>;

class Grid : public QWidget {
    Q_OBJECT

  public:
    explicit Grid(QWidget* parent = nullptr);

    // vector<vector<CellData>> toMatrix();
    // void fromMatrix(const vector<vector<CellData>>& matrix);
    void applyStateChange(int x, int y, int digit);

  public slots:
    void newSudoku(const reader::SudokuGrid& grid);
    void handleMousePress(Cell* cell, bool ctrl);
    void onShowSolution();
    void onHideSolution();
    void onEditModeChanged(bool checked);
    void onClearSolution();
    void onRandom();
    void onUndo() { m_history.undo(); }
    void onRedo() { m_history.redo(); }

  private:
    QVector<Cell*> m_cells;
    QSet<Cell*> m_selected;
    Cell* m_lastSelectedCell;
    bool m_isDragging;
    EEditMode m_editMode;
    bool m_dirty;
    SudokuMatrix m_currentSudoku;
    SolutionSet m_solutionSet;
    History m_history;

    void updateUI();

  protected:
    void keyPressEvent(QKeyEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
};
