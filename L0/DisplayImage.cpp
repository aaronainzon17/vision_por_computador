#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
// int main(int argc, char** argv )
// {
//     if ( argc != 2 )
//     {
//         printf("usage: DisplayImage.out <Image_Path>\n");
//         return -1;
//     }
//     Mat image;
//     image = imread( argv[1], 1 );
//     if ( !image.data )
//     {
//         printf("No image data \n");
//         return -1;
//     }
//     namedWindow("Display Image", WINDOW_AUTOSIZE );
//     imshow("Display Image", image);
//     waitKey(0);
//     return 0;
// }

int main(int, char**) {
    // open the first webcam plugged in the computer
    cv::VideoCapture camera(CAP_V4L2);
    if (!camera.isOpened()) {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return 1;
    }

    // create a window to display the images from the webcam
    cv::namedWindow("Webcam", WINDOW_AUTOSIZE);

    // this will contain the image from the webcam
    cv::Mat frame;
        
    // capture the next frame from the webcam
    camera >> frame;
    
    // display the frame until you press a key
    while (1) {
        // show the image on the window
        cv::imshow("Webcam", frame);
        // wait (10ms) for a key to be pressed
        if (cv::waitKey(10) >= 0)
            break;
    }
    return 0;
}