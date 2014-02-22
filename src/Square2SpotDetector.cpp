/*
 * RectangleSpotDetector.cpp
 *
 *  Created on: 2013.11.24.
 *      Author: Balint
 */

#include "Square2SpotDetector.h"
#include "iostream"

Square2SpotDetector::Square2SpotDetector() {
	// TODO Auto-generated constructor stub

}

Square2SpotDetector::~Square2SpotDetector() {
	// TODO Auto-generated destructor stub
}

Mat Square2SpotDetector::detect(uchar delta, const Mat& img) {
	return Mat(Size(0, 0), CV_8U);
}

Mat Square2SpotDetector::detect(uchar delta, const Mat& img, const Mat& orig) {
	Mat result(img.rows, img.cols, img.type());

	uchar SPOT_THRESHOLD = 230;
	uchar SPOT_OUTER_DELTA = 50;

	for (int i = OUTER_HALFWIDTH; i < img.cols - OUTER_HALFWIDTH; i++) {
		for (int j = OUTER_HALFWIDTH; j < img.rows - OUTER_HALFWIDTH; j++) {
			uchar spotAvg = this->avgOfSpotAt(img, i, j);
			uchar outerAvg = this->avgOfOuterAt(img, i, j);
			if (spotAvg > SPOT_THRESHOLD
					&& spotAvg > outerAvg + SPOT_OUTER_DELTA) {

				Vec3b avg = avg3OfAuraAt(orig, i, j);
				if ((avg.val[2] + avg.val[0]) / 2 + GREEN_AVG_DELTA
						< avg.val[1]) {
					result.at<uchar>(j, i) = 255;
				} else {
					result.at<uchar>(j, i) = 0;
				}
			} else {
				result.at<uchar>(j, i) = 0;
			}
		}
	}

	return result;
}

uchar Square2SpotDetector::avgOfSpotAt(const Mat& img, int x, int y) {
	long avg = 0;
	int count = 0;

	for (int k = x - SPOT_HALFWIDTH; k <= x + SPOT_HALFWIDTH; k++) {
		for (int l = y - SPOT_HALFWIDTH; l <= y + SPOT_HALFWIDTH; l++) {
			avg += img.at<uchar>(l, k);
			count++;
		}
	}

	return avg / count;
}

Vec3b Square2SpotDetector::avg3OfAuraAt(const Mat& orig, int x, int y) {
	long sum[3];
	sum[0] = 0;
	sum[1] = 0;
	sum[2] = 0;
	int count = 0;

	for (int k = x - AURA_HALFWIDTH; k <= x + AURA_HALFWIDTH; k++) {
		for (int l = y - AURA_HALFWIDTH; l <= y + AURA_HALFWIDTH; l++) {
			const uchar * pix = orig.at<Vec3b>(l, k).val;
			for (int k = 0; k < 3; k++) {
				sum[k] += pix[k];
			}
			count++;
		}
	}

	return Vec3b(sum[0] / count, sum[1] / count, sum[2] / count);
}

uchar Square2SpotDetector::avgOfOuterAt(const Mat& img, int x, int y) {
	long avg = 0;
	int count = 0;

	for (int i = -OUTER_HALFWIDTH; i <= +OUTER_HALFWIDTH; i++) {
		for (int j = -OUTER_HALFWIDTH; j <= +OUTER_HALFWIDTH; j++) {
			if (i < -AURA_HALFWIDTH || i > AURA_HALFWIDTH || j < -AURA_HALFWIDTH
					|| j > AURA_HALFWIDTH) {
				avg += img.at<uchar>(y + j, x + i);
				count++;
			}
		}
	}

	return avg / count;
}
