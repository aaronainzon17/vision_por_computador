#ifndef RECONOCER_HPP
#define RECONOCER_HPP

#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "descriptores.hpp"

#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;
using namespace cv;


enum Objeto {TRIANGULO, RUEDA, CIRCULO, VAGON, RECTANGULO, };

static RNG rng(12345);
const double areaMin = 1000;    //Indicar en la memoria que este valor se usa para ignorar los blobs basura y que se ha sacado 
                                //Viendo las areas de los descriptores

void reconocer(string nomfich);

void leerDatosAprendizaje(string nomfich,vector<vector<float>> &datosAprendidos);


#endif /* RECONOCER_HPP_ */