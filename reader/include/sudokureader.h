#pragma once

#include <iostream>
#include <tesseract/baseapi.h>
#include <opencv2/opencv.hpp>
#include "imageprocessor.h"

using SudokuGrid = std::array<std::array<int, 9>, 9>;
class SudokuReader
{
private:
    cv::Mat image;
    std::unique_ptr<tesseract::TessBaseAPI> tess;
    std::unique_ptr<ImageProcessor> processor;

public:
    SudokuReader(const cv::Mat &img);
    SudokuGrid getSudokuCells(const cv::Mat &deskewedImage);
};