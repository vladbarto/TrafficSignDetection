#include "TSR.h"
#include "stdafx.h"
#include "common.h"
#include <random>

using namespace std;
using namespace cv;

uchar PIXEL_FUNDAL = 0;
uchar PIXEL_OBIECT = 255;

bool isInside(Mat img, int i, int j)
{
	if ((i >= 0 && i < img.rows) && (j >= 0 && j < img.cols)) {
		return true;
	}
	else {
		return false;
	}
}

boolean classDecision(char text[], int classification, boolean isRed) {
	// se clasifica semnele de circulatie in functie de culoare (isRed) si forma(classification)
	/* codurile pentru forme:
	*		0: elipsa
	*		1: octogon
	*		2: triunghi
	*		3: dreptunghi
	*/
	if (isRed) {
		switch (classification)
		{
		case 0:
			strcpy(text, "Interzicere/Restrictie");
			return true;
			break;
		case 1:
			strcpy(text, "Stop");
			return true;
			break;
		case 2:
			strcpy(text, "Avertizare");
			return true;
			break;
		case 3:
			strcpy(text, "Avertizare");
			return true;
			break;
		default:
			strcpy(text, "altceva rosu");
			return false;
		}
	}
	else { // isBlue
		switch (classification)
		{
		case 0:
			strcpy(text, "Obligare");
			return true;
			break;
		case 3:
			strcpy(text, "Informare/Orientare");
			return true;
			break;
		default:
			strcpy(text, "altceva blue");
			return false;
		}
	}
	
}

void drawBoundingBox(Mat src, vector<vector<Point>> contours, boolean isRed, int index, int classification) {

	vector<Point> approx;
	double epsilon = 0.02 * arcLength(contours[index], true);
	approxPolyDP(contours[index], approx, epsilon, true);

	int numberOfEdges = approx.size();
	int xmin = src.cols, ymin = src.rows, Xmax = 0, Ymax = 0;
	for (int contur = 0; contur < contours[index].size(); contur++) {
		Point p = contours[index][contur];
		if (p.x < xmin) { xmin = p.x; }
		if (p.y < ymin) { ymin = p.y; }
		if (p.x > Xmax) { Xmax = p.x; }
		if (p.y > Ymax) { Ymax = p.y; }
	}
	CvPoint p1 = CvPoint(xmin, ymin);
	CvPoint p2 = CvPoint(Xmax, Ymax);

	Point textPosition = Point(xmin, Ymax + 25);
	float font_size = 0.75;
	int font_weight = 2;
	char text[255] = { 0 };

	boolean deseneaza = classDecision(text, classification, isRed);
	if (deseneaza == true) {
		// Seteaza dimensiunea textului
		int baseline = 0;
		Size textSize = getTextSize(text, FONT_ITALIC, font_size, font_weight, &baseline);
		baseline += font_weight;

		// Deseneaza caseta de text
		Point backgroundTopLeft = textPosition + Point(0, -textSize.height);
		Point backgroundBottomRight = textPosition + Point(textSize.width, baseline);
		rectangle(src, backgroundTopLeft, backgroundBottomRight, Scalar(0, 255, 255), FILLED);

		// Pune textul in caseta
		putText(src, text, textPosition, FONT_ITALIC, font_size, Scalar(0, 0, 255), font_weight);

		// Deseneaza bounding box
		rectangle(src, p1, p2, Scalar (0, 255, 0), 3);
	}
}


