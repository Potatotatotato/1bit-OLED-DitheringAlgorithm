#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "Serial/WzSerialPort.h"
#include "Screenshot/Screenshot.h"

using namespace cv;
using namespace std;

#define CREATE_VIDEO_BIN_FILE 1

#define SERIAL_BAUDRATE 460800
#define COM_PORT "\\\\.\\COM12"	//Format for COM >= 10
//#define COM_PORT "COM4"	//Format for COM < 9

void ditheringImg(Mat& img, uint32_t row, uint32_t column);
bool getVideoFrame(VideoCapture& videoCap, Mat& img);
bool getCameraFrame(VideoCapture& cameraCap, Mat& img);
bool getScreenFrame(Screenshot& screenshot, Mat& img);

int main()
{
	uint32_t frame = 0;
	uint32_t frameByteCounter = 0;
	uint8_t imageBuffer[1024];
	Mat img;
	
	//Serial port
	WzSerialPort com;
	if (com.open(COM_PORT, SERIAL_BAUDRATE, 0, 8, 1))
		printf("Open COM succeed!\r\n");
	else
		printf("Can't open COM!\r\n");

#if CREATE_VIDEO_BIN_FILE == 1
	FILE* outputTxt;
	String outputFilePath = "ditheringVideo.bin";
	fopen_s(&outputTxt, outputFilePath.c_str(), "wb"); //must use "wb", because we are create a binary file.
#endif

	String videoPath = "E:\\E-DISK_User\\Videos\\OBS Output\\2023-02-06 16-44-01.mp4";
	VideoCapture videoCap(videoPath);
	VideoCapture cameraCap(0);
	Screenshot screenshot;

	while (1)
	{
		if (getScreenFrame(screenshot, img))
		{
			resize(img, img, Size(128, 64));
			cvtColor(img, img, COLOR_BGR2GRAY);

			for (uint32_t row = 0; row < 64; row += 2)
			{
				for (uint32_t column = 0; column < 128; column += 2)
				{
					ditheringImg(img, row, column);
				}
			}

			memset(imageBuffer, 0, 1024);
			frameByteCounter = 0;
			for (uint32_t row = 0; row < 8; row++)
			{
				for (uint32_t colum = 0; colum < 128; colum++)
				{
					//uint8_t tempByte = 0;
					for (uint8_t i = 0; i < 8; i++)
					{
						if (img.at<uchar>(row * 8 + i, colum))
							imageBuffer[frameByteCounter] |= (0x01 << i);
					}
		
					#if CREATE_VIDEO_BIN_FILE == 1
						fputc((int)imageBuffer[frameByteCounter], outputTxt);
					#endif
					frameByteCounter++;
				}
			}
			if(!com.send(imageBuffer, 1024))
				printf("Serial send failed!\r\n");
			printf("frame = %d\r\n", frame);
			frame++;
			//imshow("image", img);
			//waitKey(20);
		}
		else
		{
			#if CREATE_VIDEO_BIN_FILE == 1
				fclose(outputTxt);
			#endif
			break;
		}
	}
	return 0;

}

void ditheringImg(Mat& img, uint32_t row, uint32_t column)
{
	uint32_t gray_average = 0;
	gray_average = (img.at<uchar>(row, column) + img.at<uchar>(row + 1, column) + img.at<uchar>(row, column + 1) + img.at<uchar>(row + 1, column + 1)) / 4;
	if (gray_average < 51)
	{
		img.at<uchar>(row, column) = 0;
		img.at<uchar>(row + 1, column) = 0;
		img.at<uchar>(row, column + 1) = 0;
		img.at<uchar>(row + 1, column + 1) = 0;
	}
	else if (gray_average < 102)
	{
		img.at<uchar>(row, column) = 0;
		img.at<uchar>(row + 1, column) = 0;
		img.at<uchar>(row, column + 1) = 255;
		img.at<uchar>(row + 1, column + 1) = 0;
	}
	else if (gray_average < 153)
	{
		img.at<uchar>(row, column) = 0;
		img.at<uchar>(row + 1, column) = 255;
		img.at<uchar>(row, column + 1) = 255;
		img.at<uchar>(row + 1, column + 1) = 0;
	}
	else if (gray_average < 204)
	{
		img.at<uchar>(row, column) = 0;
		img.at<uchar>(row + 1, column) = 255;
		img.at<uchar>(row, column + 1) = 255;
		img.at<uchar>(row + 1, column + 1) = 255;
	}
	else
	{
		img.at<uchar>(row, column) = 255;
		img.at<uchar>(row + 1, column) = 255;
		img.at<uchar>(row, column + 1) = 255;
		img.at<uchar>(row + 1, column + 1) = 255;
	}
}

//true for success
bool getVideoFrame(VideoCapture& videoCap, Mat& img)
{
	return videoCap.read(img);
}

//true for success
bool getCameraFrame(VideoCapture& cameraCap, Mat& img)
{
	return cameraCap.read(img);
}

//true for success
bool getScreenFrame(Screenshot& screenshot, Mat& img)
{
	img = screenshot.getScreenshot();
	return true;
}
