////////////////////////////////////////////////////////////////
// 
// Lab7 by Ryan Wong for BCIT-4618
// Date Modified: 1-13-2020
//
////////////////////////////////////////////////////////////////
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
	int colour_flag = 0;
	cv::Mat frame, HSV, CSELECT, eroded, dilated;
	cv::Rect r; 
	std::vector<cv::Vec4i> hierarchy;
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Point> contour;
	double fpsnumb;
	double start;
	double freq;
	int area;
	int largest_area;
	std::vector<cv::Scalar> HSV_LOWER;
	std::vector<cv::Scalar> HSV_UPPER;
	
	int servo_x_position = 90;
	int servo_y_position = 90;
	
	int position_y;
	int position_x;

	//multi threading
	bool _thread_exit;
	static UINT update_thread(CTrak* ptr);
	static UINT draw_thread(CTrak* ptr);
public:
	CTrak(int chosen_camera);
	~CTrak();
	void draw();
	void update();
	void run();
	void thread_start();
};