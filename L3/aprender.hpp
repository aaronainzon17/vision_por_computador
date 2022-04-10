#ifndef APRENDER_HPP
#define APRENDER_HPP

#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "descriptores.hpp"
#include <iostream>
#include <fstream>
#include <math.h>
#include "reconocer.hpp"

using namespace std;
using namespace cv;

void aprender(string nomfich, Moments& mu_x, Point2f& mc_x, double& area_x, double& diametro_x);

void aprenderTodo();

#endif