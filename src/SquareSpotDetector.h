/*
 * RectangleSpotDetector.h
 *
 *  Created on: 2013.11.24.
 *      Author: Balint
 */

#ifndef RECTANGLESPOTDETECTOR_H_
#define RECTANGLESPOTDETECTOR_H_

#include "SpotDetector.h"

// Bels� n�gyzet param�tere
const int SQUARE_HALFWIDTH = 4; // ha 1 akkor 3x3-as n�gyzet lesz.
// Bels� �s k�ls� keret t�vols�ga
const int OUTER_FRAME_SEP = 3; // ennyi nem vizsg�lt pixel lesz k�zt�k!

class SquareSpotDetector: public SpotDetector {
	uchar avgOfRectangleAt(const Mat& img, int x, int y);
	uchar avgOfOuterFrameAt(const Mat& img, int x, int y);
public:
	SquareSpotDetector();
	virtual ~SquareSpotDetector();
	virtual Mat detect(uchar delta, const Mat& greyscaleImage);
	virtual Mat detect(uchar delta, const Mat& greyscaleImage, const Mat& original);
};

#endif /* RECTANGLESPOTDETECTOR_H_ */
