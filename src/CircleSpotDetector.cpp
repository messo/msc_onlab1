/*
 * RectangleSpotDetector.cpp
 *
 *  Created on: 2013.11.24.
 *      Author: Balint
 */

#include "CircleSpotDetector.h"
#include "iostream"

// belsõ világos (fehér - kiégett)
const int SPOT_RADIUS = 3; // 5
// külsõ aura körgyûrû külsõ körének sugara (amit zöldnek keressük)
const int AURA_RADIUS = 5; // 10
// külsõ maszkba írt kör plusz sugara. (tehát mindhárom maszk egy 2*R+1 széles négyzet lesz)
const int OUTER_RADIUS = 6; // 11

const int GREEN_AVG_DELTA = 40;

bool CircleSpotDetector::inCircle(int x, int y, int r) {
	return x * x + y * y < r * r;
}

CircleSpotDetector::CircleSpotDetector() :
		sizeOfMask(OUTER_RADIUS * 2 + 1, OUTER_RADIUS * 2 + 1), outerMask(
				sizeOfMask, CV_8U), auraMask(sizeOfMask, CV_8U), spotMask(
				sizeOfMask, CV_8U) {

	for (int y = 0; y < sizeOfMask.height; y++) {
		for (int x = 0; x < sizeOfMask.width; x++) {
			// a kivonás azért kell, hogy betranszformáljuk úgy, hogy a mátrix közepe (0,0) -- koordinátageo
			if (this->inCircle(x - OUTER_RADIUS, y - OUTER_RADIUS,
					SPOT_RADIUS)) {
				spotMask.at<uchar>(y, x) = 1;
				auraMask.at<uchar>(y, x) = 1;
				outerMask.at<uchar>(y, x) = 0;
			} else {
				spotMask.at<uchar>(y, x) = 0;
				if (this->inCircle(x - OUTER_RADIUS, y - OUTER_RADIUS,
						AURA_RADIUS)) {
					// ha nincs benne a belsõben, de benne van a külsõben: akkor aura:
					auraMask.at<uchar>(y, x) = 1;
					outerMask.at<uchar>(y, x) = 0;
				} else {
					auraMask.at<uchar>(y, x) = 0;
					outerMask.at<uchar>(y, x) = 1;
				}
			}
		}
	}

	for (int y = 0; y < sizeOfMask.height; y++) {
		for (int x = 0; x < sizeOfMask.width; x++) {
			std::cout << ((spotMask.at<uchar>(y, x) == 1) ? " " : "0");
		}
		std::cout << "\n";
	}
	std::cout.flush();

	for (int y = 0; y < sizeOfMask.height; y++) {
		for (int x = 0; x < sizeOfMask.width; x++) {
			std::cout << ((auraMask.at<uchar>(y, x) == 1) ? " " : "0");
		}
		std::cout << "\n";
	}
	std::cout.flush();

	for (int y = 0; y < sizeOfMask.height; y++) {
		for (int x = 0; x < sizeOfMask.width; x++) {
			std::cout << ((outerMask.at<uchar>(y, x) == 1) ? " " : "0");
		}
		std::cout << "\n";
	}
	std::cout.flush();
}

CircleSpotDetector::~CircleSpotDetector() {
	// TODO Auto-generated destructor stub
}

uchar CircleSpotDetector::getAvgAtMask(const Mat& img, const Mat& mask, int x,
		int y) {

	long sum = 0;
	long weightSum = 0;
	int R = (mask.rows - 1) / 2;

	uchar maskV = 0;

	for (int j = -R; j <= +R; j++) {
		for (int i = -R; i <= +R; i++) {
			maskV = mask.at<uchar>(j + R, i + R);
			sum += img.at<uchar>(y + j, x + i) * maskV;
			weightSum += maskV;
		}
	}

	return sum / weightSum;
}

Vec3b CircleSpotDetector::getAvg3AtMask(const Mat& img, const Mat& mask, int x,
		int y) {

	long sum[3];
	sum[0] = 0;
	sum[1] = 0;
	sum[2] = 0;
	long weightSum = 0;
	int R = (mask.rows - 1) / 2;

	uchar maskV = 0;

	for (int j = -R; j <= +R; j++) {
		for (int i = -R; i <= +R; i++) {
			maskV = mask.at<uchar>(j + R, i + R);
			const uchar * pix = img.at<Vec3b>(y + j, x + i).val;
			for (int k = 0; k < 3; k++) {
				sum[k] += pix[k] * maskV;
			}
			weightSum += maskV;
		}
	}

	Vec3b ret;
	for (int k = 0; k < 3; k++) {
		ret.val[k] = sum[k] / weightSum;
	}

	return ret;
}

Mat CircleSpotDetector::detect(uchar delta, const Mat& img) {
	return Mat(Size(0, 0), CV_8U);
}

Mat CircleSpotDetector::detect(uchar delta, const Mat& img, const Mat& orig) {
	Mat result(img.rows, img.cols, img.type());

	uchar SPOT_THRESHOLD = 230;
	uchar SPOT_OUTER_DELTA = 50;

	//uchar delta = 115;
	for (int i = OUTER_RADIUS; i < img.cols - OUTER_RADIUS; i++) {
		for (int j = OUTER_RADIUS; j < img.rows - OUTER_RADIUS; j++) {
			uchar spotAvg = this->getAvgAtMask(img, spotMask, i, j);
			uchar outerAvg = this->getAvgAtMask(img, outerMask, i, j);

			if (spotAvg > SPOT_THRESHOLD
					&& spotAvg > outerAvg + SPOT_OUTER_DELTA) {
				// ebben a pontban számolunk egy átlagot:
				Vec3b avg = this->getAvg3AtMask(orig, auraMask, i, j);
				//std::cout << "(" << i << ", " << j << "): " << (int) avg.val[2]
				//		<< " " << (int) avg.val[1] << " " << (int) avg.val[0]
				//		<< "\n";
				//std::cout.flush();

				if ((avg.val[2] + avg.val[0]) / 2 + GREEN_AVG_DELTA
						< avg.val[1]) {
					result.at<uchar>(j, i) = 255;
				} else {
					result.at<uchar>(j, i) = 0;
				}

				// && this->euclideanDistance(
				//		this->getAvg3AtMask(orig, auraMask, i, j), REF_AURA)
				//		< RGB_DISTANCE) {
				// result.at<uchar>(j, i) = 255;
			} else {
				result.at<uchar>(j, i) = 0;
			}
		}
		//std::cout << i << "\n";
		//std::cout.flush();
	}

	return result;
}

int CircleSpotDetector::euclideanDistance(const Vec3b& a, const Vec3b& b) {
	int sum = 0;
	for (int k = 0; k < 3; k++) {
		sum += (a.val[k] - b.val[k]) * (a.val[k] - b.val[k]);
	}
	return sum;
}
