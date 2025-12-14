#pragma once

#include <QLabel>
#include <QWidget>
#include <opencv2/opencv.hpp>
#include "sudokureader.h"

class SudokuImage : public QFrame
{
    Q_OBJECT
  public:
    explicit SudokuImage(QFrame* parent = nullptr);

  private:
    QLabel* m_image;
    SudokuReader m_reader;
};

