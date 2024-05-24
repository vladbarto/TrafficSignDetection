// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include "Demo.h"
#include "TSR.h"

#define TO_RADIANS (CV_PI / 180.0)

struct peak {
	int theta, ro, hval;
	bool operator < (const peak& o) const {
		return hval > o.hval;
	}
};

int inside(Mat src, int inputI, int inputJ) {
	if (inputI >= 0 && inputI < src.rows && inputJ >= 0 && inputJ < src.cols)
		return 1;

	return 0;
}

Mat urmarireContur(Mat src)
{
	int di[8] = { 0,-1,-1,-1,0,1,1,1 };
	int dj[8] = { 1,1,0,-1,-1,-1,0,1 };
	
	Mat src_color = Mat::zeros(src.size(), CV_8UC1);

	int height = src.rows;
	int width = src.cols;
	int dir = 7;

	Point2d p1;
	int i_aux, j_aux;

	int forda = 1;
	for (int i = 0; i < height && forda; i++)
		for (int j = 0; j < width; j++)
		{
			if (src.at<uchar>(i, j) == 0)
			{
				src_color.at<uchar>(i, j) = 255;
				src.at<uchar>(i, j) = 0;
				p1 = Point2d(i, j);
				int k = 0;
				int not_finished = 1;
				
				while (not_finished)
				{
					if (dir % 2 == 0)
						dir = (dir + 7) % 8;
					else
						dir = (dir + 6) % 8;

					while (1)
					{
						if (inside(src, i, j) && p1.x == i && p1.y == j && k != 0)
						{
							not_finished = 0;
							break;
						}

						if (inside(src, i + di[dir], j + dj[dir]) && src.at<uchar>(i + di[dir], j + dj[dir]) == 0)
						{
							src_color.at<uchar>(i + di[dir], j + dj[dir]) = 255;
							i = i + di[dir];
							j = j + dj[dir];
							k++;
							break;
						}
						else
							dir = (dir + 1) % 8;
					}
				}
				forda = 0;
				break;
			}
		}

	return src_color;
}

Mat houghTransform(Mat src) {
	uchar PIXEL_OBIECT = 0;
	uchar PIXEL_FUNDAL = 255;
	uchar PIXEL_CONTUR = 255;
	
	uchar RO_MAX = 144;
	uchar THETA_MAX = 360;

		Mat hough = Mat::zeros(RO_MAX+1, THETA_MAX+1, CV_8UC1);
		int ro, theta;
		
		int height = src.rows, width = src.cols;
		Mat contur = urmarireContur(src);
		
		for (int x = 0; x < height; x++) {
			for (int y = 0; y < width; y++) {
				
				if (contur.at<uchar>(x, y) == PIXEL_CONTUR) {
					for (theta = 0; theta < THETA_MAX; theta+=1) { // pas delta theta = 20
						ro = x * cos(theta*TO_RADIANS) + y * sin(theta*TO_RADIANS);
						if (ro < RO_MAX) {
							hough.at<uchar>(ro, theta)++;
						}
					}
				}
			}
		}
		double min, max;
		cv::minMaxLoc(hough, &min, &max);

		Mat houghImg;
		hough.convertTo(houghImg, CV_8UC1,
			255.f / max);

		/// PANA AICI AM OBTINUT ACUMULATORUL HOUGH
		std::vector<peak> peaks;

		// Detect peaks
		for (int r = 0; r <= RO_MAX; ++r) {
			for (int t = 0; t <= THETA_MAX; ++t) {
				if (hough.at<uchar>(r, t) > 0) {
					peaks.push_back({ t, r, static_cast<int>(hough.at<uchar>(r, t)) });
				}
			}
		}

		// Sort peaks in descending order of hval
		std::sort(peaks.begin(), peaks.end());

		// Print or use the detected peaks as needed
		for (const auto& p : peaks) {
			std::cout << "Peak at (theta=" << p.theta << ", ro=" << p.ro << "), hval=" << p.hval << std::endl;
		}

		// Thresholding peaks (optional)
		const int threshold = 6;	//50; // Adjust this threshold according to your needs
		std::vector<peak> filteredPeaks;
		for (const auto& p : peaks) {
			if (p.hval >= threshold) {
				filteredPeaks.push_back(p);
			}
		}

		// Grouping peaks
		const int proximityThreshold = 10; // Adjust this threshold according to your needs
		std::vector<std::vector<peak>> edgeGroups;
		for (const auto& p : filteredPeaks) {
			bool foundGroup = false;
			for (auto& group : edgeGroups) {
				for (const auto& peakInGroup : group) {
					if (std::abs(peakInGroup.theta - p.theta) <= proximityThreshold &&
						std::abs(peakInGroup.ro - p.ro) <= proximityThreshold) {
						group.push_back(p);
						foundGroup = true;
						break;
					}
				}
				if (foundGroup) break;
			}
			if (!foundGroup) {
				edgeGroups.push_back({ p });
			}
		}

		// Counting groups (number of edges)
		int numberOfEdges = edgeGroups.size();
		std::cout << "Number of edges: " << numberOfEdges << std::endl;
		
		
		imshow("Contur", contur);
		waitKey();

		return houghImg;
}

void testHoughTransform() {

	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src;
		src = imread(fname, IMREAD_GRAYSCALE);

		
		Mat houghImg = houghTransform(src);
		imshow("source", src);
		imshow("hough acc", houghImg);

		waitKey();
	}
}

void testBoundingBox() {

	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src;
		src = imread(fname, IMREAD_GRAYSCALE);


		drawBoundingBox(src);
		//imshow("source", src);

		//waitKey(0);
	}
}

void callPipeline() {
	//BGR2HSV();
	//testHoughTransform();
	testBoundingBox();
}

int main()
{
	print();
	int op;
	//do
	//{
	//	system("cls");
	//	destroyAllWindows();
	//	printf("Menu:\n");
	//	printf(" 1 - Open image\n");
	//	printf(" 2 - Open BMP images from folder\n");
	//	printf(" 3 - Image negative - diblook style\n");
	//	printf(" 4 - BGR->HSV\n");
	//	printf(" 5 - Resize image\n");
	//	printf(" 6 - Canny edge detection\n");
	//	printf(" 7 - Edges in a video sequence\n");
	//	printf(" 8 - Snap frame from live video\n");
	//	printf(" 9 - Mouse callback demo\n");
	//	printf(" 0 - Exit\n\n");
	//	printf("Option: ");
	//	scanf("%d",&op);
	//	switch (op)
	//	{
	//		case 1:
	//			testOpenImage();
	//			break;
	//		case 2:
	//			testOpenImagesFld();
	//			break;
	//		case 3:
	//			testParcurgereSimplaDiblookStyle(); //diblook style
	//			break;
	//		case 4:
	//			//testColor2Gray();
	//			testBGR2HSV();
	//			break;
	//		case 5:
	//			testResize();
	//			break;
	//		case 6:
	//			testCanny();
	//			break;
	//		case 7:
	//			testVideoSequence();
	//			break;
	//		case 8:
	//			testSnap();
	//			break;
	//		case 9:
	//			testMouseClick();
	//			break;
	//	}
	//}
	//while (op!=0);

	// pipeline:
	callPipeline();
	return 0;
}