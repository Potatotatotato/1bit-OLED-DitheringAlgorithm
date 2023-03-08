#pragma once
#include <opencv2/opencv.hpp>
#include <Windows.h>


class Screenshot
{
public:
    Screenshot();
    double static getZoom();
    cv::Mat getScreenshot();
    cv::Mat getScreenshot(int x, int y, int width, int height);

private:
    int m_width;
    int m_height;
    HDC m_screenDC;
    HDC m_compatibleDC;
    HBITMAP m_hBitmap;
    LPVOID m_screenshotData = nullptr;
};

