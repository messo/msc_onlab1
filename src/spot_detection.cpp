#include <stdio.h>
#include <iostream>
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "SquareSpotDetector.h"
#include "PixelSpotDetector.h"
#include "CircleSpotDetector.h"
#include "Square2SpotDetector.h"

using namespace cv;

int main(int argc, char** argv) {
	VideoCapture inputCapture;
	inputCapture.open(0);
	//std::cout << inputCapture.get(CV_CAP_PROP_BRIGHTNESS) << "\n";
	//std::cout << inputCapture.get(CV_CAP_PROP_BACKLIGHT);
	std::cout.flush();
	// inputCapture.set(CV_CAP_PROP_BACKLIGHT, 0);
	// std::cout << inputCapture.get(CV_CAP_PROP_);

	//Mat imgLeft = imread("laser3.png", CV_LOAD_IMAGE_COLOR);

	SpotDetector* detector = new Square2SpotDetector(); //Square2SpotDetector();

	uchar delta = 100;

	Mat image = imread("laser.png");
	//Mat greyscale;
	//cvtColor(image, greyscale, CV_BGR2GRAY);
	//Mat detected = detector->detect(delta, greyscale, imread("magic.png"));

	int x = 325;
	int y = 245;

	bool showSpotSquare = false;
	bool showOuterSquare = false;

	while (true) {
		double t0 = (double)getTickCount();
		inputCapture >> image;

		if (showSpotSquare) {
			for (int k = x - SPOT_HALFWIDTH; k <= x + SPOT_HALFWIDTH; k++) {
				for (int l = y - SPOT_HALFWIDTH; l <= y + SPOT_HALFWIDTH; l++) {
					uchar * pix = image.at<Vec3b>(l, k).val;
					pix[0] = 0;
					pix[1] = 0;
					pix[2] = 255;
				}
			}
		}
		if (showOuterSquare) {
			for (int i = -OUTER_HALFWIDTH; i <= +OUTER_HALFWIDTH; i++) {
				for (int j = -OUTER_HALFWIDTH; j <= +OUTER_HALFWIDTH; j++) {
					if (i < -AURA_HALFWIDTH || i > AURA_HALFWIDTH
							|| j < -AURA_HALFWIDTH || j > AURA_HALFWIDTH) {
						uchar * pix = image.at<Vec3b>(y + j, x + i).val;
						pix[0] = 0;
						pix[1] = 255;
						pix[2] = 0;
					}
				}
			}
		}

		double t = (double)getTickCount();

		Mat greyscale;
		Mat res(Size(320, 240), image.type());
		resize(image, res, Size(320, 240));

		cvtColor(res, greyscale, CV_BGR2GRAY);
		Mat detected = detector->detect(delta, greyscale, res);

		t = ((double)getTickCount() - t)/getTickFrequency();
		std::cout << "Speed: " << t << "\n";
		std::cout.flush();

		imshow("result", detected);

		char key = (char) waitKey(10);
		if (key == 27) {
			break;
		} else if (key == 'a') {
			// belsõ maszk (PIROS)
			Mat* mask = &(((CircleSpotDetector*) detector)->spotMask);
			int R = (mask->rows - 1) / 2;
			for (int j = -R; j <= +R; j++) {
				for (int i = -R; i <= +R; i++) {
					uchar * pix = image.at<Vec3b>(j + y, i + x).val;
					if (mask->at<uchar>(j + R, i + R) == 1) {
						pix[0] = 0;
						pix[1] = 0;
						pix[2] = 255;
					}
				}
			}
		} else if (key == 'b') {
			// aura maszk (KÉK)
			Mat* mask = &(((CircleSpotDetector*) detector)->auraMask);
			int R = (mask->rows - 1) / 2;
			for (int j = -R; j <= +R; j++) {
				for (int i = -R; i <= +R; i++) {
					uchar * pix = image.at<Vec3b>(j + y, i + x).val;
					if (mask->at<uchar>(j + R, i + R) == 1) {
						pix[0] = 255;
						pix[1] = 0;
						pix[2] = 0;
					}
				}
			}
		} else if (key == 'c') {
			// perem maszk (ZÖLD)
			Mat* mask = &(((CircleSpotDetector*) detector)->outerMask);
			int R = (mask->rows - 1) / 2;
			for (int j = -R; j <= +R; j++) {
				for (int i = -R; i <= +R; i++) {
					uchar * pix = image.at<Vec3b>(j + y, i + x).val;
					if (mask->at<uchar>(j + R, i + R) == 1) {
						pix[0] = 0;
						pix[1] = 255;
						pix[2] = 0;
					}
				}
			}
		} else if (key == 'd') {
			image = imread("magic.png");
		} else if (key == 'q') {
			showSpotSquare = !showSpotSquare;
		} else if (key == 'w') {
			showOuterSquare = !showOuterSquare;
		}

		imshow("aa", image);

		t0 = ((double)getTickCount() - t0)/getTickFrequency();
		std::cout << "FPS: " << (1.0/t0) << "\n";
	}

	delete detector;

	return 0;
}
