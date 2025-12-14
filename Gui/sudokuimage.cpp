#include "sudokuimage.h"

#include <QImageReader>

SudokuImage::SudokuImage(QFrame* parent): QFrame(parent)
{
    // qDebug()<<parent->size();
    m_image = new QLabel(this);
    m_image->setFixedSize(parent->size());
    m_image->setAlignment(Qt::AlignCenter);

    QImageReader reader("sudoku3.jpg");
    reader.setAutoTransform(true);
    QImage img = reader.read();

    if (!img.isNull()) {
        QPixmap sudoku = QPixmap::fromImage(img);
        sudoku = sudoku.scaled(
            m_image->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            );
        m_image->setPixmap(sudoku);
    }
}
