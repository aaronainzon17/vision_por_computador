#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include "iostream"
#include <math.h>

using namespace std;
using namespace cv;

/*Implementacion de la matriz del kernel a mano y tras eso utilizar el la func filter2D para aplicar el gradiente*/

Mat hough(Mat img, Mat angle, Mat magnitude);

int main(int, char**) {

    // Reading image
    Mat img = imread("../../Contornos/pasillo2.pgm");
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
    
    Mat kernelY = (Mat_<float>(3,3)<<1.,2.,1.,0.,0.,0.,-1.,-2.,-1.); //kernel para el eje y
    Mat kernelX = (Mat_<float>(3,3)<<-1.,0.,1.,-2.,0.,2.,-1.,0.,1.); //kernel para el eje X
    
    Mat sobelX,sobelY,sobelXaux,sobelYaux;

    filter2D(img_blur,sobelX,CV_64F,kernelX);
    filter2D(img_blur,sobelY,CV_64F,kernelY);

    //Esto es solo para mostrarlo, los calculos de los gradientes se hacen con los valores originales
    sobelXaux = sobelX/2 + 128;
    sobelXaux.convertTo(sobelXaux,CV_8UC1);
    imshow("Gradiente en X", sobelXaux);

    waitKey(0);

    sobelYaux = sobelY/2 + 128;
    sobelYaux.convertTo(sobelYaux,CV_8UC1);
    imshow("Gradiente en Y", sobelYaux);
    
    waitKey(0);
    
    Mat magnitude, magnitude_aux,angle, angle_aux;

    cartToPolar(sobelX,sobelY, magnitude, angle);
    //magnitude_aux = magnitude / 255;
    magnitude.convertTo(magnitude, CV_8UC1);
    imshow("modulo", magnitude);
    cout << magnitude << endl;
    waitKey(0);

    angle_aux = ((angle/CV_PI) * 128);
    angle_aux.convertTo(angle_aux, CV_8UC1);
    imshow("orientacion", angle_aux);
    waitKey(0);
    
    //destroyAllWindows();
    float th;
    int x,y;
    //Apartado 2
    //cout << angle << endl;
	Mat pto_fuga = hough(img_blur, angle, magnitude);

    imshow("Punto de fuga", pto_fuga);
	waitKey(0);			// Se pausa para ver los resultados.
    
    destroyAllWindows();
    
    return 0;
}

Mat hough(Mat img, Mat angle, Mat magnitude){
    float threshold = 1000;
    cout << "La imagen tiene " << img.rows << " filas" << endl;
    int centro[img.cols];
    //Se inicializa el vector a 0
    for (int i = 0; i < img.cols; i++){
        centro[i] = 0;
    }
    //Se itera sobre la imagen
    int entran = 0;
    for (int i = 0; i < img.rows; i++){
        for(int j = 0; j < img.cols; j++){
            if (magnitude.at<float>(i,j) >= threshold){
                cout << "La celda " << i << "," << j << " tiene el valor " << magnitude.at<float>(i,j) << endl;
                float x = j - img.cols/2;
                float y = img.rows/2 - i;
                float th = angle.at<float>(i,j);
                float p = x * cos(th) + y * sin(th); // rho = distancia al punto de origen 

                float distance_90 = th - (CV_PI/2);
                float distance_270 = th - ((3*CV_PI)/2); 
                // float distance_90 = abs(th - (CV_PI/2));
                // float distance_270 = abs(th - ((3*CV_PI)/2));
                // float distance_180 = abs(th - (CV_PI));
                // float distance = abs(th);
                //if (dist > 0.07){
                //if ((distance_90 > 0.088) && (distance_270 > 0.088)){
                if ((distance_90 > 0.088) && (distance_270 > 0.088)){   //&& (distance_180 > 0.088) && (distance > 0.088)
                    entran++;
                    //Vote Line 
                    int x_fuga = (p - y*sin(th)) / cos(th);		// Se calcula la x sabiendo que y = 0 y conociendo rho(p)
                    //int x_fuga = (p) / cos(th);

                    if (x_fuga < img.cols/2 && x_fuga >= -img.cols/2) {	// Se comprueba que corta en la imagen.
                        x_fuga += img.cols/2;		// Se pone el corte en el rango.
                        centro[x_fuga]++;	// Se actualiza el valor.
                    }
                    
                }
                
            }
        }
    }
    cout << "Han entrado al bucle " << entran << endl;
    int max_votos = 0;
    //Se saca el punto mas votado
	for(int i = 0; i < img.cols; i++){
		if(centro[i] > centro[max_votos]){
			cout << "La celda " << i << " tiene " << centro[i] << " votos" << endl;
            max_votos = i;
		}
	}
    cout << "El punto de fuga se ha encontrado en las coordenadas:" << endl;
    cout << max_votos << " " << img.rows/2 << endl;
    circle(img, Point(max_votos,img.rows/2), 3, CV_RGB(255,0,0), 1);
	
    return img;		// Se devuelve la matriz con el punto de fuga
}

void findMax(Mat magnitude) {
    float mid = magnitude.rows / 2;
    cout << "La fila de la mitad es: " << mid << endl;

    float max = 0;
    for (int i = 0; i < magnitude.rows; i++){
        for(int j = 0; j < magnitude.cols; j++){
            if (magnitude.at<float>(i,j) >= max){
                max = magnitude.at<float>(i,j);
            }
        }
    }
    cout << "El maximo valor de la imagen es: " << max << endl;
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