#pragma once
#include <iostream>
#include <list>
using namespace std;

void BGR2HSV(Mat src, Mat H, Mat S, Mat V);
Mat filterbyRed(Mat H, Mat S, Mat V);
Mat filterbyBlue(Mat H, Mat S, Mat V);
Mat inchidere(Mat src, int dim);

Mat douaTreceri(Mat img, int* labelSize);
Mat deleteSmallObj(Mat labels, Mat src, int labelSize);

void detectShapes(Mat binaryImage, Mat output, boolean isRed);