vector<Point> urmarireContur(Mat src, int LABEL)
{
	vector<Point> contur;
	int di[8] = { 0,-1,-1,-1,0,1,1,1 };
	int dj[8] = { 1,1,0,-1,-1,-1,0,1 };

	Mat src_color = Mat::zeros(src.size(), CV_8UC1);

	int height = src.rows;
	int width = src.cols;
	int dir = 7;

	Point p1;
	int i_aux, j_aux;

	int forda = 1;
	for (int i = 0; i < height && forda; i++)
		for (int j = 0; j < width; j++)
		{
			if (src.at<int>(i, j) == LABEL) // gaseste primul pixel obiect
			{
				src_color.at<uchar>(i, j) = 255;
				src.at<int>(i, j) = LABEL;
				p1 = Point(j, i);

				// salveaza primul punct in lista puncte de output
				contur.push_back(p1);
				int k = 0;

				// algoritmul are o logica care va inactiva not_finished, va termina bucla while si va finaliza cautarea conturlui
				int not_finished = 1;

				while (not_finished)
				{
					if (dir % 2 == 0)
						dir = (dir + 7) % 8;
					else
						dir = (dir + 6) % 8;

					while (1)
					{
						if (isInside(src, i, j) && p1.x == j && p1.y == i && k != 0)
						{
							not_finished = 0;
							break;
						}

						if (isInside(src, i + di[dir], j + dj[dir]) && src.at<int>(i + di[dir], j + dj[dir]) == LABEL)
						{
							contur.push_back(Point(j + dj[dir], i + di[dir]));
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
				forda = 0; // optimizare; cand am terminat conturul ne oprim din a mai parcurge matricea
				break;
			}
		}

	/*imshow("contur gasit", src_color);
	waitKey();*/
	return contur;
}

void BGR2HSV(Mat src, Mat H, Mat S, Mat V) {
	int height = src.rows;
	int width = src.cols;

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
			//printf("%d\n", h);
			if (((h >= 0 && h < 15) || (h > 215 && h <= 255)) &&
				(s > 70) &&
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
			if ((h >= 140 && h <= 185) && ((s > 110) && (v > 100)))
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

Mat inchidere(Mat src, int dim) {
	bool ok = true;
	int height = src.rows;
	int width = src.cols;
	int mij = dim / 2;

	//initializarea matricilor destinatie
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

	// dilatare
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

	// eroziune
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

	return dst2;
}

Mat douaTreceri(Mat img, int* labelSize) // asta merge
{
	Mat dst = Mat::zeros(img.size(), CV_8UC3);
	Mat labels = Mat::zeros(img.size(), CV_32SC1);

	int label = 0;
	int height = img.rows;
	int width = img.cols;

	// Vectorii de directie 8-neighborhood
	int di[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int dj[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

	// Prima trecere: asigneaza initial etichetele si inregistreaza echivalentele
	vector<vector<int>> edges(5000);
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

	// A doua trecere: rezolva echivalenta utilizand BFS
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

	// Reasigneaza etichetele bazate pe noua etichetare
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			labels.at<int>(i, j) = newLabels[labels.at<int>(i, j)];
		}
	}

	*labelSize = newLabel;

	// Generarea alteatoare de culori
	vector<Vec3b> colors(newLabel + 1);
	default_random_engine eng;
	uniform_int_distribution<int> distrib(0, 255);
	for (int i = 0; i <= newLabel; i++) {
		colors[i] = Vec3b(distrib(eng), distrib(eng), distrib(eng));
	}

	// Coloreaza imaginea rezultat bazata pe etichetare
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

	return labels;
}

Mat deleteSmallObj(Mat labels, Mat src, int labelSize) {
	vector<int> aria(labelSize + 1, 0);
	int minSize = 1000;

	int label = 0;
	int height = src.rows;
	int width = src.cols;

	Mat dst = Mat(height, width, CV_8UC1);
	src.copyTo(dst);

	// ariile se stocheaza intr-un array in functie de eticheta lor
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (labels.at<int>(i, j) != 0) {
				aria[labels.at<int>(i, j)]++;
			}
		}
	}

	//se pastreaza doar pixelii a caror arie/label este mai mare de un anumit prag inpus (minSize)
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (aria[labels.at<int>(i, j)] < minSize) {
				dst.at<uchar>(i, j) = 0;
			}
		}
	}

	return dst;
}

void detectRectangle(Mat& resultImage, const vector<vector<Point>>& contours, boolean isRed, int index) {
	vector<Point> approx;
	double epsilon = 0.02 * arcLength(contours[index], true);
	// se face aproximarea conturului si ne rezulta numarul de varfuri in approx
	approxPolyDP(contours[index], approx, epsilon, true);

	// verificarea daca este patrat
	if (approx.size() == 4) {
		drawContours(resultImage, vector<vector<Point>>{approx}, -1, Scalar(0, 255, 0), 2);
		drawBoundingBox(resultImage, contours, isRed, index, 3);
	}
}

void detectOctogon(Mat& resultImage, const vector<vector<Point>>& contours, boolean isRed, int index) {
	vector<Point> approx;
	double epsilon = 0.02 * arcLength(contours[index], true);
	// se face aproximarea conturului si ne rezulta numarul de varfuri in approx
	approxPolyDP(contours[index], approx, epsilon, true);

	// verificarea daca este octogon
	if (approx.size() == 8) {
		if(isRed)
			drawContours(resultImage, vector<vector<Point>>{approx}, -1, Scalar(0, 255, 0), 2);
		drawBoundingBox(resultImage, contours, isRed, index, 1);
	}
	else {		//altfel, verificare daca este patrat
		detectRectangle(resultImage, contours, isRed, index);
	}
}

void detectTriangles(Mat& resultImage, const vector<vector<Point>>& contours, boolean isRed, int index) {
	vector<Point> approx;
	double epsilon = 0.02 * arcLength(contours[index], true);
	// se face aproximarea conturului si ne rezulta numarul de varfuri in approx
	approxPolyDP(contours[index], approx, epsilon, true);

	// verificarea daca este octogon
	if (approx.size() == 3) {
		drawContours(resultImage, vector<vector<Point>>{approx}, -1, Scalar(0, 255, 0), 2);
		drawBoundingBox(resultImage, contours, isRed, index, 2);
	}
	else {		//altfel, verificare daca este patrat
		detectRectangle(resultImage, contours, isRed, index);
	}
}

void findContoursLaMana(Mat src, vector<vector<Point>> &contours) {
	// FIND CONTOURS FACUT IN CASA
	// BASED ON LABELING AND URMARIRE CONTUR (which detects the first contour)
	int labelSize;
	Mat labels = douaTreceri(src.clone(), &labelSize);
	int currentLabel = 1;
	for (int i = 0; i < labels.rows; i++) {
		for (int j = 0; j < labels.cols; j++) {
			if (labels.at<int>(i, j) == currentLabel) {
				contours.push_back(urmarireContur(labels, currentLabel++));
			}
		}
	}
	/*findContours(src.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);*/
}

void detectShapes(Mat src, Mat output, boolean isRed) {

	vector<vector<Point>> contours;
	findContoursLaMana(src.clone(), contours); int x;
	/*Mat t = douaTreceri(src.clone(), &x);
	imshow("t", t);*/

	for (int i = 0; i < contours.size(); ++i) {
		if (contours[i].size() < 5) continue; // fitEllipse needs at least 5 points
		//incadram obiectul nostru intr-o elipsa
		RotatedRect ell = fitEllipse(contours[i]);

		Mat1b maskContour(src.rows, src.cols, uchar(0));
		//desenam conturul real al obiectului intr-o masca
		drawContours(maskContour, contours, i, Scalar(255), 2);

		Mat1b maskEllipse(src.rows, src.cols, uchar(0));
		//desenam conturul elipsei in care se incadreaza obiectul
		ellipse(maskEllipse, ell, Scalar(255), 2);

		Mat1b intersection = maskContour & maskEllipse;

		// Count amount of intersection
		float cnz = countNonZero(intersection);
		// Count number of pixels in the drawn contour
		float n = countNonZero(maskContour);
		// Compute your measure
		float measure = cnz / n;

		// cu cat measure e mai aproape de 1, cu atat e mai rotunda forma
		// cu cat e mai aproape de 0, forma se apropie de un poligon cu numar redus de laturi 
		if (measure >= 0.6) {
			ellipse(output, ell, Scalar(0, measure * 255, 255 - measure * 255), 3);
			drawBoundingBox(output, contours, isRed, i, 0);
		}
		else if (measure >= 0.4) {
			detectOctogon(output, contours, isRed, i);
		}
		else {
			detectTriangles(output, contours, isRed, i);
		}
	}

}
