/*
 * RectangleSpotDetector.h
 *
 *  Created on: 2013.11.24.
 *      Author: Balint
 */

#ifndef SQUARE2SPOTDETECTOR_H_
#define SQUARE2SPOTDETECTOR_H_

#include "SpotDetector.h"

// Belsõ négyzet paramétere
const int SPOT_HALFWIDTH = 2; // 4
const int AURA_HALFWIDTH = 3; // 6
const int OUTER_HALFWIDTH = 5; // 10

const int GREEN_AVG_DELTA = 30;

class Square2SpotDetector: public SpotDetector {
	uchar avgOfSpotAt(const Mat& img, int x, int y);
	Vec3b avg3OfAuraAt(const Mat& orig, int x, int y);
	uchar avgOfOuterAt(const Mat& img, int x, int y);
public:
	Square2SpotDetector();
	virtual ~Square2SpotDetector();
	Mat detect(uchar delta, const Mat& greyscaleImage);
	Mat detect(uchar delta, const Mat& greyscaleImage, const Mat& orig);
};

#endif /* RECTANGLESPOTDETECTOR_H_ */
