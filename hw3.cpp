#include "opencv2/opencv.hpp"
#include <iostream>
#include <string.h>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]){
	char filename[] = "src_images/";
	strcat(filename, argv[1]);

	Mat srcImage = imread(filename);

	/*-- Read all of the images --*/
	Mat srcImages[12];
	srcImages[0]  = imread("src_images/beach_1.jpg");
	srcImages[1]  = imread("src_images/beach_2.jpg");
	srcImages[2]  = imread("src_images/beach_3.jpg");
	srcImages[3]  = imread("src_images/mountain_1.jpg");
	srcImages[4]  = imread("src_images/mountain_2.jpg");
	srcImages[5]  = imread("src_images/mountain_3.jpg");
	srcImages[6]  = imread("src_images/restaurant_1.jpg");
	srcImages[7]  = imread("src_images/restaurant_2.jpg");
	srcImages[8]  = imread("src_images/restaurant_3.jpg");
	srcImages[9]  = imread("src_images/road_1.jpg");
	srcImages[10] = imread("src_images/road_2.jpg");
	srcImages[11] = imread("src_images/road_3.jpg");

	vector<Mat> bgr_planes;
	int histSize = 256;
	float rangeSize = 256;
	float range[] = {0, 256};
	const float* histRange = {range};
	bool uniform = true;
	bool accumulate = false;
	Mat b_hist, g_hist, r_hist;
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	for(int i=0; i<12; i++){
		/* Split to three single-channel image and calculate the Histogram */
		split(srcImages[i], bgr_planes);
		calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
		calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
		calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

		/* Normalize */
		normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
		normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
		normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

		/* Draw lines on histogram */
		for(int j=1; j<histSize; j++){
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

		float sum_b = 0;
		float sum_g = 0;
		float sum_r = 0;
		for(int k=0; k<histSize; k++){
			sum_b = sum_b + b_hist.at<float>(k);
			sum_g = sum_g + g_hist.at<float>(k);
			sum_r = sum_r + r_hist.at<float>(k);
		}
		sum_b = sum_b / histSize;
		sum_g = sum_g / histSize;
		sum_r = sum_r / histSize;
		cout<<"Average of blue:  "<<sum_b<<endl;
		cout<<"Average of green: "<<sum_g<<endl;
		cout<<"Average of red:   "<<sum_r<<endl;
		cout<<endl;
		
	}


	/*
	split(srcImage, bgr_planes);
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	

	for(int i=1; i<histSize; i++){
    
		line(histImage, Point(bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1))),
			Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);

        
		line(histImage, Point(bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1))),
			Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);

        
		line(histImage, Point(bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1))),
			Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}


	float sum_b = 0;
	float sum_g = 0;
	float sum_r = 0;
	for(int i=0; i<histSize; i++){
		sum_b = sum_b + b_hist.at<float>(i);
		sum_g = sum_g + g_hist.at<float>(i);
		sum_r = sum_r + r_hist.at<float>(i);
	}
	sum_b = sum_b / histSize;
	sum_g = sum_g / histSize;
	sum_r = sum_r / histSize;
	cout<<"Average of blue:  "<<sum_b<<endl;
	cout<<"Average of green: "<<sum_g<<endl;
	cout<<"Average of red:   "<<sum_r<<endl;
	*/

	// imshow("srcImage", srcImage);
	// imshow("histImage", histImage);

	waitKey(0);
	return 0;
}