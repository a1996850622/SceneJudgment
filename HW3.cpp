#include "opencv2/opencv.hpp"
#include <stdio.h>

using namespace std;
using namespace cv;

/* Global variables */
int histSize = 256;
int hist_w = 512;
int hist_h = 400;
int bin_w = cvRound((double)hist_w / histSize);
Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

Mat* CalculateHistogram(Mat);
void DrawHistogram(Mat*, int);
void Compare();

int main(int argc, char *argv[]){
	char filename[] = "src_images/";
	strcat(filename, argv[1]);

	Mat dstImage = imread(filename);

	Mat srcImages[4];
	srcImages[0] = imread("src_images/beach_1.jpg");
	srcImages[1] = imread("src_images/mountain_1.jpg");
	srcImages[2] = imread("src_images/restaurant_1.jpg");
	srcImages[3] = imread("src_images/road_1.jpg");

	Mat *color_channel;

	for(int i=0; i<4; i++){
		color_channel = CalculateHistogram(srcImages[i]);
		DrawHistogram(color_channel, i);
	}

	cout << *(color_channel+2) << endl;

	waitKey(0);
	return 0;

}

Mat* CalculateHistogram(Mat srcImage){
	vector<Mat> bgr_planes;
	float rangeSize = 256;
	float range[] = {0, 256};
	const float* histRange = {range};
	bool uniform = true;
	bool accumulate = false;
	Mat b_hist, g_hist, r_hist;

	/* Split to three single-channel image and calculate the Histogram */
	split(srcImage, bgr_planes);
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	/* Normalize */
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	Mat* color_channel = new Mat[3];
	color_channel[0] = b_hist;
	color_channel[1] = g_hist;
	color_channel[2] = r_hist;

	return color_channel;

	//imshow("histImage", histImage);
}

void DrawHistogram(Mat* color_channel, int i){
	Mat b_hist = *(color_channel + 0);
	Mat g_hist = *(color_channel + 1);
	Mat r_hist = *(color_channel + 2);

	/* Draw lines on histogram */
	for(int i=1; i<histSize; i++){
		/* Draw the line of B-Channel */
		line(histImage, Point(bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1))),
			Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);

	    /* Draw the line of G-Channel */
		line(histImage, Point(bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1))),
			Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);

	    /* Draw the line of R-Channel */
		line(histImage, Point(bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1))),
			Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}

	switch(i){
		case 0:
			imshow("beach1_histogram", histImage);
			break;
		case 1:
			imshow("mountain1_histogram", histImage);
			break;
		case 2:
			imshow("restaurant1_histogram", histImage);
			break;
		case 3:
			imshow("road1_histogram", histImage);
			break;
	}
}
