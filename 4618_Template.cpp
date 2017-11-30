////////////////////////////////////////////////////////////////
// ELEX 4618 Template project for BCIT
// Created Oct 5, 2016 by Craig Hennessey
// Last updated March 23, 2017
////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <string>
#include <iostream>
#include <thread>
#include "CControl.h"
#include "CTrak.h"
#include "Serial.h" // Must include Windows.h after Winsock2.h, so Serial must include after Client/Server

// OpenCV Include
#include "opencv.hpp"

// OpenCV Library
#pragma comment(lib,".\\opencv\\lib\\opencv_world310d.lib")



////////////////////////////////////////////////////////////////
// Display Video on screen
////////////////////////////////////////////////////////////////
void do_video()
{
  cv::VideoCapture vid;

  vid.open(1);

  if (vid.isOpened() == TRUE)
  {
    do
    {
      cv::Mat frame, edges;
      vid >> frame;
      if (frame.empty() == false)
      {
        cv::cvtColor(frame, edges, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(edges, edges, cv::Size(7, 7), 1.5, 1.5);
        cv::Canny(edges, edges, 0, 30, 3);
        cv::imshow("edges", edges);
      }
    }
    while (cv::waitKey(10) != ' ');
  }      
}		
   

int main(int argc, char* argv[])
{	
	CTrak takkerino(1);
	CControl ard;
	int i = 0;
	while (1)
	{
	takkerino.update();
	takkerino.draw();
	ard.set_data(SERVO, 0, i);
	ard.set_data(SERVO, 1, i);
	//Sleep(750);
	//i = i + 50;
	}
/*
	CControl arduino;
	
	while (1)
	{
		arduino.set_data(DIGITAL, 8, 0);
		arduino.set_data(SERVO, 0, 100);
		arduino.set_data(SERVO, 1, 100);
	}
	

	*/


	//do_video ();

}
