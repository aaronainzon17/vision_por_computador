#include "opencv2/opencv.hpp"
#include "iostream"

using namespace std;
using namespace cv;

Mat frame, mask;
int slider_G, slider_R, slider_B;
VideoCapture camera(0);

Mat getSkin(Mat input){
    //Valores para detectar la piel
    int Y_MIN = 0;
    int Y_MAX = 255;
    int Cr_MIN = 133;
    int Cr_MAX = 173;
    int Cb_MIN = 77;
    int Cb_MAX = 127;

    Mat skin;
    //first convert our RGB image to YCrCb
    cvtColor(input,skin,COLOR_BGR2YCrCb);

    //uncomment the following line to see the image in YCrCb Color Space
    //imshow("YCrCb Color Space",skin);

    //filter the image in YCrCb color space
    inRange(skin,Scalar(Y_MIN,Cr_MIN,Cb_MIN),Scalar(Y_MAX,Cr_MAX,Cb_MAX),skin);

    return skin;
}

void on_trackbar(int, void*) {
   add(frame, Scalar(slider_B,slider_G,slider_R), frame, mask);
}

int main(){
    //VideoCapture camera(0);
    if (!camera.isOpened()) {
        cerr << "ERROR: Could not open camera" << endl;
        return 1;
    }

    // create a window to display the images from the webcam
    //namedWindow("Webcam", WINDOW_AUTOSIZE);
    namedWindow("Alien", WINDOW_AUTOSIZE); // Create Window
    
    slider_B = 0;
    slider_G = 255;
    slider_R = 0;

    createTrackbar( "Blue", "Alien", &slider_B, 255, on_trackbar );
    createTrackbar( "Green", "Alien", &slider_G, 255, on_trackbar );
    createTrackbar( "Red", "Alien", &slider_R, 255, on_trackbar );
   
    //start an infinite loop where webcam feed is copied to cameraFeed matrix
    //all of our operations will be performed within this loop
    while(1) {
        // capture the next frame from the webcam
        camera >> frame;

        //show the current image
        imshow("Original Image",frame);

        mask = getSkin(frame);
        //frame.setTo(Scalar(0,255,0), mask);
        //add(frame, Scalar(0,255,0), frame, mask);
        on_trackbar(1,0);
        imshow("Alien",frame);
        waitKey(30);
    }
    return 0;
}