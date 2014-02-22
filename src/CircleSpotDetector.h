/*
 * RectangleSpotDetector.h
 *
 *  Created on: 2013.11.24.
 *      Author: Balint
 */

#ifndef CIRCLESPOTDETECTOR_H_
#define CIRCLESPOTDETECTOR_H_

#include "SpotDetector.h"

class CircleSpotDetector: public SpotDetector {
	Size sizeOfMask;

	/**
	 * Meghatározza, hogy egy (x, y) pont beleesik-e egy (0,0) középpontú r sugarú körbe
	 *
	 * @param x
	 * @param y
	 * @param r
	 * @return
	 */
	bool inCircle(int x, int y, int r);
	int euclideanDistance(const Vec3b& a, const Vec3b& b);
	uchar getAvgAtMask(const Mat& img, const Mat& mask, int x, int y);
	Vec3b getAvg3AtMask(const Mat& img, const Mat& mask, int x, int y);
public:
	Mat spotMask, auraMask, outerMask;
	CircleSpotDetector();
	virtual ~CircleSpotDetector();
	Mat detect(uchar delta, const Mat& greyscaleImage);
	Mat detect(uchar delta, const Mat& greyscaleImage, const Mat& originalImage);
};

#endif /* RECTANGLESPOTDETECTOR_H_ */
