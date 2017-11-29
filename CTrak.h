

#pragma once
#include "Serial.h"
#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include "CBase4618.h"
#include <fstream>
#include <thread>
#include <mutex>
#include <cmath>
// OpenCV Include
#include "opencv.hpp"

// OpenCV Library
#pragma comment(lib,".\\opencv\\lib\\opencv_world310d.lib")

class CTrak : public CBase4618 //Ctrak class inherits all the properties of CBASE4618
{
private:
	cv::VideoCapture vid;
	cv::Mat frame, HSV, CSELECT, eroded, dilated;
	cv::Rect r; 
	std::vector<cv::Vec4i> hierarchy;
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Point> contour;
	double fpsnumb;
	double start;
	double freq;


public:
	CTrak(int chosen_camera);
	~CTrak();
	void draw();
	void update();

};