#pragma once

#include "sudokureader.h"
#include <QLabel>
#include <QPixmap>
#include <QWidget>
#include <opencv2/opencv.hpp>

class SudokuImage : public QFrame {
    Q_OBJECT
  public:
    explicit SudokuImage(QFrame* parent = nullptr);

    void load(QString fileName);
    void setDebugView(bool visible);

  private:
    QLabel* m_image;
    SudokuReader m_reader;

    QPixmap cvMatGrayscaleToQPixmap(const cv::Mat& mat);
    QPixmap cvMatColorToQPixmap(const cv::Mat& mat);

    QPixmap m_debugPixmap;
    QPixmap m_origPixmap;

    bool m_isImgLoaded;

    std::vector<std::vector<int>> m_sudokuMatrix;
    void setImage(cv::Mat img);

  signals:
    void newSudoku(const reader::SudokuGrid& grid);

  public slots:
    void onRemove();

  protected:
    void mousePressEvent(QMouseEvent* event);
};
