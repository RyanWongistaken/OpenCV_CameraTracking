////////////////////////////////////////////////////////////////
// 
// Lab7 by Ryan Wong for BCIT-4618
// Date Modified: 1-13-2020
//
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

int main(int argc, char* argv[])
{	
	CTrak camera(1); // Create object
	camera.run(); // run the run function

}
