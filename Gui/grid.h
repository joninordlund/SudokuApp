#pragma once

#include <QWidget>
#include <QVector>
#include <vector>
#include "cell.h"

using SudokuGrid = QVector<Cell *>;
class Grid : public QWidget
{
    Q_OBJECT

public:
    explicit Grid(QWidget *parent) : QWidget(parent) {};
    Grid(const std::vector<std::vector<int>> &matrix, QWidget *parent = nullptr);
    std::vector<std::vector<int>> toMatrix();
    void fromMatrix(const std::vector<std::vector<int>> &matrix);

public slots:
    void handleMousePress(Cell *cell, bool ctrl);

private:
    QVector<Cell *> m_cells;
    QSet<Cell *> m_selected;
    Cell *m_lastSelectedCell;
    bool m_isDragging;

    void keyPressEvent(QKeyEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};