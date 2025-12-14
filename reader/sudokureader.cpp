#include "sudokureader.h"
#include <unistd.h>

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <stdexcept>
#include <iostream>

void SudokuReader::setImage(const cv::Mat &img)
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
reader::SudokuGrid SudokuReader::getSudokuCells(const cv::Mat &processedImg)
{
    reader::SudokuGrid grid;
    cv::Mat cell;

    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            tess->Clear();
            cell = processor->getCellImage(processedImg, x, y);
            // cv::bitwise_not(cell, cell); // numero mustaksi
            // cv::imshow("Cell", cell);
            // cv::waitKey(0);
            tess->SetImage(cell.data, cell.cols, cell.rows, cell.channels(), cell.step1());
            tess->Recognize(0);
            const char *outText = tess->GetUTF8Text();
            float confidence = tess->MeanTextConf();
            if (confidence < 80)
            {
                grid[y][x] = 0;
            }
            else
            {
                std::cout << "Kuva " << y << "," << x << ": Tunnistettu teksti: " << outText;
                std::cout << "Konfidenssi: " << confidence << "%" << std::endl;
                grid[y][x] = outText[0] - '0';
            }
            delete[] outText;
        }
    }
    cv::Mat fullProcessedImage = processor->getProcessedSudokuImage(processedImg);
    cv::imwrite("debug_final.png", fullProcessedImage);
    // cv::imshow("Koko prosessoitu ruudukko (OCR-syöte)", fullProcessedImage);
    // cv::waitKey(0);
    return grid;
}
