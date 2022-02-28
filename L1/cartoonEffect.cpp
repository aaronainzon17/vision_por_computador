
#include<opencv2/imgproc.hpp>
#include<opencv2/photo.hpp>
#include<opencv2/highgui.hpp>
#include<iostream>

using namespace cv;
using namespace std;
//https://www.analyticsvidhya.com/blog/2021/07/know-how-to-give-cartoon-effects-to-your-photos-with-opencv/
Mat frame, cartoon;
int sigmaX;

void cartoonize(){
    //Convert to gray scale
    Mat grayImage;
    cvtColor(frame, grayImage, COLOR_BGR2GRAY);

    //apply gaussian blur
    GaussianBlur(grayImage, grayImage, Size(3, 3), (double)sigmaX/100);

    //find edges
    Mat edgeImage;
    Laplacian(grayImage, edgeImage, -1, 5);
    convertScaleAbs(edgeImage, edgeImage);
    
    //invert the image
    edgeImage = 255 - edgeImage;

    //apply thresholding
    threshold(edgeImage, edgeImage, 150, 255, THRESH_BINARY);

    //blur images heavily using edgePreservingFilter
    Mat edgePreservingImage;
    edgePreservingFilter(frame, edgePreservingImage, 2, 50, 0.4);
    
    cartoon = Scalar::all(0);

    // Combine the cartoon and edges
    cv::bitwise_and(edgePreservingImage, edgePreservingImage, cartoon, edgeImage);
}

int main(){

    //Read input image
    VideoCapture camera(0);
    if (!camera.isOpened()) {
        cerr << "ERROR: Could not open camera" << endl;
        return 1;
    }

    cout << "Aplicando efecto de CARTOON" << endl;
    namedWindow("Cartoon", WINDOW_AUTOSIZE); // Create Window
    createTrackbar( "Blur Effect", "Cartoon", &sigmaX, 100);

    while(1) {
        // capture the next frame from the webcam
        camera >> frame;

        //show the current image
        imshow("Original Image",frame);

        cartoonize();

        imshow("Cartoon",cartoon);
        if (waitKey(10) >= 0){
            break;
        }
    }
    
    destroyAllWindows();

    return 0;
}
