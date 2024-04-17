#include "TSR.h"
#include "stdafx.h"
#include "common.h"

void BGR2HSV() {
	/**
	* TODO:
	* - fix the function
	* - cumva sa returneze cele 3 matrice H, S si V
	* Pentru a testa outputul (cum ar trebui sa arate): https://onlinetools.com/image/show-hsv-image-colors
	*/

	char fname[MAX_PATH];
	Mat ausgabebild;
	while (openFileDlg(fname))
	{
		Mat src;
		src = imread(fname, CV_LOAD_IMAGE_COLOR);
		imshow("Eingabebild", src);
		ausgabebild = Mat::zeros(src.rows, src.cols, CV_8UC3);

		Mat H, S, V;
		H = Mat::zeros(src.rows, src.cols, CV_8UC1);
		S = Mat::zeros(src.rows, src.cols, CV_8UC1);
		V = Mat::zeros(src.rows, src.cols, CV_8UC1);

		for (int i = 0; i < src.rows; i++)
			for (int j = 0; j < src.cols; j++)
			{
				int rotKanal = src.at<Vec3b>(i, j)[2];
				int grunKanal = src.at<Vec3b>(i, j)[1];
				int blauKanal = src.at<Vec3b>(i, j)[0];

				int maximalesWert = max(rotKanal, max(grunKanal, blauKanal));
				int minimalesWert = min(rotKanal, min(grunKanal, blauKanal));

				int C = maximalesWert - minimalesWert;
				// Value / Helligkeit
				V.at<uchar>(i, j) = maximalesWert;

				//// Saturation / Sättigung
				int currentV = maximalesWert;
				if (currentV != 0)
					S.at<uchar>(i, j) = C / currentV;
				else S.at<uchar>(i, j) = 0;

				// Hue / Farbton
				if (C != 0) {
					if (maximalesWert == rotKanal)
						H.at<uchar>(i, j) = 60 * (grunKanal - blauKanal) / C;
					if (maximalesWert == grunKanal)
						H.at<uchar>(i, j) = 120 + 60 * (blauKanal - rotKanal) / C;
					if (maximalesWert == blauKanal)
						H.at<uchar>(i, j) = 240 + 60 * (rotKanal - grunKanal) / C;
				}
				else
					H.at<uchar>(i, j) = 0;
				if (H.at<uchar>(i, j) < 0)
					H.at<uchar>(i, j) += 360;

				// Normalisierung
				H.at<uchar>(i, j) = H.at<uchar>(i, j) * 255 / 360;
				S.at<uchar>(i, j) *= 255;
				V.at<uchar>(i, j) *= 255;
			}

		namedWindow("H Channel", CV_WINDOW_AUTOSIZE);
		cvMoveWindow("H Channel", 20, 0);

		namedWindow("S Channel", CV_WINDOW_AUTOSIZE);
		cvMoveWindow("S Channel", H.cols + 20, 0);

		namedWindow("V Channel", CV_WINDOW_AUTOSIZE);
		cvMoveWindow("V Channel", H.cols + S.cols + 20, 0);

		imshow("H Channel", H);
		imshow("S Channel", S);
		imshow("V Channel", V);
		waitKey(0);
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
