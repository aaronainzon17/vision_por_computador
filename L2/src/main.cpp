#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include "iostream"
#include <math.h>

using namespace std;
using namespace cv;



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
    Mat sobelx, sobely, absDsty,absDstx;
    // Sobel(img_blur, sobelx, CV_64F, 1, 0, 5);
    // Sobel(img_blur, sobely, CV_64F, 0, 1, 5);

    // Sobel(img, sobelx, CV_8U, 1, 0, 3, 1, 0, BORDER_DEFAULT);
    // Sobel(img, sobely, CV_8U, 0, 1, 3, 1, 0, BORDER_DEFAULT);
    //Esta solucion de arriba a secas no vale porque 
    /*
In our last example, output datatype is cv.CV_8U. But there is a slight problem with that. Black-to-White transition is taken as Positive slope (it has a positive value) while White-to-Black transition is taken as a Negative slope (It has negative value). So when you convert data to cv.CV_8U, all negative slopes are made zero. In simple words, you miss that edge.

If you want to detect both edges, better option is to keep the output datatype to some higher forms, like cv.CV_16S, cv.CV_64F etc, take its absolute value and then convert back to cv.CV_8U. Below code demonstrates this procedure for a horizontal Sobel filter and difference in results.
    */

    Sobel(img, sobelx, CV_8U, 1, 0, 3, 1, 0, BORDER_DEFAULT);
    Sobel(img, absDstx, CV_64F, 1, 0, 3, 1, 0, BORDER_DEFAULT);
    Sobel(img, sobely, CV_8U, 0, 1, 3, 1, 0, BORDER_DEFAULT);
    Sobel(img, absDsty, CV_64F, 0, 1, 3, 1, 0, BORDER_DEFAULT);
    convertScaleAbs(absDstx, absDstx, 1, 0);
    convertScaleAbs(absDsty, absDsty, 1, 0);
    
    //Sacar algo de cartToPolar 


    imshow("Abs x", absDstx);
    waitKey(0);
    // imshow("Sobel y", sobelx);
    // waitKey(0);
    imshow("Abs y", absDsty);
    waitKey(0);
    // imshow("Sobel XY using Sobel() function", sobelxy);
    // waitKey(0);


    // // Canny edge detection
    // Mat edges;
    // Canny(img_blur, edges, 100, 200, 3, false);
    // // Display canny edge detected image
    // imshow("Canny edge detection", edges);
    // waitKey(0);

    //Ahora toca el módulo, simplemente se saca el módulo para la componente x e y y que se obtiene de las matrices obtenida anteriormente
    
    
    
    destroyAllWindows();
 
    return 0;
}