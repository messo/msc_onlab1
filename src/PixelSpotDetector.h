/*
 * PixelSpotDetector.h
 *
 *  Created on: 2013.11.24.
 *      Author: Balint
 */

#ifndef PIXELSPOTDETECTOR_H_
#define PIXELSPOTDETECTOR_H_

#include "SpotDetector.h"

const int W = 1;

class PixelSpotDetector: public SpotDetector {
	uchar ave(const Mat& img, int k, int l);
	uchar med(const Mat& img, int k, int l);
public:
	PixelSpotDetector();
	virtual ~PixelSpotDetector();
	virtual Mat detect(uchar delta, const Mat& greyscaleImage);
	virtual Mat detect(uchar delta, const Mat& greyscaleImage, const Mat& originalImage);
};

#endif /* PIXELSPOTDETECTOR_H_ */
