// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include "Demo.h"
#include "TSR.h"

int inside(Mat src, int inputI, int inputJ) {
	// daca i si j se incadreaza in proportiile imaginii sursa
	if (inputI >= 0 && inputI < src.rows && inputJ >= 0 && inputJ < src.cols)
		return 1; // 1 = true

	return 0; // 0 = false
}

Mat negativImagine(Mat src) {
	Mat dst = Mat(src.size(), CV_8UC1);

	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++) {
			if (src.at<uchar>(i, j) == 0)
				dst.at<uchar>(i, j) = 255;
			else
				dst.at<uchar>(i, j) = 0;
		}
	
	return dst;
}

Mat callPipeline(Mat src) {

	int height = src.rows;
	int width = src.cols;
	int labelSize;
	
	// crearea unor matrici pentru conversia in HSV. Imaginea se extrage pe fiecare canal in parte
	Mat H = Mat(height, width, CV_8UC1);
	Mat S = Mat(height, width, CV_8UC1);
	Mat V = Mat(height, width, CV_8UC1);

	// imaginea pe canal rosu, respectiv albastru
	Mat redMat = Mat(height, width, CV_8UC1);
	Mat blueMat = Mat(height, width, CV_8UC1);
	
	// idem, dupa procedeul de inchidere
	Mat redMatClosed = Mat(height, width, CV_8UC1);
	Mat blueMatClosed = Mat(height, width, CV_8UC1);
	
	// obiectele de pe fiecare canal etichetate
	Mat labelsBlue = Mat(height, width, CV_32SC1);
	Mat labelsRed = Mat(height, width, CV_32SC1);
	
	// imagini per fiecare canal dupa filtrarea obiectelor necorespunzatoare
	Mat cleanRed = Mat(height, width, CV_8UC1);
	Mat cleanBlue = Mat(height, width, CV_8UC1);
	
	Mat rezRed = Mat(height, width, CV_8UC1);
	Mat rezBlue = Mat(height, width, CV_8UC1);

	double t = (double)getTickCount(); // Get the current time [s]
	// conversia din spatiul RGB in HSV
	BGR2HSV(src, H, S, V);

	// extractia pe canale
	redMat = filterbyRed(H, S, V);
	blueMat = filterbyBlue(H, S, V);
	
	// etichetare si curatare obiecte pe canal red
	labelsRed = douaTreceri(redMat, &labelSize);
	cleanRed = deleteSmallObj(labelsRed, redMat, labelSize);
	redMatClosed = inchidere(cleanRed, 5);

	// etichetare si curatare obiecte pe canal red
	labelsBlue = douaTreceri(blueMat, &labelSize);
	cleanBlue = deleteSmallObj(labelsBlue, blueMat, labelSize);
	blueMatClosed = inchidere(cleanBlue, 5);

	Mat output = src.clone();
	detectShapes(redMatClosed, output, true);
	detectShapes(blueMatClosed, output, false);

	// Get the current time again and compute the time difference [s]
	t = ((double)getTickCount() - t) / getTickFrequency();
	// Print (in the console window) the processing time in [ms] 
	printf("Time = %.3f [ms]\n", t * 1000);

	return output;
}

int main()
{
	print();
	int op;
	do
	{
		system("cls");
		destroyAllWindows();
		printf("Choose the desired mode:\n");
		printf(" \t1 - Photo\n");
		printf(" \t2 - Video\n");
		printf("Press 0 to exit\n\n");
		printf("Option: ");
		scanf("%d",&op);

		char fname[MAX_PATH];
		switch (op)
		{
			case 1:
				while (openFileDlg(fname))
				{
					Mat src = imread(fname);
					Mat output = callPipeline(src);
					imshow("Detected Shapes", output);
					waitKey();
				}
				break;
			case 2:
				while (openFileDlg(fname)) {
					VideoCapture video(fname);

					// S-a deschis video-ul cu succes?
					if (!video.isOpened()) {
						cout << "Error: Unable to open the video file." << endl;
						return -1;
					}

					// Proprietatile video
					int frame_width = video.get(CAP_PROP_FRAME_WIDTH);
					int frame_height = video.get(CAP_PROP_FRAME_HEIGHT);
					double fps = video.get(CAP_PROP_FPS);
					int frame_count = video.get(CAP_PROP_FRAME_COUNT);

					// Definire codec + creare obiect VideoWriter
					VideoWriter output_video("output_video.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, Size(frame_width, frame_height));

					// S-a initializat corect obiectul video writer?
					if (!output_video.isOpened()) {
						cout << "Error: Unable to create output video file." << endl;
						return -1;
					}

					// Procesarea fiecarui frame
					Mat frame, gray;
					while (video.read(frame)) {
						// Partea de procesare

						Mat output = callPipeline(frame);

						// Scrie fiecare frame pe output
						output_video.write(output);

						// Display the frame
						imshow("Frame", output);
						if (waitKey(25) == 27) // Press Esc to exit
							break;
					}

					// Release VideoCapture si obiectele VideoWriter
					video.release();
					output_video.release();

					// Close all OpenCV windows
					destroyAllWindows();
					break;
				}
		}
	}
	while (op!=0);

	return 0;
}