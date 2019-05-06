#pragma once
#include "Matrix.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>

using namespace std;

std::pair<vector<Matrix>,int> read_video(string filename,pair<int,int> dimensions) {
	vector<Matrix> frames;
	int nframes;
	try {
		//open the video file
		cv::VideoCapture cap(filename); // open the video file
		if (!cap.isOpened()) {  // check if we succeeded
			CV_Error(CV_StsError, "Can not open Video file");
		}
		nframes = cap.get(CV_CAP_PROP_FRAME_COUNT);
		for (int frameNum = 0; frameNum < nframes; frameNum++)
		{
			cv::Mat frame;
			Matrix Frame = Matrix(dimensions.first, dimensions.second);
			cap >> frame; // get the next frame from video
			cv::Mat greyscale;
			cv::cvtColor(frame, greyscale, CV_RGB2GRAY);
			for (int i = 0; i < dimensions.first; i++) {
				for (int j = 0; j < dimensions.second; j++) {
					Frame.put(i, j, float(greyscale.at<int>(i, j)));
				}
			}
			frames.push_back(Frame);
		}
	}
	catch (cv::Exception& e) {
		cerr << e.msg << endl;
		exit(1);
	}
	return std::make_pair(frames,nframes);
}