/*
 * RectangleSpotDetector.h
 *
 *  Created on: 2013.11.24.
 *      Author: Balint
 */

#ifndef RECTANGLESPOTDETECTOR_H_
#define RECTANGLESPOTDETECTOR_H_

#include "SpotDetector.h"

// Belsõ négyzet paramétere
const int SQUARE_HALFWIDTH = 4; // ha 1 akkor 3x3-as négyzet lesz.
// Belsõ és külsõ keret távolsága
const int OUTER_FRAME_SEP = 3; // ennyi nem vizsgált pixel lesz köztük!

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
