// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include "Demo.h"
#include "TSR.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

//
//void callPipeline() {
//	BGR2HSV();
//}

int main()
{
	print();
	int op;
	do
	{
		system("cls");
		destroyAllWindows();
		printf("Menu:\n");
		printf(" 1 - Magie\n");
		printf(" 0 - Exit\n\n");
		printf("Option: ");
		scanf("%d",&op);
		switch (op)
		{
			case 1:
				char fname[MAX_PATH];
				while (openFileDlg(fname))
				{
					Mat src = imread(fname);
					int height = src.rows;
					int width = src.cols;
					int labelSize;

					// Componentele d eculoare ale modelului HSV
					Mat H = Mat(height, width, CV_8UC1);
					Mat S = Mat(height, width, CV_8UC1);
					Mat V = Mat(height, width, CV_8UC1);
					Mat redMat = Mat(height, width, CV_8UC1);
					Mat blueMat = Mat(height, width, CV_8UC1);
					Mat redMatClosed = Mat(height, width, CV_8UC1);
					Mat blueMatClosed = Mat(height, width, CV_8UC1);
					Mat labels = Mat(height, width, CV_32SC1);
					Mat cleanRed = Mat(height, width, CV_8UC1);

					BGR2HSV(src, H, S, V);
					redMat = filterbyRed(H, S, V);
					blueMat = filterbyBlue(H, S, V);
					labels = douaTreceri(redMat, &labelSize);
					cleanRed = deleteSmallObj(labels, redMat, labelSize);
					redMatClosed = inchidere(cleanRed, 5);
					//blueMatClosed = inchidere(blueMat, 11);
					
					detectShapes(redMatClosed);

					imshow("input image", src);
					//imshow("H", H);
					//imshow("S", S);
					//imshow("V", V);
					imshow("filter_red", redMat);
					imshow("clean_red", cleanRed);
					/*imshow("filter_blue", blueMat);
					imshow("filter_red_closed", redMatClosed);*/
					//imshow("filter_blue_closed", blueMatClosed);
					//imshow("sharp", redSharp);
					//std::cout << countEdges(redSharp) << std::endl;

					waitKey();
				}
				
				break;
			//case 5:
			//	testResize();
			//	break;
			//case 6:
			//	testCanny();
			//	break;
			//case 7:
			//	testVideoSequence();
			//	break;
			//case 8:
			//	testSnap();
			//	break;
			//case 9:
			//	testMouseClick();
			//	break;
		}
	}
	while (op!=0);

	//pipeline:
	//	callPipeline();
	return 0;
}


