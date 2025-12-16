#pragma once

#include "sudokureader.h"
#include <QLabel>
#include <QWidget>
#include <opencv2/opencv.hpp>

class SudokuImage : public QFrame {
    Q_OBJECT
  public:
    explicit SudokuImage(QFrame* parent = nullptr);

  private:
    QLabel* m_image;
    cv::Mat m_cvImg;
    SudokuReader m_reader;

    QPixmap cvMatGrayscaleToQPixmap(const cv::Mat& mat);
    QPixmap cvMatColorToQPixmap(const cv::Mat& mat);

  signals:
    void newSudoku(const reader::SudokuGrid& grid);

  protected:
    void mousePressEvent(QMouseEvent* event);

};
