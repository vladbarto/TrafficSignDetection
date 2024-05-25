#include "TSR.h"
#include "stdafx.h"
#include "common.h"
#include <opencv2/opencv.hpp>
#include <random>

using namespace std;
using namespace cv;

void BGR2HSV(Mat src, Mat H, Mat S, Mat V) {

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
				(s > 90) &&
				(v > 70))
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


	//imshow("image", src);
	//imshow("dilatare", dst);
	//imshow("inchidere", dst2);

	return dst2;
}

Mat douaTreceri(Mat img, int *labelSize) // asta merge
{
	Mat dst = Mat::zeros(img.size(), CV_8UC3);
	Mat labels = Mat::zeros(img.size(), CV_32SC1);

	int label = 0;
	int height = img.rows;
	int width = img.cols;

	// Direction vectors for 8-neighborhood
	int di[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int dj[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

	// First pass: assign initial labels and record equivalences
	vector<vector<int>> edges(500);
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			if (img.at<uchar>(i, j) == 255 && labels.at<int>(i, j) == 0) {
				vector<int> L;

				for (int d = 0; d < 8; d++) {
					if (labels.at<int>(i + di[d], j + dj[d]) > 0) {
						L.push_back(labels.at<int>(i + di[d], j + dj[d]));
					}
				}

				if (L.empty()) {
					label++;
					labels.at<int>(i, j) = label;
				}
				else {
					int x = *min_element(L.begin(), L.end());
					labels.at<int>(i, j) = x;

					for (int y : L) {
						if (y != x) {
							edges[x].push_back(y);
							edges[y].push_back(x);
						}
					}
				}
			}
		}
	}

	// Second pass: resolve equivalences using BFS
	vector<int> newLabels(label + 1, 0);
	int newLabel = 0;

	for (int i = 1; i <= label; i++) {
		if (newLabels[i] == 0) {
			newLabel++;
			queue<int> q;
			newLabels[i] = newLabel;
			q.push(i);

			while (!q.empty()) {
				int x = q.front();
				q.pop();

				for (int y : edges[x]) {
					if (newLabels[y] == 0) {
						newLabels[y] = newLabel;
						q.push(y);
					}
				}
			}
		}
	}

	// Reassign labels based on the new labeling
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			labels.at<int>(i, j) = newLabels[labels.at<int>(i, j)];
		}
	}

	*labelSize = newLabel;

	// Generate random colors
	vector<Vec3b> colors(newLabel + 1);
	default_random_engine eng;
	uniform_int_distribution<int> distrib(0, 255);
	for (int i = 0; i <= newLabel; i++) {
		colors[i] = Vec3b(distrib(eng), distrib(eng), distrib(eng));
	}

	// Color the result image based on the labels
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (labels.at<int>(i, j) == 0) {
				dst.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
			}
			else {
				dst.at<Vec3b>(i, j) = colors[labels.at<int>(i, j)];
			}
		}
	}

	//imshow("Original Image", img);
	imshow("Labeled Image", dst);

	return labels;
}

Mat deleteSmallObj(Mat labels, Mat src, int labelSize) {
	vector<int> aria(labelSize + 1, 0);
	//cout << labelSize << endl;
	int minSize = 800;

	int label = 0;
	int height = src.rows;
	int width = src.cols;

	Mat dst = Mat(height, width, CV_8UC1);
	src.copyTo(dst);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (labels.at<int>(i, j) != 0) {
				aria[labels.at<int>(i, j)]++;
			}
		}
	}

	for (int i = 0; i < labelSize + 1; i++) {
		cout << aria[i] << endl;
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (aria[labels.at<int>(i, j)] < minSize) {
				dst.at<uchar>(i, j) = 0;
			}
		}
	}
	
	return dst;
}

void detectOctogon(Mat& resultImage, const vector<vector<Point>>& contours, int index) {
	vector<Point> approx;
	double epsilon = 0.02 * arcLength(contours[index], true);
	approxPolyDP(contours[index], approx, epsilon, true);

	if (approx.size() == 8) {
		drawContours(resultImage, vector<vector<Point>>{approx}, -1, Scalar(0, 255, 0), 2);
	}
}

void detectRectangle(Mat& resultImage, const vector<vector<Point>>& contours, int index) {
	vector<Point> approx;
	double epsilon = 0.02 * arcLength(contours[index], true);
	approxPolyDP(contours[index], approx, epsilon, true);

	if (approx.size() == 4) {
		drawContours(resultImage, vector<vector<Point>>{approx}, -1, Scalar(0, 255, 0), 2);
	}
	else {
		detectOctogon(resultImage, contours, index);
	}
}

