#pragma once
#include "Matrix.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>

Matrix frametomatrix(cv::Mat frame, std::pair<int, int> dimensions) {
	cv::Mat greyscale;
	Matrix Frame = Matrix(dimensions.first, dimensions.second);
	cv::cvtColor(frame, greyscale, CV_RGB2GRAY);
	uchar* mp = &greyscale.at<uchar>(0);
	for (int i = 0; i < dimensions.first; i++) {
		for (int j = 0; j < dimensions.second; j++) {
			Frame.put(i, j, float(mp[i*dimensions.second + j]));
		}
	}
	return Frame;
}

std::vector<Matrix> read_video(std::string filename,std::pair<int,int> dimensions) {
	std::vector<Matrix> frames;
	try {
		//open the video file
		cv::VideoCapture cap(filename); // open the video file
		if (!cap.isOpened()) {  // check if we succeeded
			CV_Error(CV_StsError, "Can not open Video file");
		}
		//cap.get(CV_CAP_PROP_FRAME_COUNT) contains the number of frames in the video;
		for (int frameNum = 0; frameNum < cap.get(CV_CAP_PROP_FRAME_COUNT); frameNum++)
		{
			cv::Mat frame;
			cap >> frame; // get the next frame from video
			auto Frame = frametomatrix(frame, dimensions);
			frames.push_back(Frame);
		}
	}
	catch (cv::Exception& e) {
		std::cerr << e.msg << std::endl;
		exit(1);
	}
	return frames;
}
