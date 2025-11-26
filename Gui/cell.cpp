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

void Cell::setDigit(int digit, bool isGiven)
{
    m_digit = digit;
    m_isGiven = isGiven;
    update();
}

void Cell::setCursor(bool cursor)
{
    m_isCursor = cursor;
    update();
}

void Cell::setGiven(bool given)
{
    m_isGiven = given;
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

    painter.setPen(QPen(Qt::black, 1));
    painter.drawRect(rect().adjusted(0, 0, -1, -1));

    int thickLineWidth = 4;
    if (m_col % 3 == 0)
    {
        painter.fillRect(QRect(0, 0, thickLineWidth, height()), Qt::black);
    }
    if (m_col == 8)
    {
        painter.fillRect(QRect(width() - thickLineWidth, 0, thickLineWidth, height()), Qt::black);
    }

    if (m_row % 3 == 0)
    {
        painter.fillRect(QRect(0, 0, width(), thickLineWidth), Qt::black);
    }
    if (m_row == 8)
    {
        painter.fillRect(QRect(0, height() - thickLineWidth, width(), thickLineWidth), Qt::black);
    }

    painter.setFont(QFont("Arial", 28, m_isGiven ? 700 : 500));
    if (m_digit != 0)
    {
        QColor color = m_isGiven ? Qt::black : Qt::red;

        painter.setPen(color);
        painter.drawText(rect(), Qt::AlignCenter, QString::number(m_digit));
    }
}
