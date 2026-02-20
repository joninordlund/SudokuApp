#include "imageprocessor.h"
#include <algorithm>
#include <iostream>

cv::Mat ImageProcessor::getSudokuImage()
{
    cv::Mat thresh = makeThresholdedImageForContour(this->image);
    cv::Mat cropped = cropSquare(thresh);
    cv::Mat temp = deskewImage(cropped);
    cv::Mat deskewedSudoku;
    cv::bitwise_not(temp, deskewedSudoku);
    return deskewedSudoku;
}

cv::Mat ImageProcessor::makeThresholdedImageForContour(const cv::Mat& img)
{
    cv::Mat gray, thresh, blurred;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);
    cv::threshold(blurred, thresh, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(thresh, thresh, cv::MORPH_CLOSE, kernel);
    return thresh;
}

cv::Mat ImageProcessor::cropSquare(cv::Mat thresh)
{
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(thresh, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
    cv::Rect foundSquareRect;

    cv::Rect bestSquareRect;
    double largestArea = 0.0;
    double minArea = 5000.0;

    for (const auto& cnt : contours)
    {
        double epsilon = 0.02 * cv::arcLength(cnt, true);
        std::vector<cv::Point> approx;
        cv::approxPolyDP(cnt, approx, epsilon, true);

        if (approx.size() == 4)
        {
            cv::Rect box = cv::boundingRect(approx);
            double aspectRatio = static_cast<double>(box.width) / static_cast<double>(box.height);
            double area = static_cast<double>(box.width * box.height);

            if (aspectRatio > 0.95 && aspectRatio < 1.10 && area > minArea && area > largestArea)
            {
                bestSquareRect = box;
                largestArea = area;
            }
        }
    }
    if (bestSquareRect.area() > 0)
    {
        cv::Mat rect = image.clone();
        cv::rectangle(rect, bestSquareRect, cv::Scalar(0, 255, 0), 2);
        cv::Mat cropped = image(bestSquareRect).clone();
        return cropped;
    }
    else
    {
        std::cerr << "Error: couldn't find a suitable square" << std::endl;
        return cv::Mat();
    }
};

cv::Mat ImageProcessor::deskewImage(const cv::Mat& cropped)
{
    cv::Mat thresh;
    thresh = makeThresholdedImageForContour(cropped);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Point> bestContour;
    double maxArea = 0.0;

    // find the largest square in the image
    for (const auto& curve : contours)
    {
        double area = cv::contourArea(curve);
        if (area > maxArea)
        {
            double epsilon = 0.02 * cv::arcLength(curve, true);
            std::vector<cv::Point> approx;
            cv::approxPolyDP(curve, approx, epsilon, true);

            if (approx.size() == 4 && area > 1000)
            {
                maxArea = area;
                bestContour = approx;
            }
        }
    }
    if (bestContour.size() != 4)
    {
        std::cerr << "Error: couldn't find a suitable square" << std::endl;
        return cv::Mat();
    }

    // For the cv::getPerspectiveTransform function, points of the square
    // must be in the following order: top-left,
    // top-right, bottom-right and bottom-left. Also cv::Point2f-type is needed.

    std::vector<cv::Point2f> pts(4);
    std::vector<cv::Point2f> ordPts(4);
    std::vector<std::pair<double, int>> sums(4);
    std::vector<std::pair<double, int>> diffs(4);

    for (int i = 0; i < 4; i++)
    {
        double bestCntX = (float)bestContour[i].x;
        double bestCntY = (float)bestContour[i].y;

        pts[i] = cv::Point2f(bestCntX, bestCntY);
        sums[i] = std::make_pair(bestCntX + bestCntY, i);
        diffs[i] = std::make_pair(bestCntX - bestCntY, i);
    }

    std::sort(sums.begin(), sums.end());
    std::sort(diffs.begin(), diffs.end());

    ordPts[0] = pts[sums[0].second];  // top-left (the smallest sum)
    ordPts[1] = pts[diffs[3].second]; // top-right (the largest difference)
    ordPts[2] = pts[sums[3].second];  // bottom-right (the largest sum)
    ordPts[3] = pts[diffs[0].second]; // bottom-left (the smallest difference)

    int width = 900;
    int height = 900;
    std::vector<cv::Point2f> destPts(4);

    destPts[0] = cv::Point2f(0, 0);
    destPts[1] = cv::Point2f((float)(width - 1), 0);
    destPts[2] = cv::Point2f((float)(width - 1), (float)(height - 1));
    destPts[3] = cv::Point2f(0, (float)(height - 1));

    cv::Mat perspectiveTr = cv::getPerspectiveTransform(ordPts, destPts);

    cv::Mat deskewedImage, gray;
    cv::cvtColor(cropped, gray, cv::COLOR_BGR2GRAY);
    cv::warpPerspective(gray, deskewedImage, perspectiveTr, cv::Size(width, height));

    return deskewedImage;
}
cv::Mat ImageProcessor::getCellImage(const cv::Mat& img, int x, int y)
{
    int singleCellSize = img.rows / 9;
    int y_start_correction = 0;
    int margin = 9;
    cv::Mat cell = img(cv::Rect(
                           x * singleCellSize + margin,
                           y * singleCellSize + margin + y_start_correction,
                           singleCellSize - 2 * margin,
                           singleCellSize - 2 * margin))
                       .clone();
    cv::Mat thresh, blurred;

    cv::GaussianBlur(cell, blurred, cv::Size(3, 3), 0);
    cv::adaptiveThreshold(blurred, thresh, 255,
                          cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY,
                          15,
                          -1);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(thresh, thresh, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(thresh, thresh, cv::MORPH_CLOSE, kernel);

    cv::Mat erodeKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2)); // Pienempi
    cv::erode(thresh, thresh, erodeKernel, cv::Point(-1, -1), 1);
    cv::Mat dilateKernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2));
    cv::dilate(thresh, thresh, dilateKernel, cv::Point(-1, -1), 1); // Vain 1 iterointi!

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    cv::Rect bestBBox;

    double maxArea = 0.0;

    for (const auto& cnt : contours)
    {
        double area = cv::contourArea(cnt);
        if (area > maxArea && area > (thresh.cols * thresh.rows) * 0.05)
        {
            maxArea = area;
            bestBBox = cv::boundingRect(cnt);
        }
    }

    if (bestBBox.area() > 0)
    {
        int margin = 3;
        cv::Rect finalRoi = bestBBox;
        finalRoi.x = std::max(0, finalRoi.x - margin);
        finalRoi.y = std::max(0, finalRoi.y - margin);

        finalRoi.width = std::min(thresh.cols - finalRoi.x, finalRoi.width + 2 * margin);
        finalRoi.height = std::min(thresh.rows - finalRoi.y, finalRoi.height + 2 * margin);

        cv::Mat croppedNumber = thresh(finalRoi).clone();

        int padding = 10;
        cv::copyMakeBorder(croppedNumber, croppedNumber,
                           padding,
                           padding,
                           padding,
                           padding,
                           cv::BORDER_CONSTANT,
                           cv::Scalar(0));
        int DESIRED_SIZE = 100;

        cv::Mat resizedNumber;
        double aspectRatio = (double)croppedNumber.cols / croppedNumber.rows;
        int newWidth, newHeight;

        if (aspectRatio > 1.0)
        {
            newWidth = DESIRED_SIZE;
            newHeight = cvRound(DESIRED_SIZE / aspectRatio);
        }
        else
        {
            newHeight = DESIRED_SIZE;
            newWidth = cvRound(DESIRED_SIZE * aspectRatio);
        }

        cv::resize(croppedNumber, resizedNumber, cv::Size(newWidth, newHeight));
        cv::Mat finalCell = cv::Mat::zeros(DESIRED_SIZE, DESIRED_SIZE, resizedNumber.type());

        int x_offset = (DESIRED_SIZE - newWidth) / 2;
        int y_offset = (DESIRED_SIZE - newHeight) / 2;

        resizedNumber.copyTo(finalCell(cv::Rect(x_offset, y_offset, newWidth, newHeight)));
        std::vector<std::vector<cv::Point>> contours;

        cv::findContours(finalCell, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

        for (const auto& contour : contours)
        {
            double area = cv::contourArea(contour);
            // Jos alue on pienempi kuin esim. 10-20 pikseliä, se on kohinaa
            if (area < 100)
            {
                cv::drawContours(finalCell, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(0), -1);
            }
        }

        return finalCell;
    }
    else
    {
        cv::Mat m = cv::Mat::ones(50, 50, thresh.type());
        return cv::Mat::ones(50, 50, thresh.type());
    }
}

cv::Mat ImageProcessor::getProcessedSudokuImage(const cv::Mat& deskewedOriginal)
{
    int singleCellSize = deskewedOriginal.rows / 9;

    cv::Mat finalImage;
    for (int y = 0; y < 9; y++)
    {
        cv::Mat rowImage;
        for (int x = 0; x < 9; x++)
        {

            cv::Mat cell = getCellImage(deskewedOriginal, x, y);

            cv::Mat resizedCell;
            cv::bitwise_not(cell, cell);
            cv::resize(cell, resizedCell, cv::Size(singleCellSize, singleCellSize));

            if (x == 0)
            {
                rowImage = resizedCell;
            }
            else
            {
                cv::hconcat(rowImage, resizedCell, rowImage);
            }
        }

        if (y == 0)
        {
            finalImage = rowImage;
        }
        else
        {
            cv::vconcat(finalImage, rowImage, finalImage);
        }
    }
    return finalImage;
}
