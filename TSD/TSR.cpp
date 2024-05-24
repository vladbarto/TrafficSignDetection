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

Mat filterbyRed(Mat H, Mat S, Mat V)
{
	Mat newmat(S.rows, S.cols, CV_8UC1);
	for (size_t nrow = 0; nrow < S.rows; nrow++)
	{
		for (size_t ncol = 0; ncol < S.cols; ncol++)
		{
			uchar h = H.at<uchar>(nrow, ncol);
			uchar s = S.at<uchar>(nrow, ncol);
			uchar v = V.at<uchar>(nrow, ncol);
			if (((h >= 0 && h < 15) || (h > 240 && h <= 255)) &&
				(s > 100) &&
				(v > 110))
			{
				newmat.at<uchar>(nrow, ncol) = 255;
			}
			else
			{
				newmat.at<uchar>(nrow, ncol) = 0;
			}
		}
	}
	return newmat;
}

Mat filterbyBlue(Mat H, Mat S, Mat V)
{
	Mat newmat(S.rows, S.cols, CV_8UC1);
	for (size_t nrow = 0; nrow < S.rows; nrow++)
	{
		for (size_t ncol = 0; ncol < S.cols; ncol++)
		{
			uchar h = H.at<uchar>(nrow, ncol);
			uchar s = S.at<uchar>(nrow, ncol);
			uchar v = V.at<uchar>(nrow, ncol);
			if ((h >= 140 && h <= 185) && ((s > 100) && (v > 110)))
			{
				newmat.at<uchar>(nrow, ncol) = 255;
			}
			else
			{
				newmat.at<uchar>(nrow, ncol) = 0;
			}
		}
	}
	return newmat;
}

bool isInside(Mat img, int i, int j)
{
	if ((i >= 0 && i < img.rows) && (j >= 0 && j < img.cols)) {
		return true;
	}
	else {
		return false;
	}
}

Mat inchidere(Mat src, int dim) {
	bool ok = true;
	int height = src.rows;
	int width = src.cols;
	int mij = dim / 2;

	Mat dst = Mat(height, width, CV_8UC1);
	Mat dst2 = Mat(height, width, CV_8UC1);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			dst.at<uchar>(i, j) = 0;
			dst2.at<uchar>(i, j) = 0;
		}
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (src.at<uchar>(i, j) == 255) {
				dst.at<uchar>(i, j) = 255;
				for (int k = 0; k < dim; k++) {
					for (int t = 0; t < dim; t++) {
						if (isInside(src, i + k - mij, j + t - mij) && dst.at<uchar>(i + k - mij, j + t - mij) == 0) {
							dst.at<uchar>(i + k - mij, j + t - mij) = 255;
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (dst.at<uchar>(i, j) == 255) {
				ok = false;
				for (int k = 0; k < dim; k++) {
					for (int t = 0; t < dim; t++) {
						if (!isInside(dst, i + k - mij, j + t - mij) || dst.at<uchar>(i + k - mij, j + t - mij) == 0) {
							ok = true;
						}
					}
				}
				if (ok == false) {
					dst2.at<uchar>(i, j) = 255;
				}
			}
		}
	}


	imshow("image", src);
	imshow("dilatare", dst);
	imshow("inchidere", dst2);

	return dst2;
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