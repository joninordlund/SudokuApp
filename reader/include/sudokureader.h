#pragma once

#include <tesseract/baseapi.h>
#include <opencv2/opencv.hpp>
#include "imageprocessor.h"

namespace reader {
    using SudokuGrid = std::array<std::array<int, 9>, 9>;
}

class SudokuReader
{
private:
    cv::Mat image;
    std::unique_ptr<tesseract::TessBaseAPI> tess;
    std::unique_ptr<ImageProcessor> processor;

    void setImage(const cv::Mat &img);
    std::pair<reader::SudokuGrid, cv::Mat> getSudokuCells(const cv::Mat &processedImg);

public:
    SudokuReader(){};
    std::pair<reader::SudokuGrid, cv::Mat> getImageData(const cv::Mat& img);
    const ImageProcessor& getProcessor() const { return *processor; }
};
