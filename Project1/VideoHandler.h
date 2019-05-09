#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include "Matrix.h"
#include <sstream>

using namespace cv;
using namespace std;

vector<Matrix> read_video(string filename, pair<int, int> dimensions) {
	vector<Matrix> frames;
	VideoCapture cap(filename); // video
	if (!cap.isOpened())  
	{
	    cout << "Cannot open the video file" << endl;
	}
	
	while (true)
	{
		Matrix Frame = Matrix(dimensions.first, dimensions.second);
	    Mat frame;
	    Mat Gray_frame;
	    bool bSuccess = cap.read(frame); // read a new frame from video
	
	    if (!bSuccess) 
	    {
	        cout << "Cannot read the frame from video file" << endl;
	        break;
	    }
		cv::cvtColor(frame, Gray_frame, CV_RGB2GRAY);
		for (int i = 0; i < dimensions.first; i++) {
			for (int j = 0; j < dimensions.second; j++) {
				Frame.put(i, j, float(Gray_frame.at<int>(i, j)));
			}
		}
		frames.push_back(Frame);
	}
	
	return frames;
	
}