#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include "iostream"
#include <math.h>
#include "descriptores.hpp"
#include "aprender.hpp"
#include "reconocer.hpp"

using namespace std;
using namespace cv;

/*Implementacion de la matriz del kernel a mano y tras eso utilizar el la func filter2D para aplicar el gradiente*/

int thresh = 100;
const int max_thresh = 255;

//RNG rng(12345);

int main(int, char**) {
   aprenderTodo();
   reconocer("../imagenesL3/reco1.pgm");
   reconocer("../imagenesL3/reco2.pgm");
   reconocer("../imagenesL3/reco3.pgm");
   return 0;
}

