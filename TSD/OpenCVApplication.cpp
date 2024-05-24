// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include "Demo.h"
#include "TSR.h"

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
		//printf(" 1 - Open image\n");
		//printf(" 2 - Open BMP images from folder\n");
		//printf(" 3 - Image negative - diblook style\n");
		printf(" 4 - BGR->HSV\n");
		//printf(" 5 - Resize image\n");
		//printf(" 6 - Canny edge detection\n");
		//printf(" 7 - Edges in a video sequence\n");
		//printf(" 8 - Snap frame from live video\n");
		//printf(" 9 - Mouse callback demo\n");
		printf(" 0 - Exit\n\n");
		printf("Option: ");
		scanf("%d",&op);
		switch (op)
		{
			//case 1:
			//	testOpenImage();
			//	break;
			//case 2:
			//	testOpenImagesFld();
			//	break;
			//case 3:
			//	testParcurgereSimplaDiblookStyle(); //diblook style
			//	break;
			case 4:
				char fname[MAX_PATH];
				while (openFileDlg(fname))
				{
					Mat src = imread(fname);
					int height = src.rows;
					int width = src.cols;

					// Componentele d eculoare ale modelului HSV
					Mat H = Mat(height, width, CV_8UC1);
					Mat S = Mat(height, width, CV_8UC1);
					Mat V = Mat(height, width, CV_8UC1);
					Mat redMat = Mat(height, width, CV_8UC1);
					Mat blueMat = Mat(height, width, CV_8UC1);
					Mat redMatClosed = Mat(height, width, CV_8UC1);
					Mat blueMatClosed = Mat(height, width, CV_8UC1);

					BGR2HSV(src, H, S, V);
					redMat = filterbyRed(H, S, V);
					blueMat = filterbyBlue(H, S, V);
					redMatClosed = inchidere(redMat, 31);
					blueMatClosed = inchidere(blueMat, 11);

					imshow("input image", src);
					//imshow("H", H);
					//imshow("S", S);
					//imshow("V", V);
					imshow("filter_red", redMat);
					imshow("filter_blue", blueMat);
					imshow("filter_red_closed", redMatClosed);
					imshow("filter_blue_closed", blueMatClosed);


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