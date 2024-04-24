#pragma once
#include <iostream>

void print();
void testOpenImage();
void testOpenImagesFld();
void testImageOpenAndSave();
void testNegativeImage();
void testParcurgereSimplaDiblookStyle();
void testColor2Gray();
void testBGR2HSV();
void testResize();
void testCanny();
void testVideoSequence();
void testSnap();
void MyCallBackFunc(int event, int x, int y, int flags, void* param);
void testMouseClick();
void showHistogram(const std::string& name, int* hist, const int  hist_cols, const int hist_height);