void detectTriangles(Mat& resultImage, const vector<vector<Point>>& contours, int index) {
	vector<Point> approx;
	double epsilon = 0.02 * arcLength(contours[index], true);
	approxPolyDP(contours[index], approx, epsilon, true);

	if (approx.size() == 3) {
		drawContours(resultImage, vector<vector<Point>>{approx}, -1, Scalar(0, 255, 0), 2);
	}
	else {
		detectRectangle(resultImage, contours, index);
	}
}

void detectShapes(Mat src) {
	Mat resultImage = Mat::zeros(src.size(), CV_8UC3);
	cvtColor(src, resultImage, COLOR_GRAY2BGR);

	vector<vector<Point>> contours;
	findContours(src.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); ++i) {
		if (contours[i].size() < 5) continue; // fitEllipse needs at least 5 points
		RotatedRect ell = fitEllipse(contours[i]);

		Mat1b maskContour(src.rows, src.cols, uchar(0));
		drawContours(maskContour, contours, i, Scalar(255), 2);

		Mat1b maskEllipse(src.rows, src.cols, uchar(0));
		ellipse(maskEllipse, ell, Scalar(255), 2);

		Mat1b intersection = maskContour & maskEllipse;

		// Count amount of intersection
		float cnz = countNonZero(intersection);
		// Count number of pixels in the drawn contour
		float n = countNonZero(maskContour);
		// Compute your measure
		float measure = cnz / n;

		if (measure >= 0.6) {
			ellipse(resultImage, ell, Scalar(0, measure * 255, 255 - measure * 255), 3);
		}
		else {
			detectTriangles(resultImage, contours, i);
		}
	}

	imshow("Detected Shapes", resultImage);
}

/* cod smecher s-ar putea sa mearga de data asta*/
double perpendicularDistance(const Point& pt, const Point& lineStart, const Point& lineEnd) {
	double dx = lineEnd.x - lineStart.x;
	double dy = lineEnd.y - lineStart.y;
	double mag = std::sqrt(dx * dx + dy * dy);
	if (mag > 0.0) {
		dx /= mag;
		dy /= mag;
	}

	double pvx = pt.x - lineStart.x;
	double pvy = pt.y - lineStart.y;

	double pvDot = dx * pvx + dy * pvy;
	double ax = pvDot * dx;
	double ay = pvDot * dy;

	double perpendicularDist = std::sqrt((pvx - ax) * (pvx - ax) + (pvy - ay) * (pvy - ay));
	return perpendicularDist;
}

// Recursive function to implement the Douglas-Peucker algorithm
void DouglasPeucker(const std::vector<Point>& pointList, double epsilon, std::vector<Point>& resultList) {
	if (pointList.size() < 2) {
		throw std::invalid_argument("Not enough points to simplify");
	}

	double dmax = 0.0;
	size_t index = 0;
	size_t end = pointList.size() - 1;

	for (size_t i = 1; i < end; ++i) {
		double d = perpendicularDistance(pointList[i], pointList[0], pointList[end]);
		if (d > dmax) {
			index = i;
			dmax = d;
		}
	}

	if (dmax > epsilon) {
		std::vector<Point> recResults1;
		std::vector<Point> recResults2;
		std::vector<Point> firstLine(pointList.begin(), pointList.begin() + index + 1);
		std::vector<Point> lastLine(pointList.begin() + index, pointList.end());

		DouglasPeucker(firstLine, epsilon, recResults1);
		DouglasPeucker(lastLine, epsilon, recResults2);

		resultList.assign(recResults1.begin(), recResults1.end() - 1);
		resultList.insert(resultList.end(), recResults2.begin(), recResults2.end());
	}
	else {
		resultList.clear();
		resultList.push_back(pointList[0]);
		resultList.push_back(pointList[end]);
	}
}

void approxPolyDP(const std::vector<Point>& curve, std::vector<Point>& approxCurve, double epsilon, bool closed) {
	if (curve.empty()) {
		return;
	}

	std::vector<Point> curveCopy = curve;
	if (closed && !(curve.front() == curve.back())) {
		curveCopy.push_back(curve.front());
	}

	DouglasPeucker(curveCopy, epsilon, approxCurve);

	if (closed) {
		approxCurve.push_back(approxCurve.front());
	}
}

//int main() {
//	std::vector<Point> curve = { Point(0, 0), Point(1, 2), Point(2, 1), Point(3, 3), Point(4, 0) };
//	std::vector<Point> approxCurve;
//	double epsilon = 1.0;
//	bool closed = true;
//
//	approxPolyDP(curve, approxCurve, epsilon, closed);
//
//	std::cout << "Approximated curve: \n";
//	for (const auto& pt : approxCurve) {
//		std::cout << "(" << pt.x << ", " << pt.y << ")\n";
//	}
//
//	return 0;
//}