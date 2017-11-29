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
	vid.open(chosen_camera);
	if (vid.isOpened() == TRUE)
	{

	}
	else
	{
		std::cout << "ERROR: Could not open Camera :(" << std::endl;
	}
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
	cv::inRange(HSV, cv::Scalar(84, 158, 53), cv::Scalar(121, 256, 191), CSELECT);//take the HSV and remove the undesired colors

																				  //clean up the image
	cv::erode(CSELECT, dilated, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
	cv::dilate(dilated, eroded, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15)));


	findContours(eroded, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	for (unsigned int i = 0; i < contours.size(); i++)
	{
		r = cv::boundingRect(contours.at(i));
		cv::rectangle(canvas, r, cv::Scalar(255, 255, 255), 3);
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
	string fps = to_string(fpsnumb);
	cv::putText(canvas, fps, cv::Point(450, 450), cv::FONT_HERSHEY_COMPLEX_SMALL, 1 , cv::Scalar(255, 255, 255), 1);
	cv::imshow("canvas", canvas);
	//cv::imshow("CSELECT", CSELECT);
	//cv::imshow("eroded", eroded);
	cv::waitKey(1);

}