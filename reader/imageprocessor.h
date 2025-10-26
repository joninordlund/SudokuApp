#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

class ImageProcessor
{
private:
    cv::Mat image;

public:
    ImageProcessor(const cv::Mat &img) : image(img)
    {
        // std::cout << "ImageProcessor: Image size: " << image.size() << std::endl;
        // std::cout << "ImageProcessor: Image type: " << image.type() << std::endl;
    }
    cv::Mat getSudokuImage();
    cv::Mat getCellImage(const cv::Mat &img, int x, int y);

private:
    cv::Mat makeThresholdedImage(const cv::Mat &img);
    cv::Mat cropSquare(cv::Mat thresh);
    cv::Mat makeThresholdedImageForContour(const cv::Mat &img);
    cv::Mat deskewImage(const cv::Mat &cropped);
};
