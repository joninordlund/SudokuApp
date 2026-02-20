#include "sudokureader.h"
#include <unistd.h>

#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <tesseract/baseapi.h>

std::pair<reader::SudokuGrid, cv::Mat> SudokuReader::getImageData(const cv::Mat& img)
{
    setImage(img);
    cv::Mat processedImg = processor->getSudokuImage();
    std::pair<reader::SudokuGrid, cv::Mat> data = getSudokuCells(processedImg);
    return data;
}

void SudokuReader::setImage(const cv::Mat& img)
{
    processor = std::make_unique<ImageProcessor>(img);
    tess = std::make_unique<tesseract::TessBaseAPI>();

    // const char *datapath = "/usr/share/tesseract-ocr/5/tessdata/";
    setenv("TESSDATA_PREFIX", "/home/linuxguy/tesseract/tessdata/", 1);

    if (tess->Init(nullptr, "eng", tesseract::OEM_TESSERACT_ONLY))
    {
        std::cerr << "Tesseractin alustus epäonnistui\n";
        throw std::runtime_error("Tesseract alustus epäonnistui");
    }

    tess->SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
    tess->SetVariable("debug_file", "/dev/null");
    tess->SetVariable("tessedit_char_whitelist", "123456789");
    tess->SetVariable("classify_bln_numeric_mode", "1");
    tess->SetVariable("enable_new_segsearch", "0");
    tess->SetVariable("classify_enable_learning", "0");
    tess->SetVariable("classify_enable_adaptive_matcher", "1");
    tess->SetVariable("enable_new_segsearch", "0");
}

std::pair <reader::SudokuGrid, cv::Mat> SudokuReader::getSudokuCells(const cv::Mat& processedImg)
{
    // reader::SudokuGrid grid;
    reader::SudokuGrid grid(9, std::vector<int>(9, 0));
    cv::Mat cell;

    // for (int y = 0; y < 9; y++)
    // {
    //     for (int x = 0; x < 9; x++)
    //     {
    //         tess->Clear();
    //         cell = processor->getCellImage(processedImg, x, y);
    //         tess->SetImage(cell.data, cell.cols, cell.rows, cell.channels(), cell.step1());
    //         tess->Recognize(0);
    //         const char* outText = tess->GetUTF8Text();
    //         float confidence = tess->MeanTextConf();
    //         if (confidence < 10)
    //         {
    //             grid[y][x] = 0;
    //         }
    //         else
    //         {
    //             // std::cout << "Kuva " << y << "," << x << ": Tunnistettu teksti: " << outText;
    //             // std::cout << "Konfidenssi: " << confidence << "%" << std::endl;
    //             grid[y][x] = outText[0] - '0';
    //         }
    //         delete[] outText;
    //     }

    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            cell = processor->getCellImage(processedImg, x, y);
            cv::bitwise_not(cell, cell);

            tess->SetImage(cell.data, cell.cols, cell.rows, cell.channels(), cell.step1());
            tess->Recognize(0);

            char* outText = tess->GetUTF8Text();
            int confidence = tess->MeanTextConf();
            // std::cout << "Number: " << outText << " Conf: " << confidence << std::endl;

            if (outText && outText[0] >= '1' && outText[0] <= '9' && confidence > 65)
            {
                grid[y][x] = outText[0] - '0';
            }
            else
            {
                grid[y][x] = 0;
            }
            delete[] outText;
        }
    }

    cv::Mat fullProcessedImage = processor->getProcessedSudokuImage(processedImg);
    cv::imwrite("debug_final.png", fullProcessedImage);

    return std::make_pair(grid, fullProcessedImage);
}
