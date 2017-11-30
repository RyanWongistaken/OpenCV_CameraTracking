#include "stdafx.h"
#include "CTrak.h"

/** @brief 
*
* @parameter 
* @parameter  
* @return
*/
CTrak::CTrak(int chosen_camera)
{
	using namespace std;
	vid.open(chosen_camera);
	if (vid.isOpened() == TRUE)
	{

	}
	else
	{
		std::cout << "ERROR: Could not open Camera :(" << std::endl;
	}
	HSV_LOWER.push_back(cv::Scalar(84, 158, 53));//blue
	HSV_UPPER.push_back(cv::Scalar(121, 256, 191));//blue

	HSV_LOWER.push_back(cv::Scalar(0, 103, 216));//Orange
	HSV_UPPER.push_back(cv::Scalar(35, 162, 255));//Orange

	HSV_LOWER.push_back(cv::Scalar(75, 125, 158));//Green
	HSV_UPPER.push_back(cv::Scalar(92, 255, 255));//Green
	cout << "Chose a color friend" << endl;
	cout << "0 = blue" << endl << "1 = Orange" << endl << "2 = Green" << endl;
	cin >> colour_flag;

}

/** @brief 
*
* @parameter 
* @return
*/
CTrak::~CTrak()
{

}

/** @brief
*
* @parameter
* @return
*/
void CTrak::update()
{
	using namespace std;
	using namespace cv;
	freq = cv::getTickFrequency();
	start = cv::getTickCount();

	//Colour selection process
	vid >> canvas; // put the video into the canvas to be editied
	cv::cvtColor(canvas, HSV, cv::COLOR_BGR2HSV); //change canvas from BGR to HSV
	cv::inRange(HSV, HSV_LOWER[colour_flag], HSV_UPPER[colour_flag], CSELECT);//take the HSV and remove the undesired colors

	//clean up the image
	cv::erode(CSELECT, dilated, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(17, 17)));
	cv::dilate(dilated, eroded, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15)));


	findContours(eroded, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	for (unsigned int i = 0; i < contours.size(); i++)
	{
		r = cv::boundingRect(contours.at(i));
		area = r.width * r.height;
		cv::rectangle(canvas, r, cv::Scalar(255, 255, 255), 3);
		if (largest_area <= area)
		{
			largest_area = area;
		}
	}

	fpsnumb = (freq / (cv::getTickCount() - start));

}

/** @brief
*
* @parameter
* @return
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