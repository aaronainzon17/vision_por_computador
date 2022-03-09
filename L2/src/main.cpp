#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include "iostream"
#include <math.h>

using namespace std;
using namespace cv;

/*Implementacion de la matriz del kernel a mano y tras eso utilizar el la func filter2D para aplicar el gradiente*/

int main(int, char**) {

    // Reading image
    Mat img = imread("../../Contornos/pasillo1.pgm");
    // Display original image
    imshow("original Image", img);
    waitKey(0);

    // Convert to graycsale
    Mat img_gray;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    // Blur the image for better edge detection
    Mat img_blur;
    GaussianBlur(img_gray, img_blur, Size(3,3), 0);

//Explicación del uso de sobel
/*
8UC1 va de 0,255
64F va de -255,255
La cosa de hacer sobel es que hay que escalar para que no se te vayan los valores fuera de -255,255
Luego para mostrar solo puedes mostrar de 0,255 por tanto hay que pasarla a 8UC1 (primero /2 +128este convertTo es solo para mostrar)

Para implementar sobel hay que escalarlo porque sino se te va de valores (o algo así ha dicho rosario)
*/
    
    Mat horizontalK = (Mat_<float>(3,3)<<1.,2.,1.,0.,0.,0.,-1.,-2.,-1.); // horizontal kernel
    Mat verticalK = (Mat_<float>(3,3)<<-1.,0.,1.,-2.,0.,2.,-1.,0.,1.); // vertical kernel
    
    Mat sobelX,sobelY,sobelXaux,sobelYaux;

    filter2D(img_blur,sobelX,CV_64F,verticalK);
    filter2D(img_blur,sobelY,CV_64F,horizontalK);

    //Esto es solo para mostrarlo, los calculos de los gradientes se hacen con los valores originales
    sobelXaux = sobelX/2 + 128;
    sobelXaux.convertTo(sobelXaux,CV_8UC1);
    imshow("Gradiente en X", sobelXaux);

    waitKey(0);

    sobelYaux = sobelY/2 + 128;
    sobelYaux.convertTo(sobelYaux,CV_8UC1);
    imshow("Gradiente en Y", sobelYaux);
    
    waitKey(0);

    Mat magnitude,angle;

    cartToPolar(sobelX,sobelY, magnitude, angle);
    magnitude.convertTo(magnitude,CV_8UC1);
    imshow("modulo", magnitude);
    waitKey(0);
    angle = angle/3.14159 *128;
    angle.convertTo(angle,CV_8UC1);
    imshow("orientacion", angle);
    waitKey(0);
    
    destroyAllWindows();
    float th;
    int x,y;
    //Apartado 2
    for (int i = 0; i < img_gray.rows; i++){
        for(int j = 0; j < img_gray.clumns; j++){
            if (magnitude.at<float>(j,i) >= threshold){
                x = j - img_gray.clumns/2;
                y = img_gray.rows/2 - i;
                th = angle.at<float>(i,j);
                p = x * cos(th) + y * sin(th);
                //Vote Line 
            }
        }
    }
    
    return 0;
}

/*
votan dos puntos que podrian pertenecer al gradiente (que podrian pertener a un gradiente votan)
1 opcion: votan puntos a lineas y lineas a puntos de fuga
Para cada fila
    para cada columna
        tenemos x,y, su modulo, orientacion
        elegir el threshold: pues el maximo partido por 10, o la media
        si tenemos la x,y y th pues scas la p 
*/