/*
 * SpotDetector.h
 *
 *  Created on: 2013.11.24.
 *      Author: Balint
 */

#ifndef SPOTDETECTOR_H_
#define SPOTDETECTOR_H_

#include "opencv2/core/core.hpp"
#include <iostream>

using namespace cv;

class SpotDetector {
public:
	virtual ~SpotDetector() {};
	virtual Mat detect(uchar delta, const Mat& greyscaleImage) = 0;
	virtual Mat detect(uchar delta, const Mat& greyscaleImage, const Mat& originalImage) = 0;
};

#endif /* SPOTDETECTOR_H_ */
