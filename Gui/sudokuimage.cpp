#include "sudokuimage.h"
#include <opencv2/opencv.hpp>

SudokuImage::SudokuImage(QFrame* parent) :
    QFrame(parent), m_isImgLoaded(false)
{
    m_image = new QLabel(this);
    m_image->setFixedSize(parent->size() - QSize(12, 12));
    m_image->setAlignment(Qt::AlignCenter);
}

void SudokuImage::load(QString fileName)
{
    cv::Mat origImg = cv::imread(fileName.toStdString());
    if (!origImg.empty())
    {
        setImage(origImg);
        m_isImgLoaded = true;
    }
}

void SudokuImage::setImage(cv::Mat origImg)
{
    std::pair<reader::SudokuGrid, cv::Mat> data = m_reader.getImageData(origImg);
    m_sudokuMatrix = data.first;
    m_debugPixmap = cvMatGrayscaleToQPixmap(data.second.clone());
    m_debugPixmap = m_debugPixmap.scaled(
        m_image->size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);
    m_origPixmap = cvMatColorToQPixmap(origImg.clone());
    m_origPixmap = m_origPixmap.scaled(
        m_image->size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);
    m_image->setPixmap(m_origPixmap);
}

void SudokuImage::setDebugView(bool visible)
{

    m_image->setPixmap(visible ? m_debugPixmap : m_origPixmap);
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
    return QPixmap::fromImage(image.copy());
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
    return QPixmap::fromImage(image.copy());
}

void SudokuImage::onRemove()
{
    m_isImgLoaded = false;
    QPixmap empty(QSize(1, 1));
    m_image->clear();

    empty.fill(Qt::lightGray);
    empty = empty.scaled(
        m_image->size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);
    m_image->setPixmap(empty);
}

void SudokuImage::mousePressEvent(QMouseEvent* event)
{
    if (m_isImgLoaded)
    {
        emit newSudoku(m_sudokuMatrix);
    }
}
