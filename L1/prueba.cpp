// #include "opencv2/imgcodecs.hpp"
// #include "opencv2/highgui.hpp"
// #include <iostream>
// using namespace cv;
// using std::cout;
// const int alpha_slider_max = 100;
// int alpha_slider;
// double alpha;
// double beta;
// Mat src1;
// Mat src2;
// Mat dst;
// void equalize(Mat img, float a, float b){
//     Mat img_aux = img.clone();
//     img = a*img + b;
//     cout << "Matriz ecualizada" << endl;
//     cout << img << endl;
//     // Create a window
//     namedWindow("ecualizacion_a_mano", 1);
//     imshow("ecualizacion_a_mano", img);
//     waitKey(0);
// }

// static void on_trackbar( int, void*, Mat)
// {
//    alpha = (double) alpha_slider/alpha_slider_max ;
//    beta = ( 1.0 - alpha );
//    addWeighted( src1, alpha, src2, beta, 0.0, dst);
//    imshow( "img", dst );
// }
// static void on_trackbar2( int, void*, Mat )
// {
//    alpha = (double) alpha_slider/alpha_slider_max ;
//    beta = ( 1.0 - alpha );
//    addWeighted( src1, alpha, src2, beta, 0.0, dst);
//    imshow( "img", dst );
// }
// int main( void )
// {
//    src1 = imread( samples::findFile("monkey.jpg") );
//    src2 = imread( samples::findFile("monkey.jpg") );
//    if( src1.empty() ) { cout << "Error loading src1 \n"; return -1; }
//    if( src2.empty() ) { cout << "Error loading src2 \n"; return -1; }
//    alpha_slider = 0;
//    namedWindow("img", WINDOW_AUTOSIZE); // Create Window
//    createTrackbar( "Brillo", "img", &alpha_slider, alpha_slider_max, on_trackbar );
//    createTrackbar( "Constraste", "img", &alpha_slider, alpha_slider_max, on_trackbar2 );
//    on_trackbar( alpha_slider, 0, img );
//    on_trackbar2( alpha_slider, 0, img );
//    waitKey(0);
//    return 0;
// }