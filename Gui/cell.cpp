#include "cell.h"
#include <QPainter>
#include <QMouseEvent>

Cell::Cell(int row, int col, QWidget* parent) :
    QWidget(parent),
    m_row(row),
    m_col(col),
    m_isSelected(false),
    m_isCursor(false),
    m_centerMarks(0),
    m_cornerMarks(0)
{
    setFixedSize(60, 60);
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
    drawCenterMarks(painter);
    drawCornerMarks(painter);
}

void Cell::drawCenterMarks(QPainter& painter)
{
    QString text;
    for (int i = 1; i <= 9; ++i)
    {
        if (m_centerMarks & (1 << (i - 1)))
        {
            text += QString::number(i);
        }
    }
    painter.setFont(QFont("Arial", height() / 4, QFont::Normal, true));
    painter.drawText(rect(), Qt::AlignCenter, text);
}

void Cell::drawCornerMarks(QPainter& painter)
{
    painter.setPen(Qt::darkGray);
    painter.setFont(QFont("Arial", height() / 5));

    int w = width() / 3;
    int h = height() / 3;

    for (int i = 1; i <= 9; ++i)
    {
        if (m_cornerMarks & (1 << (i - 1)))
        {
            int row = (i - 1) / 3;
            int col = (i - 1) % 3;
            QRect subRect(col * w, row * h, w, h);
            painter.drawText(subRect, Qt::AlignCenter, QString::number(i));
        }
    }
}
