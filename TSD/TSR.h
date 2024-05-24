#pragma once
#include <iostream>

void openImage();
void BGR2HSV(Mat src, Mat H, Mat S, Mat V);
Mat filterbyRed(Mat H, Mat S, Mat V);
Mat filterbyBlue(Mat H, Mat S, Mat V);
Mat inchidere(Mat src, int dim);