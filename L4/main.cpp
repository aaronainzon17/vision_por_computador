#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
//#include "opencv2/features2d.hpp"
//#include "opencv2/xfeatures2d.hpp"

#include "opencv2/xfeatures2d/nonfree.hpp"
//#ifdef HAVE_OPENCV_XFEATURES2D

using namespace cv;
using namespace cv::xfeatures2d;
using std::cout;
using std::endl;

void probarSurf(Mat src, Mat img_gray,Mat srcP2,Mat img_grayP2,Mat srcP3,Mat img_grayP3){
   //-- Step 1: Detect the keypoints using SURF Detector
   int minHessian = 400;
   Ptr<SURF> detector = SURF::create( minHessian );
   std::vector<KeyPoint> keypoints, keypointsP2, keypointsP3;
   detector->detect( img_gray, keypoints );
   detector->detect(img_grayP2,keypointsP2);
   detector->detect(img_grayP3,keypointsP3);
   //-- Draw keypoints
   // Mat img_keypoints;
   // drawKeypoints( src, keypoints, img_keypoints );
   // //-- Show detected (drawn) keypoints
   // int down_width = 1000;
	// int down_height = 800;
	// Mat imS;
	// resize(img_keypoints, imS, Size(down_width, down_height), INTER_LINEAR);
   // imshow("SURF Keypoints", imS );
   // waitKey(0);
}

void probarSift(Mat src, Mat img_gray,Mat srcP2,Mat img_grayP2,Mat srcP3,Mat img_grayP3){
   cv::Ptr<cv::xfeatures2d::SIFT> detector = cv::xfeatures2d::SIFT::create();
   std::vector<cv::KeyPoint> keypoints, keypointsP2, keypointsP3;
   detector->detect(img_gray, keypoints);
   detector->detect(img_grayP2,keypointsP2);
   detector->detect(img_grayP3,keypointsP3);
   //Mat img_keypoints;
   //drawKeypoints( src, keypoints, img_keypoints );
   //-- Show detected (drawn) keypoints
   // int down_width = 1000;
	// int down_height = 800;
	// Mat imS;
	// resize(img_keypoints, imS, Size(down_width, down_height), INTER_LINEAR);
   // imshow("Sift Keypoints", imS );
   // waitKey(0);

}

void probarHarris(Mat src, Mat img_gray,Mat srcP2,Mat img_grayP2,Mat srcP3,Mat img_grayP3){
   int thresh = 200;
   int max_thresh = 255;
   int blockSize = 2;
   int apertureSize = 3;
   double k = 0.04;
   Mat dst = Mat::zeros( src.size(), CV_32FC1 );
   cornerHarris( img_gray, dst, blockSize, apertureSize, k );
   Mat dst_norm, dst_norm_scaled;
   normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
   convertScaleAbs( dst_norm, dst_norm_scaled );
   for( int i = 0; i < dst_norm.rows ; i++ )
   {
      for( int j = 0; j < dst_norm.cols; j++ )
      {
         if( (int) dst_norm.at<float>(i,j) > thresh )
         {
               circle( src, Point(j,i), 5,  Scalar(0), 2, 8, 0 );
         }
      }
   }
   // Mat imS;
   // int down_width = 1000;
	// int down_height = 800;
	// resize(src, imS, Size(down_width, down_height), INTER_LINEAR);
   // imshow("Harris Keypoints", imS );
   // //imshow( "Harris Keypoints", dst_norm_scaled );
   // waitKey(0);
}

void probarOrb(Mat src, Mat img_gray,Mat srcP2,Mat img_grayP2,Mat srcP3,Mat img_grayP3){
   Ptr<ORB> detector = ORB::create( );
   std::vector<KeyPoint> keypoints, keypointsP2, keypointsP3;
   detector->detect( img_gray, keypoints );
   detector->detect(img_grayP2,keypointsP2);
   detector->detect(img_grayP3,keypointsP3);
   //-- Draw keypoints
   // Mat img_keypoints;
   // drawKeypoints( src, keypoints, img_keypoints );
   //-- Show detected (drawn) keypoints
   // int down_width = 1000;
	// int down_height = 800;
	// Mat imS;
	// resize(img_keypoints, imS, Size(down_width, down_height), INTER_LINEAR);
   // imshow("ORB Keypoints", imS );
   // waitKey(0);
}

void probarAkaze(Mat src, Mat img_gray,Mat srcP2,Mat img_grayP2,Mat srcP3,Mat img_grayP3){
   Ptr<AKAZE> detector = AKAZE::create( );
   std::vector<KeyPoint> keypoints, keypointsP2, keypointsP3;
   detector->detect( img_gray, keypoints );
   detector->detect(img_grayP2,keypointsP2);
   detector->detect(img_grayP3,keypointsP3);
   //-- Draw keypoints
   // Mat img_keypoints;
   // drawKeypoints( src, keypoints, img_keypoints );
   //-- Show detected (drawn) keypoints
   // int down_width = 1000;
	// int down_height = 800;
	// Mat imS;
	// resize(img_keypoints, imS, Size(down_width, down_height), INTER_LINEAR);
   // imshow("Akaze Keypoints", imS );
   // waitKey(0);
}

int main( int argc, char* argv[] )
{
   Mat srcP1 = imread("../BuildingScene/poster1.jpg");
   Mat srcP2 = imread("../BuildingScene/poster2.jpg");
   Mat srcP3 = imread("../BuildingScene/poster3.jpg");
   //Pasar a nivel de grises
   Mat img_grayP1, img_grayP2, img_grayP3;
   cvtColor(srcP1, img_grayP1, COLOR_BGR2GRAY);
   cvtColor(srcP2, img_grayP2, COLOR_BGR2GRAY);
   cvtColor(srcP3, img_grayP3, COLOR_BGR2GRAY);

   probarSurf(srcP1,img_grayP1,srcP2,img_grayP2,srcP3,img_grayP3);
   probarSift(srcP1,img_grayP1,srcP2,img_grayP2,srcP3,img_grayP3);
   probarHarris(srcP1,img_grayP1,srcP2,img_grayP2,srcP3,img_grayP3);
   probarOrb(srcP1,img_grayP1,srcP2,img_grayP2,srcP3,img_grayP3);
   probarAkaze(srcP1,img_grayP1,srcP2,img_grayP2,srcP3,img_grayP3);


   return 0;
}
// #else
// int main()
// {
//     std::cout << "This tutorial code needs the xfeatures2d contrib module to be run." << std::endl;
//     return 0;
// }
// #endif