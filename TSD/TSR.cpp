#include "TSR.h"
#include "stdafx.h"
#include "common.h"

void BGR2HSV(Mat src, Mat H, Mat S, Mat V) {
	/**
	* TODO:
	* - fix the function
	* - cumva sa returneze cele 3 matrice H, S si V
	* Pentru a testa outputul (cum ar trebui sa arate): https://onlinetools.com/image/show-hsv-image-colors
	*/

	int height = src.rows;
	int width = src.cols;

	// definire pointeri la matricele (8 biti/pixeli) folosite la afisarea componentelor individuale H,S,V
	float r, g, b, M, m, C, h, s, v;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Vec3b v3 = src.at<Vec3b>(i, j);
			b = (float)v3[0] / 255;
			g = (float)v3[1] / 255;
			r = (float)v3[2] / 255;
			M = max(b, max(g, r));
			m = min(b, min(g, r));
			C = M - m;

			v = M;

			if (v != 0) {
				s = C / v;
			}
			else {
				s = 0;
			}
			if (C != 0) {
				if (M == r) {
					h = 60 * (g - b) / C;
				}
				if (M == g) {
					h = 120 + 60 * (b - r) / C;
				}
				if (M == b) {
					h = 240 + 60 * (r - g) / C;
				}
			}
			else {
				h = 0;
			}
			if (h < 0) {
				h = h + 360;
			}
			H.at<uchar>(i, j) = h * 255 / 360;		// lpH = 0 .. 255
			S.at<uchar>(i, j) = s * 255;			// lpS = 0 .. 255
			V.at<uchar>(i, j) = v * 255;			// lpV = 0 .. 255
		}
	}

}

void openImage() {
	char fname[MAX_PATH];
	if (openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		imshow("image", src);
		waitKey();
	}
}