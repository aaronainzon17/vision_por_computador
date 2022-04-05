#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include "iostream"
#include <math.h>

using namespace std;
using namespace cv;

/*Implementacion de la matriz del kernel a mano y tras eso utilizar el la func filter2D para aplicar el gradiente*/

Mat hough(Mat img, Mat angle, Mat magnitude);
double normalizar_pi_pi(double th);
void findMax(Mat magnitude);

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
    
    //Se definen los kernel para poder aplicar la mascara y realizar la segunda derivada con Sobel
    Mat kernelY = (Mat_<double>(3,3)<<1.,2.,1.,0.,0.,0.,-1.,-2.,-1.); //kernel para el eje y
    Mat kernelX = (Mat_<double>(3,3)<<-1.,0.,1.,-2.,0.,2.,-1.,0.,1.); //kernel para el eje X
    
    Mat sobelX,sobelY,sobelXaux,sobelYaux;
    
    //Se aplica la mascara
    filter2D(img_blur,sobelX,CV_64F,kernelX);
    filter2D(img_blur,sobelY,CV_64F,kernelY);
    
    //Normalizar el sobelX y sobelY dividiendo entre 4 (la suma de las componentes positivas del kernel)
    sobelX = sobelX*0.25;
    sobelY = sobelY*0.25;

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
    //Con los gradientes de x e y se obtiene el modulo y la orientacion
    cartToPolar(sobelX,sobelY, magnitude, angle);
    magnitude.convertTo(magnitude_aux, CV_8UC1);
    imshow("modulo", magnitude_aux);
    waitKey(0);
    //Esto se realiza solo para mostrar la imagen
    angle_aux = ((angle/CV_PI) * 128);
    angle_aux.convertTo(angle_aux, CV_8UC1);
    imshow("orientacion", angle_aux);
    waitKey(0);
    
    //Apartado 2
	Mat pto_fuga = hough(img_blur, angle, magnitude);
    imshow("Punto de fuga", pto_fuga);
	waitKey(0);			// Se pausa para ver los resultados.
    
    destroyAllWindows();
    
    return 0;
}

Mat hough(Mat img, Mat angle, Mat magnitude){
    double threshold = 30;
    int centro[img.cols];
    //Se inicializa el vector a 0 (el vector sirve para elegir cual es el punto de fuga,
    //cada posicion sera un punto de la linea central de laimagen)
    for (int i = 0; i < img.cols; i++){
        centro[i] = 0;
    }
    //Se itera sobre la imagen
    for (int i = 0; i < img.rows; i++){
        for(int j = 0; j < img.cols; j++){
            //Se ignorar los pixeles con un modulo bajo ya que no tienen importancia
            if (magnitude.at<double>(i,j) >= threshold){
                double x = j - img.cols/2;
                double y = img.rows/2 - i;
                double th = angle.at<double>(i,j);
                double p = x * cos(th) + y * sin(th); // rho = distancia al punto de origen 
                //Se calcula la linea a la que pertenece el pixel
                double d_90 = abs(normalizar_pi_pi(th - (CV_PI/2)));
                double d_270 = abs(normalizar_pi_pi(th - ((3*CV_PI)/2))); 
                double d_180 = abs(normalizar_pi_pi(th - (CV_PI)));
                double d_360 = abs(normalizar_pi_pi(th - (2*CV_PI)));
                double d = abs(th);
                double max_angle = 5*CV_PI/180;
                //Se eliminan las lines que son verticales, horizontales o con un angulo muy pequeño respecto a la linea central
                if ((d_90 > max_angle) && (d_270 > max_angle) && (d > max_angle) && (d_180 > max_angle) && (d_360 > max_angle)){
                    //Vote Line 
                    int x_fuga = (p) / cos(th); // Se calcula la x sabiendo que y = 0 y conociendo rho(p)
                    if ((x_fuga < img.cols/2) && (x_fuga >= -img.cols/2)) {	// Se comprueba que corta en la imagen.
                        x_fuga += img.cols/2;		// Se pone el corte en el rango.
                        centro[x_fuga]++;	// Se actualiza el valor.
                    }   
                }
            }
        }
    }

    int max_votos = 0;
    //Se saca el punto mas votado
	for(int i = 0; i < img.cols; i++){
		if(centro[i] > centro[max_votos]){
            max_votos = i;
		}
	}
    cout << "El punto de fuga se ha encontrado en las coordenadas:" << endl;
    cout << max_votos << " " << img.rows/2 << endl;
    circle(img, Point(max_votos,img.rows/2), 3, CV_RGB(255,0,0), 1);
	
    return img;		// Se devuelve la matriz con el punto de fuga
}

void findMax(Mat magnitude) {
    double mid = magnitude.rows / 2;
    cout << "La fila de la mitad es: " << mid << endl;

    double max = 0;
    for (int i = 0; i < magnitude.rows; i++){
        for(int j = 0; j < magnitude.cols; j++){
            if (magnitude.at<double>(i,j) >= max){
                max = magnitude.at<double>(i,j);
            }
        }
    }
    cout << "El maximo valor de la imagen es: " << max << endl;
}
double normalizar_pi_pi(double th){
    while (th > CV_PI){
            th = th - 2 * CV_PI;
    }
    while (th < -CV_PI){
        th = th + 2 * CV_PI;
    }
    return th;
}
        