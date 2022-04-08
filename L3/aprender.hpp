#ifndef APRENDER_HPP
#define APRENDER_HPP

#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "descriptores.hpp"

#include "iostream"
#include <math.h>

using namespace std;
using namespace cv;


enum Objeto {TRIANGULO, RUEDA, CIRCULO, VAGON, RECTANGULO, };

const double areaMin = 1000;    //Indicar en la memoria que este valor se usa para ignorar los blobs basura y que se ha sacado 
                                //Viendo las areas de los descriptores

void aprender(string nomfich, Moments& mu_x, Point2f& mc_x, double& area_x, double& diametro_x);

#endif /* IMAGEN_HPP_ */