#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <ctime> 
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
    Mat_<Vec3b>::iterator itend = image.end<Vec3b>();
    // iterador created at the end of the image
    for ( ; it!= itend; ++it) { // loop over all pixels
        (*it)[0]= (*it)[0]/div*div + div/2;
        (*it)[1]= (*it)[1]/div*div + div/2;
        (*it)[2]= (*it)[2]/div*div + div/2;
    }
}

int main( int argc, char** argv ) {
    Mat img = imread( argv[1], -1 );
    //En img se guarda la matriz de los pixeles
    if( img.empty() ) return -1;
    //Aqui se muestra la imagen
    namedWindow( "Example2", WINDOW_AUTOSIZE );
    imshow( "Example2", img );
    //Se espera a que terminemos el programa
    //Meter aqui lo de transformar matrices y comparar tiempos
    Mat img1 = img.clone(),img2=img.clone(),img3=img.clone();
    //Traverse with pointers
    auto t = std::chrono::high_resolution_clock::now();
    colorReducePointers(img1);
    auto d = std::chrono::high_resolution_clock::now()-t; 
    std::cout <<"Tiempo de punteros: " << std::chrono::duration_cast<std::chrono::milliseconds>(d).count() << "ms"<<std::endl;
    //Traverse with iterators
    t = std::chrono::high_resolution_clock::now();
    colorReduceIterators(img2);
    d = std::chrono::high_resolution_clock::now()-t; 
    std::cout <<"Tiempo de iterador: " << std::chrono::duration_cast<std::chrono::milliseconds>(d).count() << "ms"<<std::endl;
    //Traverse with index
    t = std::chrono::high_resolution_clock::now();
    salt(img3,img.rows*img.cols);
    d = std::chrono::high_resolution_clock::now()-t; 
    std::cout <<"Tiempo de indexer: " << std::chrono::duration_cast<std::chrono::milliseconds>(d).count() << "ms"<<std::endl;

    namedWindow( "Punteros", WINDOW_AUTOSIZE );
    imshow( "Punteros", img1 );
    namedWindow( "Iterator", WINDOW_AUTOSIZE );
    imshow( "Iterator", img2 );
    namedWindow( "Indexer", WINDOW_AUTOSIZE );
    imshow( "Indexer", img3 );
    waitKey( 0 );
    destroyWindow( "Example2" );
    destroyWindow( "Punteros" );
    destroyWindow( "Iterator" );
    destroyWindow( "Indexer" );

    //Analisis de tiempo de tranverse matrix

}
