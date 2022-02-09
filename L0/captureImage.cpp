#include "opencv2/opencv.hpp"
#include "iostream"

using namespace std;
using namespace cv;

int main(int, char**) {
    // open the first webcam plugged in the computer
    VideoCapture camera(0);
    if (!camera.isOpened()) {
        cerr << "ERROR: Could not open camera" << endl;
        return 1;
    }

    // create a window to display the images from the webcam
    namedWindow("Webcam", WINDOW_AUTOSIZE);

    // this will contain the image from the webcam
    Mat frame;
        
    // capture the next frame from the webcam
    camera >> frame;
    
    // display the frame until you press a key
    while (1) {
        // show the image on the window
        imshow("Webcam", frame);
        // wait (10ms) for a key to be pressed
        if (waitKey(10) >= 0)
            break;
    }
    return 0;
}