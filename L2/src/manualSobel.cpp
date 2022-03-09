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
    
    Mat horizontalK = (Mat_<float>(3,3)<<1.,2.,1.,0.,0.,0.,-1.,-2.,-1.); // horizontal kernel
    Mat verticalK = (Mat_<float>(3,3)<<-1.,0.,1.,-2.,0.,2.,-1.,0.,1.); // vertical kernel
    //Mat kern3 = (Mat_<float>(3,3)<<0.,1.,2.,-1.,0.,1.,-2.,-1.,0.); // left diagonal
    //Mat kern4 = (Mat_<float>(3,3)<<-2.,-1.,0.,-1.,0.,1.,0.,1.,2.); // right diagonal

    Mat sobelX,sobelY,sobelXaux,sobelYaux;

    filter2D(img_blur,sobelX,CV_64F,verticalK);
    filter2D(img_blur,sobelY,CV_64F,horizontalK);
    //filter2D(res,res,res.type(),kern3);
    //filter2D(res,res,res.type(),kern4);
    
    sobelXaux = sobelX/2 + 128;
    sobelXaux.convertTo(sobelXaux,CV_8UC1);
    imshow("Gradiente en X", sobelXaux);

    waitKey(0);

    sobelYaux = sobelY/2 + 128;
    sobelYaux.convertTo(sobelYaux,CV_8UC1);
    imshow("Gradiente en Y", sobelYaux);
    

    waitKey(0);
    destroyAllWindows();
 
    return 0;
}