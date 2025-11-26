#pragma once
#include <QWidget>
// #include "grid.h"
class Grid;

class Cell : public QWidget
{
    Q_OBJECT
public:
    Cell(int row, int col, Grid *owner, QWidget *parent = nullptr);
    void setDigit(int digit, bool isGiven);
    void setSelected(bool sel);
    void setCursor(bool cursor);
    void setGiven(bool given);

    bool isCursor() { return m_isCursor; }
    bool isGiven() { return m_isGiven; }

    int row() const { return m_row; }
    int col() const { return m_col; }
    int digit() const { return m_digit; }
    void clearCornerMarks() { m_cornerMarks.clear(); }
    void clearCenterMarks() { m_centerMarks.clear(); }

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    int m_digit;
    QSet<int> m_cornerMarks;
    QSet<int> m_centerMarks;
    int m_row;
    int m_col;
    bool m_isGiven;
    bool m_isLocked;
    bool m_isSelected;
    bool m_isCursor;

signals:
    void cellClicked(Cell *cell, bool ctrl);
};
