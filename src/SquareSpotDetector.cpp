/*
 * RectangleSpotDetector.cpp
 *
 *  Created on: 2013.11.24.
 *      Author: Balint
 */

#include "SquareSpotDetector.h"
#include "iostream"

SquareSpotDetector::SquareSpotDetector() {
	// TODO Auto-generated constructor stub

}

SquareSpotDetector::~SquareSpotDetector() {
	// TODO Auto-generated destructor stub
}

Mat SquareSpotDetector::detect(uchar delta, const Mat& img) {
	Mat result(img.rows, img.cols, img.type());

	int SEP = SQUARE_HALFWIDTH + OUTER_FRAME_SEP;

	for (int i = SEP; i < img.cols - SEP; i++) {
		for (int j = SEP; j < img.rows - SEP; j++) {
			if (this->avgOfRectangleAt(img, i, j)
					> this->avgOfOuterFrameAt(img, i, j) + delta) {
				result.at<uchar>(j, i) = 255;
			} else {
				result.at<uchar>(j, i) = 0;
			}
		}
	}

	return result;
}

Mat SquareSpotDetector::detect(uchar delta, const Mat& img, const Mat& orig) {
	return this->detect(delta, img);
}

uchar SquareSpotDetector::avgOfRectangleAt(const Mat& img, int x, int y) {
	long avg = 0;
	int count = 0;

	for (int k = x - SQUARE_HALFWIDTH; k <= x + SQUARE_HALFWIDTH; k++) {
		for (int l = y - SQUARE_HALFWIDTH; l <= y + SQUARE_HALFWIDTH; l++) {
			avg += img.at<uchar>(l, k);
			count++;
		}
	}

	return avg / count;
}

uchar SquareSpotDetector::avgOfOuterFrameAt(const Mat& img, int x, int y) {
	int d = SQUARE_HALFWIDTH + OUTER_FRAME_SEP + 1;

	long avg = 0;
	int count = 0;

	for (int k = x - d; k <= x + d; k++) {
		avg += img.at<uchar>(y - d, k);
		avg += img.at<uchar>(y + d, k);
		count += 2;
	}

	// figyelni kell, hogy a sarkokat ne számoljuk kétszer!
	for (int l = y - d + 1; l <= y + d - 1; l++) {
		avg += img.at<uchar>(l, x - d);
		avg += img.at<uchar>(l, x + d);
		count += 2;
	}

	return avg / count;
}
