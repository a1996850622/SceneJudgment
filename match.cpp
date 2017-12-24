#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]){
	Mat beach1 = imread("src_images/beach_1.jpg");
	Mat beach2 = imread("src_images/beach_2.jpg");
	Mat mountain1 = imread("src_images/mountain_1.jpg");

	vector<Mat> bgr_planes_beach1;
	int histSize = 256;
	float rangeSize = 256;
	float range[] = {0, 256};
	const float* histRange = {range};
	bool uniform = true;
	bool accumulate = false;
	Mat b_hist_beach1, g_hist_beach1, r_hist_beach1;
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	split(beach1, bgr_planes_beach1);
	calcHist(&bgr_planes_beach1[0], 1, 0, Mat(), b_hist_beach1, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes_beach1[1], 1, 0, Mat(), g_hist_beach1, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes_beach1[2], 1, 0, Mat(), r_hist_beach1, 1, &histSize, &histRange, uniform, accumulate);

	normalize(b_hist_beach1, b_hist_beach1, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist_beach1, g_hist_beach1, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist_beach1, r_hist_beach1, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	vector<Mat> bgr_planes_beach2;
	Mat b_hist_beach2, g_hist_beach2, r_hist_beach2;
	split(beach2, bgr_planes_beach2);
	calcHist(&bgr_planes_beach2[0], 1, 0, Mat(), b_hist_beach2, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes_beach2[1], 1, 0, Mat(), g_hist_beach2, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes_beach2[2], 1, 0, Mat(), r_hist_beach2, 1, &histSize, &histRange, uniform, accumulate);
	normalize(b_hist_beach2, b_hist_beach2, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist_beach2, g_hist_beach2, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist_beach2, r_hist_beach2, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	float b1[256];
	float b2[256];

	for(int i=0; i<histSize; i++){
		b1[i] = 0.3 * r_hist_beach1.at<float>(i) + 0.59 * g_hist_beach1.at<float>(i) + 0.11 * b_hist_beach1.at<float>(i);
		b2[i] = 0.3 * r_hist_beach2.at<float>(i) + 0.59 * g_hist_beach2.at<float>(i) + 0.11 * b_hist_beach2.at<float>(i);

		cout<< fabs(b1[i] - b2[i]) / ((b1[i]+b2[i])/2.0) <<endl;
	}

	waitKey(0);
	return 0;
}