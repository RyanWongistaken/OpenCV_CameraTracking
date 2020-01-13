////////////////////////////////////////////////////////////////
// 
// Lab7 by Ryan Wong for BCIT-4618
// Date Modified: 1-13-2020
//
////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CTrak.h"

/** @brief enables multi threading and pushes HSV values into the desried vector
*
* @parameter chosen camera 
* @return nothing
*/
CTrak::CTrak(int chosen_camera)
{
	using namespace std;
	_thread_exit = false;
	vid.open(chosen_camera);
	if (vid.isOpened() == TRUE)
	{

	}
	else
	{
		std::cout << "ERROR: Could not open Camera :(" << std::endl;
	}
	HSV_LOWER.push_back(cv::Scalar(82, 30, 156 ));//blue
	HSV_UPPER.push_back(cv::Scalar(135, 255, 255));//blue

	HSV_LOWER.push_back(cv::Scalar(5, 0, 242));//Orange
	HSV_UPPER.push_back(cv::Scalar(61, 184, 255));//Orange

	HSV_LOWER.push_back(cv::Scalar(75, 125, 158));//Green
	HSV_UPPER.push_back(cv::Scalar(92, 255, 255));//Green
	cout << "Chose a color friend" << endl;
	cout << "0 = blue" << endl << "1 = Orange" << endl << "2 = Green" << endl;
	cin >> colour_flag;

}

/** @brief nothing
*
* @parameter nothing
* @return nothing
*/
CTrak::~CTrak()
{

}

/** @brief does all calculations (fps, servo position, size of rectangle drawn)
*
* @parameter nothing
* @return nothing
*/
void CTrak::update()
{
	using namespace std;
	using namespace cv;
	freq = cv::getTickFrequency();
	start = cv::getTickCount();
	largest_area = 0;

	//Colour selection process
	vid >> canvas; // put the video into the canvas to be editied
	cv::cvtColor(canvas, HSV, cv::COLOR_BGR2HSV); //change canvas from BGR to HSV
	cv::inRange(HSV, HSV_LOWER[colour_flag], HSV_UPPER[colour_flag], CSELECT);//take the HSV and remove the undesired colors

	//clean up the image
	cv::erode(CSELECT, dilated, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(17, 17)));//3-6
	cv::dilate(dilated, eroded, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15)));//3-6

	//draw the rectangle around the object
	findContours(eroded, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	for (unsigned int i = 0; i < contours.size(); i++)
	{
		r = cv::boundingRect(contours.at(i));
		area = r.width * r.height;
		cv::rectangle(canvas, r, cv::Scalar(255, 255, 255), 3);
		if (largest_area <= area)
		{
			position_x = r.x + (r.width / 2);
			position_y = r.y + (r.height / 2);
			largest_area = area;
		}
	}

	//move the servo in the y direction if object is not centered
	if (position_y <= (canvas.size().height * 0.47))
	{
		servo_y_position = servo_y_position + 1;
		Arduino.set_data(SERVO, 0, servo_y_position);
		Sleep(250);

		//keeps it within range
		if (servo_y_position < 0)
		{
			servo_y_position = 0;
		}
		else if (servo_y_position > 175)
		{
			servo_y_position = 175;
		}

	}
	else if (position_y >= (canvas.size().height * 0.53))
	{
		servo_y_position = servo_y_position - 1;
		Arduino.set_data(SERVO, 0, servo_y_position);
		Sleep(250);
		//keeps it within range
		if (servo_y_position < 0)
		{
			servo_y_position = 0;
		}
		else if (servo_y_position > 175)
		{
			servo_y_position = 175;
		}

	}

	//move the servo in the x direction if object is not centered
	if (position_x <= (canvas.size().width * 0.47))
	{
		servo_x_position = servo_x_position + 1;
		Arduino.set_data(SERVO, 1, servo_x_position);
		

		//keeps it within range
		if (servo_x_position < 0)
		{
			servo_x_position = 0;
		}
		else if (servo_x_position > 175)
		{
			servo_x_position = 175;
		}

	}
	else if (position_x= (canvas.size().width * 0.53))
	{
		servo_x_position = servo_x_position - 1;
		Arduino.set_data(SERVO, 1, servo_x_position);

		//keeps it within range
		if (servo_x_position < 0)
		{
			servo_x_position = 0;
		}
		else if (servo_x_position > 175)
		{
			servo_x_position = 175;
		}

	}
	fpsnumb = (freq / (cv::getTickCount() - start));

}

/** @brief enables threads to acess to acess the method update
*
* @parameter pointer to the class
* @return returns 0
*/
UINT CTrak::update_thread(CTrak* ptr)
{
	while (ptr->_thread_exit == false)
	{
		ptr->update();
	}
	return 0;
}

/** @brief enables threads to acess to acess the method update
*
* @parameter pointer to the class
* @return nothing
*/
UINT CTrak::draw_thread(CTrak* ptr)
{
	while (ptr->_thread_exit == false)
	{

		ptr->draw();

	}
	return 0;
}


/** @brief puts the canvas on the screen
*
* @parameter nothing
* @return nothing
*/
void CTrak::draw()
{
	using namespace std;
	string fps = "FPS: " + to_string(fpsnumb);
	string _area = "Area: " + to_string(largest_area);
	cv::putText(canvas, _area, cv::Point(450, 425), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(255, 255, 255), 1);
	cv::putText(canvas, fps, cv::Point(450, 450), cv::FONT_HERSHEY_COMPLEX_SMALL, 1 , cv::Scalar(255, 255, 255), 1);
	cv::imshow("canvas", canvas);
	//cv::imshow("CSELECT", CSELECT);
	//cv::imshow("eroded", eroded);
	cv::waitKey(1);

}

/** @brief starts and then detaches the threads
*
* @parameter nothing
* @return nothing
*/
void CTrak::thread_start()
{
	std::thread t1(&CTrak::update_thread, this);
	std::thread t2(&CTrak::draw_thread, this);
	t1.detach();
	t2.detach();
}

/** @brief runs the program forever
*
* @parameter nothing
* @return nothing
*/
void CTrak::run()
{
	using namespace std;
	CTrak::thread_start();
	while (1);
	{
		std::cout << "ignore me!" << endl;
	}
	_thread_exit = true;
	cv::waitKey(100);
}