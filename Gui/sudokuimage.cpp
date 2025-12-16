#include "sudokuimage.h"
#include <opencv2/opencv.hpp>

SudokuImage::SudokuImage(QFrame* parent) : QFrame(parent)
{
    m_image = new QLabel(this);
    m_image->setFixedSize(parent->size());
    m_image->setAlignment(Qt::AlignCenter);

    m_cvImg = cv::imread("sudoku121.jpg");
    if (!m_cvImg.empty())
    {
        QPixmap sudoku = cvMatColorToQPixmap(m_cvImg);
        sudoku = sudoku.scaled(
            m_image->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation);
        m_image->setPixmap(sudoku);
    }
}

QPixmap SudokuImage::cvMatGrayscaleToQPixmap(const cv::Mat& mat)
{
    if (mat.empty() || mat.type() != CV_8UC1)
    {
        qWarning() << "Matrix is empty or not grayscale";
        return QPixmap();
    }
    QImage image(
        mat.data,
        mat.cols,
        mat.rows,
        mat.step,
        QImage::Format_Grayscale8);
    return QPixmap::fromImage(image);
}

QPixmap SudokuImage::cvMatColorToQPixmap(const cv::Mat& mat)
{
    if (mat.empty() || mat.type() != CV_8UC3)
    {
        qWarning() << "Matrix is empty or not color";
        return QPixmap();
    }
    cv::Mat rgbMat;
    cv::cvtColor(mat, rgbMat, cv::COLOR_BGR2RGB);
    QImage image(
        rgbMat.data,
        rgbMat.cols,
        rgbMat.rows,
        rgbMat.step,
        QImage::Format_RGB888);
    return QPixmap::fromImage(image);
}

void SudokuImage::mousePressEvent(QMouseEvent* event)
{
    qDebug() << "Click";
    std::pair<reader::SudokuGrid, cv::Mat> data = m_reader.getImageData(m_cvImg);

    emit newSudoku(data.first);
}
