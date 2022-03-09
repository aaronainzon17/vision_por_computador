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
    Mat img = imread("poster.pgm");
    // Display original image
    imshow("original Image", img);
    waitKey(0);

    // Convert to graycsale
    Mat img_gray;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    // Blur the image for better edge detection
    Mat img_blur;
    GaussianBlur(img_gray, img_blur, Size(3,3), 0);
    Mat sobelx, sobely,sobelxMostrar,sobelyMostrar;
    // Sobel(img_blur, sobelx, CV_64F, 1, 0, 5);
    // Sobel(img_blur, sobely, CV_64F, 0, 1, 5);

    // Sobel(img, sobelx, CV_8U, 1, 0, 3, 1, 0, BORDER_DEFAULT);
    // Sobel(img, sobely, CV_8U, 0, 1, 3, 1, 0, BORDER_DEFAULT);
    //Esta solucion de arriba a secas no vale porque 
    /*
In our last example, output datatype is cv.CV_8U. But there is a slight problem with that. Black-to-White transition is taken as Positive slope (it has a positive value) while White-to-Black transition is taken as a Negative slope (It has negative value). So when you convert data to cv.CV_8U, all negative slopes are made zero. In simple words, you miss that edge.

If you want to detect both edges, better option is to keep the output datatype to some higher forms, like cv.CV_16S, cv.CV_64F etc, take its absolute value and then convert back to cv.CV_8U. Below code demonstrates this procedure for a horizontal Sobel filter and difference in results.
    */

//Explicación del uso de sobel
/*
8UC1 va de 0,255
64F va de -255,255
La cosa de hacer sobel es que hay que escalar para que no se te vayan los valores fuera de -255,255
Luego para mostrar solo puedes mostrar de 0,255 por tanto hay que pasarla a 8UC1 (primero /2 +128este convertTo es solo para mostrar)

Para implementar sobel hay que escalarlo porque sino se te va de valores (o algo así ha dicho rosario)
*/

    //Para implementar sobel solo hay que aplicar la mascara y escalar
    //Sobel(img, sobelx, CV_64F, 1, 0, 3, 1, 0, BORDER_DEFAULT);
    //Sobel(img, sobely, CV_64F, 0, 1, 3, 1, 0, BORDER_DEFAULT);
    
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

    

    //Ahora toca el módulo, simplemente se saca el módulo para la componente x e y y que se obtiene de las matrices obtenida anteriormente
    Mat modulo;
    modulo.create(img.rows,img.cols,CV_64F);
    cvtColor(modulo, modulo, COLOR_BGR2GRAY);
    modulo = Scalar::all(0);
    for (int x = 0; x < img.cols; x++) {
        for (int y = 0; y < img.rows; y++) {
            modulo.at<Vec3b>(y,x) = sqrt(pow(sobelx.at<Vec3b>(y,x),2) + pow(sobely.at<Vec3b>(y,x),2));
        }
    }
    imshow("modulo", modulo);
    waitKey(0);
    
    destroyAllWindows();
 
    return 0;
}