#pragma once
#include "Matrix.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>

void train(std::string input, std::string output, std::pair<int,int> indimen, std::pair<int,int> outdimen) {
	std::vector<std::pair<int, int>> arrangement;
	arrangement.push_back(std::make_pair(640, 480));
	arrangement.push_back(std::make_pair(480, 360));
	arrangement.push_back(std::make_pair(720, 480));
	arrangement.push_back(std::make_pair(1280, 720));
	arrangement.push_back(std::make_pair(1920, 1080));
	Network* net = new Network(arrangement, 50.0f);
	net->load();
	try {
		cv::VideoCapture inp(input); // open the video file
		if (!inp.isOpened()) {  // check if we succeeded
			CV_Error(CV_StsError, "Can not open Video file");
		}
		cv::VideoCapture out(output); // open the video file
		if (!out.isOpened()) {  // check if we succeeded
			CV_Error(CV_StsError, "Can not open Video file");
		}
		for (int frameNum = 0; frameNum < inp.get(CV_CAP_PROP_FRAME_COUNT); frameNum++) {
			cv::Mat framei;
			inp >> framei; // get the next frame from video
			Matrix inpFrame = Matrix(indimen.first, indimen.second);
			cv::Vec3b* mpi = &framei.at<cv::Vec3b>(0);
			for (int i = 0; i < indimen.first; i++) {
				for (int j = 0; j < indimen.second; j++) {
					auto uval = mpi[i*indimen.second + j];
					auto val = float((uval[0] + uval[1] + uval[2]) / 3.0);
					inpFrame.put(i, j, val);
				}
			}
			cv::Mat frameo;
			out >> frameo; // get the next frame from video
			Matrix outFrame = Matrix(outdimen.first, outdimen.second);
			cv::Vec3b* mpo = &frameo.at<cv::Vec3b>(0);
			for (int i = 0; i < outdimen.first; i++) {
				for (int j = 0; j < outdimen.second; j++) {
					auto uval = mpo[i*outdimen.second + j];
					auto val = float((uval[0] + uval[1] + uval[2]) / 3.0);
					outFrame.put(i, j, val);
				}
			}
			net->instantialise(inpFrame);
			net->evaluate();
			net->CalcCostDerivative(outFrame, net->Nlayers);
			auto f = net->CalcCost(outFrame);
			net->backprop(f);
			printf("frame no %d done cost %f", frameNum, f);
			std::cout << std::endl;
		}
		net->save();
	}
	catch (cv::Exception& e) {
		std::cerr << e.msg << std::endl;
		exit(1);
	}
}