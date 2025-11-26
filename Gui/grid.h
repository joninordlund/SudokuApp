#pragma once

#include <QWidget>
#include <QVector>
#include <vector>
#include "cell.h"

enum EEditMode {
    SOLVE, EDIT_CLUES
};

using SudokuGrid = QVector<Cell *>;
class Grid : public QWidget
{
    Q_OBJECT

public:
    explicit Grid(QWidget *parent) : QWidget(parent) {};
    Grid(const std::vector<std::vector<int>> &matrix, QWidget *parent = nullptr);
    std::vector<std::vector<int>> toMatrix();
    void fromMatrix(const std::vector<std::vector<int>> &matrix, bool isSolution);
    void setEditMode(EEditMode mode){m_editMode = mode;}

public slots:
    void handleMousePress(Cell *cell, bool ctrl);
    void onShowSolution();
    void onHideSolution();

private:
    QVector<Cell *> m_cells;
    QSet<Cell *> m_selected;
    std::vector<std::vector<int>> m_currentSudoku;
    Cell *m_lastSelectedCell;
    bool m_isDragging;
    bool m_editMode;
    bool m_dirty;

    void keyPressEvent(QKeyEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
protected:
    // void paintEvent(QPaintEvent *event) override;
};
