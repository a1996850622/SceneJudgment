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

/* Function */
Mat* CalculateHistogram(Mat);
void DrawHistogram(Mat*, int);
void Compare(Mat*, Mat*);

int main(int argc, char *argv[]){
	char filename[] = "src_images/";
	strcat(filename, argv[1]);
	float data[12];

	Mat dstImage = imread(filename);
	Mat *dst_color_channel;
	dst_color_channel = CalculateHistogram(dstImage);

	Mat srcImages[12];
	srcImages[0]  = imread("src_images/beach_1.jpg");
	srcImages[1]  = imread("src_images/mountain_1.jpg");
	srcImages[2]  = imread("src_images/restaurant_1.jpg");
	srcImages[3]  = imread("src_images/road_1.jpg");

	srcImages[4]  = imread("src_images/beach_2.jpg");
	srcImages[5]  = imread("src_images/mountain_2.jpg");
	srcImages[6]  = imread("src_images/restaurant_2.jpg");
	srcImages[7]  = imread("src_images/road_2.jpg");

	srcImages[8]  = imread("src_images/beach_3.jpg");
	srcImages[9]  = imread("src_images/mountain_3.jpg");
	srcImages[10] = imread("src_images/restaurant_3.jpg");
	srcImages[11] = imread("src_images/road_3.jpg");

	Mat *color_channel;

	/* Calculate and Draw Histogram */
	for(int i=0; i<12; i++){
		color_channel = CalculateHistogram(srcImages[i]);
		DrawHistogram(color_channel, i);
		Compare(color_channel, dst_color_channel);
	}

	// cout << *(color_channel+2) << endl;

	waitKey(0);
	return 0;
}

/* Calculate Histogram */
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

/* Draw and Save Histogram */
void DrawHistogram(Mat* color_channel, int j){
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
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

	/* Show and Write out all the Histograms */
	switch(j){
		case 0:
			imwrite("Histogram/beach1_histogram.jpg", histImage);
			imshow("beach1_histogram", histImage);
			break;
		case 1:
			imwrite("Histogram/mountain1_histogram.jpg", histImage);
			imshow("mountain1_histogram", histImage);
			break;
		case 2:
			imwrite("Histogram/restaurant1_histogram.jpg", histImage);
			imshow("restaurant1_histogram", histImage);
			break;
		case 3:
			imwrite("Histogram/road1_histogram.jpg", histImage);
			imshow("road1_histogram", histImage);
			break;
		case 4:
			imwrite("Histogram/beach2_histogram.jpg", histImage);
			imshow("beach2_histogram", histImage);
			break;
		case 5:
			imwrite("Histogram/mountain2_histogram.jpg", histImage);
			imshow("mountain2_histogram", histImage);
			break;
		case 6:
			imwrite("Histogram/restaurant2_histogram.jpg", histImage);
			imshow("restaurant2_histogram", histImage);
			break;
		case 7:
			imwrite("Histogram/road2_histogram.jpg", histImage);
			imshow("road2_histogram", histImage);
			break;
		case 8:
			imwrite("Histogram/beach3_histogram.jpg", histImage);
			imshow("beach3_histogram", histImage);
			break;
		case 9:
			imwrite("Histogram/mountain3_histogram.jpg", histImage);
			imshow("mountain3_histogram", histImage);
			break;
		case 10:
			imwrite("Histogram/restaurant3_histogram.jpg", histImage);
			imshow("restaurant3_histogram", histImage);
			break;
		case 11:
			imwrite("Histogram/road3_histogram.jpg", histImage);
			imshow("road3_histogram", histImage);
			break;
	}
}

void Compare(Mat* color_channel, Mat* dst_color_channel){
	float sample[histSize], dst[histSize];
	float Correlation = 0;

	for(int i=0; i<histSize; i++){
		sample[i] = 0.3 * (*(color_channel + 2)).at<float>(i) + 0.59 * (*(color_channel + 1)).at<float>(i) + 0.11 * (*(color_channel + 0)).at<float>(i);
		dst[i] = 0.3 * (*(dst_color_channel + 2)).at<float>(i) + 0.59 * (*(dst_color_channel + 1)).at<float>(i) + 0.11 * (*(dst_color_channel + 0)).at<float>(i);

		//cout<< fabs(sample[i] - dst[i]) / ((sample[i] + dst[i]) / 2.0) <<endl;
		Correlation = Correlation + fabs(sample[i] - dst[i]) / ((sample[i] + dst[i]) / 2.0);
	}
	cout<<"Average:"<< Correlation/256 <<endl;
}
