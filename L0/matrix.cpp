#include "opencv2/highgui/highgui.hpp"
using namespace cv;

void salt(cv::Mat &image, int n) {
    // Adds salt and pepper noise to n pixels randomly selected in the image
    for (int k=0; k<n; k++) {
        int i= rand() % image.rows;
        int j= rand() % image.cols;
        if (image.channels() == 1) { // b/w image
            image.at<uchar>(i,j)= 255;
        } else if (image.channels() == 3) { // color image
            image.at<cv::Vec3b>(i,j)[0]= 255;
            image.at<cv::Vec3b>(i,j)[1]= 255;
            image.at<cv::Vec3b>(i,j)[2]= 255;
        }
    }
}

void colorReducePointers(cv::Mat &img, int div=64) {
    for (int i=0; i<img.rows; i++) {
        uchar* data= img.ptr<uchar>(i); // pointer to row i
        for (int j=0; j<img.cols*img.channels(); j++) {
            data[j]= data[j]/div*div + div/2;
            // if you like, you can do it this way:
            // *data++= *data/div*div + div/2;
        }
    }
}

void colorReduceIterators(cv::Mat &image, int div=64) {
    Mat_<Vec3b>::iterator it = image.begin<Vec3b>();
    // iterator created at the begining of the image
    Mat_<Vec3b>::iterator itend = imagen.end<Vec3b>();
    // iterador created at the end of the image
    for ( ; it!= itend; ++it) { // loop over all pixels
        (*it)[0]= (*it)[0]/div*div + div/2;
        (*it)[1]= (*it)[1]/div*div + div/2;
        (*it)[2]= (*it)[2]/div*div + div/2;
    }
}

int main( int argc, char** argv ) {
    Mat img = imread( argv[1], -1 );
    if( img.empty() ) return -1;
    namedWindow( "Example2", WINDOW_AUTOSIZE );
    imshow( "Example2", img );
    waitKey( 0 );
    destroyWindow( "Example2" );
}