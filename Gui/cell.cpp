#include "cell.h"
#include "grid.h"
#include <QPainter>
#include <QMouseEvent>

Cell::Cell(int row, int col, Grid *owner, QWidget *parent) : QWidget(parent),
                                                             m_row(row),
                                                             m_col(col),
                                                             m_isSelected(false),
                                                             m_isCursor(false)
{
    setFixedSize(60, 60);

    connect(this, &Cell::cellClicked, owner, &Grid::handleMousePress);
    setFocusPolicy(Qt::StrongFocus);
}

void Cell::setSelected(bool sel)
{
    m_isSelected = sel;
    update();
}

void Cell::setDigit(int digit)
{
    m_digit = digit;
    update();
}

void Cell::setCursor(bool cursor)
{
    m_isCursor = cursor;
    update();
}

void Cell::mousePressEvent(QMouseEvent *event)
{
    bool ctrl = event->modifiers() & Qt::ControlModifier;
    emit cellClicked(this, ctrl);
}

void Cell::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (m_isSelected)
    {
        painter.fillRect(rect(), Qt::lightGray);
    }
    else
    {
        painter.fillRect(rect(), Qt::white);
    }
    QPen pen;
    if (m_isCursor && m_isSelected)
    {
        pen = QPen(Qt::darkGray, 8);
    }
    else
    {
        pen = QPen(Qt::black, 1);
    }
    painter.setPen(pen);
    painter.drawRect(rect().adjusted(0, 0, -1, -1));
    const int thickness = 3;
    if (m_row % 3 == 2)
    {
        painter.fillRect(QRect(0, height() - thickness, width(), thickness), Qt::black);
    }
    if (m_col % 3 == 2)
    {
        painter.fillRect(QRect(width() - thickness, 0, thickness, height()), Qt::black);
    }

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 28, 700));
    if (m_digit != 0)
    {
        painter.drawText(rect(), Qt::AlignCenter, QString::number(m_digit));
    }
}
