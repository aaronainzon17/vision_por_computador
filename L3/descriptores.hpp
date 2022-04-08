#ifndef DESCRIPTORES_HPP
#define DESCRIPTORES_HPP

//RNG rng(12345);

#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include "iostream"
#include <math.h>

using namespace std;
using namespace cv;
// Espacios de nombres.
using namespace cv;
using namespace std;

Mat sacarOtsu(Mat img);

Mat sacarBlobs(Mat outsu);

void sacarDescriptores(vector<vector<Point>> contours, Mat figure_bin, RNG rng, vector<Moments>& mu, vector<Point2f>& mc, double areasContornos[],double diametroContornos[]);


#endif /* IMAGEN_HPP_ */