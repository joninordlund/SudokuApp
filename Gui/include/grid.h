#pragma once

#include "cell.h"
#include "history.h"
#include "solutionset.h"
#include "sudokuboard.h"
#include "sudokureader.h"

#include <QVector>
#include <QWidget>

enum EEditMode {
    SOLVE = 1,
    EDIT_CLUES
};

enum EMarkType {
    DIGIT = 1,
    CENTERMARK,
    CORNERMARK
};

using namespace std;
using SudokuGrid = QVector<Cell*>;

class Grid : public QWidget {
    Q_OBJECT

  public:
    explicit Grid(QWidget* parent = nullptr);

    // vector<vector<CellData>> toMatrix();
    // void fromMatrix(const vector<vector<CellData>>& matrix);
    void applyStateChange(int x, int y, const CellData& data);
    History* getHistory() { return &m_history; }

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
    void handleHistoryChanged();

  signals:
    void solutionCountChanged(int count, int maxCount);

  private:
    QVector<Cell*> m_cells;
    QSet<Cell*> m_selectedCellSet;
    Cell* m_lastSelectedCell;
    bool m_isDragging;
    bool m_isPeeking;
    EEditMode m_editMode;
    bool m_dirty;
    SudokuBoard m_board;
    SolutionSet m_solutionSet;
    History m_history;

    void updateUI();
    void enterDigit(int digit, EMarkType type);

    void deleteCell();

    void changeCell(int digit, EMarkType type, bool isGiven);

    CellData handleNumberInput(int x, int y, int digit, EMarkType inputType);

  protected:
    void keyPressEvent(QKeyEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
};
