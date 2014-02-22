/*
 * PixelSpotDetector.cpp
 *
 *  Created on: 2013.11.24.
 *      Author: Balint
 */

#include "PixelSpotDetector.h"

PixelSpotDetector::PixelSpotDetector() {
	// TODO Auto-generated constructor stub

}

PixelSpotDetector::~PixelSpotDetector() {
	// TODO Auto-generated destructor stub
}

Mat PixelSpotDetector::detect(uchar delta, const Mat& img) {
	Mat result(img.rows, img.cols, img.type());

	for (int i = 0; i < img.cols; i++) {
		for (int j = 0; j < img.rows; j++) {
			long aveWithD = this->ave(img, i, j) + delta;
			long medWithD = this->med(img, i, j) + delta;

			if (img.at<uchar>(j, i) > aveWithD
					&& img.at<uchar>(j, i) > medWithD) {
				result.at<uchar>(j, i) = 255;
			} else {
				result.at<uchar>(j, i) = 0;
			}
		}
	}

	return result;
}

Mat PixelSpotDetector::detect(uchar delta, const Mat& img, const Mat& orig) {
	return this->detect(delta, img);
}

uchar PixelSpotDetector::ave(const Mat& img, int k, int l) {
	long avg = 0;
	int count = 0;

	for (int z = -W; z <= W; z++) {
		if (k + z >= 0 && k + z < img.cols && l + z >= 0 && l + z < img.rows
				&& z != 0) {
			// az ablakon belül vagyunk.
			avg += img.at<uchar>(l + z, k + z);
			count++;
		}
	}

	if (count == 0) {
		return 0;
	}

	return avg / count;
}

uchar PixelSpotDetector::med(const Mat& img, int k, int l) {
	int i = 0;
	for (int z = -W; z <= +W; z++) {
		if ((k + z) >= 0 && (k + z) < img.cols && (l + z) >= 0
				&& (l + z) < img.rows && z != 0) {
			i++;
		}
	}

	uchar array[i];

	if (i == 0) {
		return 0;
	}

	i = 0;
	for (int z = -W; z <= +W; z++) {
		if ((k + z) >= 0 && (k + z) < img.cols && (l + z) >= 0
				&& (l + z) < img.rows && z != 0) {
			array[i++] = img.at<uchar>(l + z, k + z);
		}
	}

	std::sort(&array[0], array + i);

	if (i % 2 == 1) {
		return array[i / 2];
	} else {
		return (array[i / 2 - 1] + array[i / 2]) / 2;
	}
}
