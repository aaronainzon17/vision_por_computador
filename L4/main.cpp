#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/core/types_c.h>
//#include "opencv2/features2d.hpp"
//#include "opencv2/xfeatures2d.hpp"

#include "opencv2/xfeatures2d/nonfree.hpp"
#include <string.h>
//#ifdef HAVE_OPENCV_XFEATURES2D

using namespace cv;
using namespace cv::xfeatures2d;
using std::cout;
using std::endl;


double ratio = 0.8;

void probarHarris(Mat src, Mat img_gray,Mat srcP2,Mat img_grayP2,Mat srcP3,Mat img_grayP3){
   //Ptr<ORB> detector = ORB::create( );
   Ptr<ORB> detector = ORB::create(500,1.2f,8,31, 0, 2,ORB::HARRIS_SCORE, 31,20);
   //int nfeatures=500, float scaleFactor=1.2f, int nlevels=8, int edgeThreshold=31, int firstLevel=0, int WTA_K=2, int scoreType=ORB::HARRIS_SCORE, int patchSize=31, int fastThreshold=20
   std::vector<KeyPoint> keypoints, keypointsP2, keypointsP3;
   detector->detect( img_gray, keypoints);
   detector->detect(img_grayP2,keypointsP2);
   detector->detect(img_grayP3,keypointsP3);
   Mat descriptorP1,descriptorP2,descriptorP3;
   detector->compute(img_gray,keypoints,descriptorP1);
   detector->compute(img_grayP2,keypointsP2,descriptorP2);
   detector->compute(img_grayP3,keypointsP3,descriptorP3);
   std::vector< std::vector<DMatch> > matches12, matches13,matches21,matches23,matches31,matches32;
   Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
   matcher->knnMatch( descriptorP1, descriptorP2, matches12, 2);
   matcher->knnMatch( descriptorP1, descriptorP3, matches13, 2);
   matcher->knnMatch( descriptorP2, descriptorP3, matches23, 2);
   Mat result1,result2,result3;
   drawMatches(src, keypoints, srcP2, keypointsP2, matches12 ,result1);
   imshow("Matches Harris_1-2",result1);
   waitKey(0);
   drawMatches(src, keypoints, srcP3, keypointsP3, matches13 ,result2);
   imshow("Matches Harris_1-3",result2);
   waitKey(0);
   drawMatches(srcP2, keypointsP2, srcP3, keypointsP3, matches23 ,result3);
   imshow("Matches Harris_2-3",result3);
   waitKey(0);
   //Apartado3
   std::vector<DMatch> good_matches12, good_matches13,good_matches23;
   for(int i=0; i < matches12.size(); i++)
   {
      if(matches12[i][0].distance < ratio * matches12[i][1].distance)
         good_matches12.push_back(matches12[i][0]);
   }
   for(int i=0; i < matches13.size(); i++)
   {
      if(matches13[i][0].distance < ratio * matches13[i][1].distance)
         good_matches13.push_back(matches13[i][0]);
   }
   for(int i=0; i < matches23.size(); i++)
   {
      if(matches23[i][0].distance < ratio * matches23[i][1].distance)
         good_matches23.push_back(matches23[i][0]);
   }

   Mat goodResult1,goodResult2,goodResult3;
   drawMatches(src, keypoints, srcP2, keypointsP2, good_matches12 ,goodResult1,Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
   imshow("Good Matches Harris_1-2",goodResult1);
   waitKey(0);
   drawMatches(src, keypoints, srcP3, keypointsP3, good_matches13 ,goodResult2,Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
   imshow("Good Matches Harris_1-3",goodResult2);
   waitKey(0);
   drawMatches(srcP2, keypointsP2, srcP3, keypointsP3, good_matches23 ,goodResult3,Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
   imshow("Good Matches Harris_2-3",goodResult3);
   waitKey(0);
 
}

void probarSurf(Mat src, Mat img_gray,Mat srcP2,Mat img_grayP2,Mat srcP3,Mat img_grayP3){
   //-- Step 1: Detect the keypoints using SURF Detector
   int minHessian = 400;
   Ptr<SURF> detector = SURF::create(minHessian);
   std::vector<KeyPoint> keypoints, keypointsP2, keypointsP3;
   detector->detect(img_gray, keypoints);
   detector->detect(img_grayP2,keypointsP2);
   detector->detect(img_grayP3,keypointsP3);
   Mat descriptorP1,descriptorP2,descriptorP3;
   detector->compute(img_gray,keypoints,descriptorP1);
   detector->compute(img_grayP2,keypointsP2,descriptorP2);
   detector->compute(img_grayP3,keypointsP3,descriptorP3);
   std::vector< std::vector<DMatch> > matches12, matches13,matches23;
   Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
   matcher->knnMatch( descriptorP1, descriptorP2, matches12, 2);
   matcher->knnMatch( descriptorP1, descriptorP3, matches13, 2);
   matcher->knnMatch( descriptorP2, descriptorP3, matches23, 2);
   Mat result1,result2,result3;
   drawMatches(src, keypoints, srcP2, keypointsP2, matches12 ,result1);
   imshow("Matches Surf_1-2",result1);
   waitKey(0);
   drawMatches(src, keypoints, srcP3, keypointsP3, matches13 ,result2);
   imshow("Matches Surf_1-3",result2);
   waitKey(0);
   drawMatches(srcP2, keypointsP2, srcP3, keypointsP3, matches23 ,result3);
   imshow("Matches Surf_2-3",result3);
   waitKey(0);
   //Apartado3
   std::vector<DMatch> good_matches12, good_matches13,good_matches23;
   for(int i=0; i < matches12.size(); i++)
   {
      if(matches12[i][0].distance < ratio * matches12[i][1].distance)
         good_matches12.push_back(matches12[i][0]);
   }
   for(int i=0; i < matches13.size(); i++)
   {
      if(matches13[i][0].distance < ratio * matches13[i][1].distance)
         good_matches13.push_back(matches13[i][0]);
   }
   for(int i=0; i < matches23.size(); i++)
   {
      if(matches23[i][0].distance < ratio * matches23[i][1].distance)
         good_matches23.push_back(matches23[i][0]);
   }

      //  drawMatches( img_object, keypoints_object, img_scene, keypoints_scene, good_matches, img_matches, Scalar::all(-1),
      //            Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
   Mat goodResult1,goodResult2,goodResult3;
   drawMatches(src, keypoints, srcP2, keypointsP2, good_matches12 ,goodResult1,Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
   imshow("Good Matches Surf_1-2",goodResult1);
   waitKey(0);
   drawMatches(src, keypoints, srcP3, keypointsP3, good_matches13 ,goodResult2,Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
   imshow("Good Matches Surf_1-3",goodResult2);
   waitKey(0);
   drawMatches(srcP2, keypointsP2, srcP3, keypointsP3, good_matches23 ,goodResult3,Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
   imshow("Good Matches Surf_2-3",goodResult3);
   waitKey(0);
}

void probarSift(Mat src, Mat img_gray,Mat srcP2,Mat img_grayP2,Mat srcP3,Mat img_grayP3){
   cv::Ptr<cv::xfeatures2d::SIFT> detector = cv::xfeatures2d::SIFT::create();
   //extractor = DescriptorExtractor::create("ORB");
   std::vector<cv::KeyPoint> keypoints, keypointsP2, keypointsP3;
   detector->detect(img_gray, keypoints);
   detector->detect(img_grayP2,keypointsP2);
   detector->detect(img_grayP3,keypointsP3);
   Mat descriptorP1,descriptorP2,descriptorP3;
   detector->compute(img_gray,keypoints,descriptorP1);
   detector->compute(img_grayP2,keypointsP2,descriptorP2);
   detector->compute(img_grayP3,keypointsP3,descriptorP3);
   //Apartado2
   std::vector< std::vector<DMatch> > matches12, matches13,matches21,matches23,matches31,matches32;
   Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
   matcher->knnMatch( descriptorP1, descriptorP2, matches12, 2);
   matcher->knnMatch( descriptorP1, descriptorP3, matches13, 2);
   matcher->knnMatch( descriptorP2, descriptorP3, matches23, 2);
   Mat result1,result2,result3;
   drawMatches(src, keypoints, srcP2, keypointsP2, matches12 ,result1);
   imshow("Matches Sift_1-2",result1);
   waitKey(0);
   drawMatches(src, keypoints, srcP3, keypointsP3, matches13 ,result2);
   imshow("Matches Sift_1-3",result2);
   waitKey(0);
   drawMatches(srcP2, keypointsP2, srcP3, keypointsP3, matches23 ,result3);
   imshow("Matches Sift_2-3",result3);
   waitKey(0);
   //Apartado3
   std::vector<DMatch> good_matches12, good_matches13,good_matches23;
   for(int i=0; i < matches12.size(); i++)
   {
      if(matches12[i][0].distance < ratio * matches12[i][1].distance)
         good_matches12.push_back(matches12[i][0]);
   }
   for(int i=0; i < matches13.size(); i++)
   {
      if(matches13[i][0].distance < ratio * matches13[i][1].distance)
         good_matches13.push_back(matches13[i][0]);
   }
   for(int i=0; i < matches23.size(); i++)
   {
      if(matches23[i][0].distance < ratio * matches23[i][1].distance)
         good_matches23.push_back(matches23[i][0]);
   }

   Mat goodResult1,goodResult2,goodResult3;
   drawMatches(src, keypoints, srcP2, keypointsP2, good_matches12 ,goodResult1,Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
   imshow("Good Matches Sift_1-2",goodResult1);
   waitKey(0);
   drawMatches(src, keypoints, srcP3, keypointsP3, good_matches13 ,goodResult2,Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
   imshow("Good Matches Sift_1-3",goodResult2);
   waitKey(0);
   drawMatches(srcP2, keypointsP2, srcP3, keypointsP3, good_matches23 ,goodResult3,Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
   imshow("Good Matches Sift_2-3",goodResult3);
   waitKey(0);

}

void probarOrb(Mat src, Mat img_gray,Mat srcP2,Mat img_grayP2,Mat srcP3,Mat img_grayP3){
   //Ptr<ORB> detector = ORB::create( );
   Ptr<ORB> detector = ORB::create(500,1.2f,8,31, 0, 2,ORB::FAST_SCORE, 31,20);
   //int nfeatures=500, float scaleFactor=1.2f, int nlevels=8, int edgeThreshold=31, int firstLevel=0, int WTA_K=2, int scoreType=ORB::HARRIS_SCORE, int patchSize=31, int fastThreshold=20
   std::vector<KeyPoint> keypoints, keypointsP2, keypointsP3;
   detector->detect( img_gray, keypoints );
   detector->detect(img_grayP2,keypointsP2);
   detector->detect(img_grayP3,keypointsP3);
   Mat descriptorP1,descriptorP2,descriptorP3;
   detector->compute(img_gray,keypoints,descriptorP1);
   detector->compute(img_grayP2,keypointsP2,descriptorP2);
   detector->compute(img_grayP3,keypointsP3,descriptorP3);
   std::vector< std::vector<DMatch> > matches12, matches13,matches21,matches23,matches31,matches32;
   Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
   matcher->knnMatch( descriptorP1, descriptorP2, matches12, 2);
   matcher->knnMatch( descriptorP1, descriptorP3, matches13, 2);
   matcher->knnMatch( descriptorP2, descriptorP3, matches23, 2);
   Mat result1,result2,result3;
   drawMatches(src, keypoints, srcP2, keypointsP2, matches12 ,result1);
   imshow("Matches Orb_1-2",result1);
   waitKey(0);
   drawMatches(src, keypoints, srcP3, keypointsP3, matches13 ,result2);
   imshow("Matches Orb_1-3",result2);
   waitKey(0);
   drawMatches(srcP2, keypointsP2, srcP3, keypointsP3, matches23 ,result3);
   imshow("Matches Orb_2-3",result3);
   waitKey(0);
   //Apartado3
   std::vector<DMatch> good_matches12, good_matches13,good_matches23;
   for(int i=0; i < matches12.size(); i++)
   {
      if(matches12[i][0].distance < ratio * matches12[i][1].distance)
         good_matches12.push_back(matches12[i][0]);
   }
   for(int i=0; i < matches13.size(); i++)
   {
      if(matches13[i][0].distance < ratio * matches13[i][1].distance)
         good_matches13.push_back(matches13[i][0]);
   }
   for(int i=0; i < matches23.size(); i++)
   {
      if(matches23[i][0].distance < ratio * matches23[i][1].distance)
         good_matches23.push_back(matches23[i][0]);
   }

   Mat goodResult1,goodResult2,goodResult3;
   drawMatches(src, keypoints, srcP2, keypointsP2, good_matches12 ,goodResult1,Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
   imshow("Good Matches Orb_1-2",goodResult1);
   waitKey(0);
   drawMatches(src, keypoints, srcP3, keypointsP3, good_matches13 ,goodResult2,Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
   imshow("Good Matches Orb_1-3",goodResult2);
   waitKey(0);
   drawMatches(srcP2, keypointsP2, srcP3, keypointsP3, good_matches23 ,goodResult3,Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
   imshow("Good Matches Orb_2-3",goodResult3);
   waitKey(0);
   
}

void probarAkaze(Mat src, Mat img_gray,Mat srcP2,Mat img_grayP2,Mat srcP3,Mat img_grayP3){
   Ptr<AKAZE> detector = AKAZE::create( );
   std::vector<KeyPoint> keypoints, keypointsP2, keypointsP3;
   detector->detect( img_gray, keypoints );
   detector->detect(img_grayP2,keypointsP2);
   detector->detect(img_grayP3,keypointsP3);
   Mat descriptorP1,descriptorP2,descriptorP3;
   detector->compute(img_gray,keypoints,descriptorP1);
   detector->compute(img_grayP2,keypointsP2,descriptorP2);
   detector->compute(img_grayP3,keypointsP3,descriptorP3);
   std::vector< std::vector<DMatch> > matches12, matches13,matches21,matches23,matches31,matches32;
   Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
   matcher->knnMatch( descriptorP1, descriptorP2, matches12, 2);
   matcher->knnMatch( descriptorP1, descriptorP3, matches13, 2);
   matcher->knnMatch( descriptorP2, descriptorP3, matches23, 2);
   Mat result1,result2,result3;
   drawMatches(src, keypoints, srcP2, keypointsP2, matches12 ,result1);
   imshow("Matches Akaze_1-2",result1);
   waitKey(0);
   drawMatches(src, keypoints, srcP3, keypointsP3, matches13 ,result2);
   imshow("Matches Akaze_1-3",result2);
   waitKey(0);
   drawMatches(srcP2, keypointsP2, srcP3, keypointsP3, matches23 ,result3);
   imshow("Matches Akaze_2-3",result3);
   waitKey(0);
   //Apartado3
   std::vector<DMatch> good_matches12, good_matches13,good_matches23;
   for(int i=0; i < matches12.size(); i++)
   {
      if(matches12[i][0].distance < ratio * matches12[i][1].distance)
         good_matches12.push_back(matches12[i][0]);
   }
   for(int i=0; i < matches13.size(); i++)
   {
      if(matches13[i][0].distance < ratio * matches13[i][1].distance)
         good_matches13.push_back(matches13[i][0]);
   }
   for(int i=0; i < matches23.size(); i++)
   {
      if(matches23[i][0].distance < ratio * matches23[i][1].distance)
         good_matches23.push_back(matches23[i][0]);
   }

   Mat goodResult1,goodResult2,goodResult3;
   drawMatches(src, keypoints, srcP2, keypointsP2, good_matches12 ,goodResult1);
   imshow("Good Matches Akaze_1-2",goodResult1);
   waitKey(0);
   drawMatches(src, keypoints, srcP3, keypointsP3, good_matches13 ,goodResult2);
   imshow("Good Matches Akaze_1-3",goodResult2);
   waitKey(0);
   drawMatches(srcP2, keypointsP2, srcP3, keypointsP3, good_matches23 ,goodResult3);
   imshow("Good Matches Akaze_2-3",goodResult3);
   waitKey(0);
   
}

void apartado2(Mat src, Mat img_gray,Mat srcP2,Mat img_grayP2,Mat srcP3,Mat img_grayP3){
   //Tras analizar lo del appartado1 se ha decidido usar SIFT porque es el mejor
   cv::Ptr<cv::xfeatures2d::SIFT> detector = cv::xfeatures2d::SIFT::create();
   //extractor = DescriptorExtractor::create("ORB");
   std::vector<cv::KeyPoint> keypoints, keypointsP2, keypointsP3;
   detector->detect(img_gray, keypoints);
   detector->detect(img_grayP2,keypointsP2);
   detector->detect(img_grayP3,keypointsP3);
   Mat descriptorP1,descriptorP2,descriptorP3;
   detector->compute(img_gray,keypoints,descriptorP1);
   detector->compute(img_grayP2,keypointsP2,descriptorP2);
   detector->compute(img_grayP3,keypointsP3,descriptorP3);
   //Apartado2
   std::vector< std::vector<DMatch> > matches12, matches13,matches21,matches23,matches31,matches32;
   Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
   matcher->knnMatch( descriptorP1, descriptorP2, matches12, 2);
   matcher->knnMatch( descriptorP1, descriptorP3, matches13, 2);
   matcher->knnMatch( descriptorP2, descriptorP3, matches23, 2);
   
   //Apartado3
   std::vector<DMatch> good_matches12, good_matches13,good_matches23;
   for(int i=0; i < matches12.size(); i++)
   {
      if(matches12[i][0].distance < ratio * matches12[i][1].distance)
         good_matches12.push_back(matches12[i][0]);
   }
   for(int i=0; i < matches13.size(); i++)
   {
      if(matches13[i][0].distance < ratio * matches13[i][1].distance)
         good_matches13.push_back(matches13[i][0]);
   }
   for(int i=0; i < matches23.size(); i++)
   {
      if(matches23[i][0].distance < ratio * matches23[i][1].distance)
         good_matches23.push_back(matches23[i][0]);
   }

   std::vector<Point2f> im1;
   std::vector<Point2f> im2;
   for( size_t i = 0; i < good_matches12.size(); i++ )
   {
      //-- Get the keypoints from the good matches
      im1.push_back( keypoints[ good_matches12[i].queryIdx ].pt );
      im2.push_back( keypointsP2[ good_matches12[i].trainIdx ].pt );
   }
   Mat H = findHomography( im1, im2, RANSAC );
   //imshow("Homografia",H);
   //waitKey(0);

   // Transformar perspectiva para imagen 2
   std::vector<Point2f> cuatroPuntos;
   cuatroPuntos.push_back(Point2f (0,0));
   cuatroPuntos.push_back(Point2f (src.size().width,0));
   cuatroPuntos.push_back(Point2f (0, src.size().height));
   cuatroPuntos.push_back(Point2f (src.size().width, src.size().height));
   Mat MDestino;
   perspectiveTransform(Mat(cuatroPuntos), MDestino, H);

   // Calcular esquinas de imagen 2
   double min_x, min_y, tam_x, tam_y;
   float min_x1, min_x2, min_y1, min_y2, max_x1, max_x2, max_y1, max_y2;
   min_x1 = min(MDestino.at<Point2f>(0).x, MDestino.at<Point2f>(1).x);
   min_x2 = min(MDestino.at<Point2f>(2).x, MDestino.at<Point2f>(3).x);
   min_y1 = min(MDestino.at<Point2f>(0).y, MDestino.at<Point2f>(1).y);
   min_y2 = min(MDestino.at<Point2f>(2).y, MDestino.at<Point2f>(3).y);
   max_x1 = max(MDestino.at<Point2f>(0).x, MDestino.at<Point2f>(1).x);
   max_x2 = max(MDestino.at<Point2f>(2).x, MDestino.at<Point2f>(3).x);
   max_y1 = max(MDestino.at<Point2f>(0).y, MDestino.at<Point2f>(1).y);
   max_y2 = max(MDestino.at<Point2f>(2).y, MDestino.at<Point2f>(3).y);
   min_x = min(min_x1, min_x2);
   min_y = min(min_y1, min_y2);
   tam_x = max(max_x1, max_x2);
   tam_y = max(max_y1, max_y2);

   // Matriz de transformación
   Mat Htr = Mat::eye(3,3,CV_64F);
   if (min_x < 0){
      tam_x = srcP2.size().width - min_x;
      Htr.at<double>(0,2)= -min_x;
   }
   if (min_y < 0){
      tam_y = srcP2.size().height - min_y;
      Htr.at<double>(1,2)= -min_y;
   }

   // Construir panorama
   Mat Panorama;
   Panorama = Mat(Size(tam_x,tam_y), CV_32F);
   warpPerspective(srcP2, Panorama,     Htr, Panorama.size(), INTER_LINEAR, BORDER_CONSTANT,   0);
   warpPerspective(src, Panorama, (Htr*H), Panorama.size(), INTER_LINEAR, BORDER_TRANSPARENT,0);

   imshow("Panorama",Panorama);
   waitKey(0);

   // Mat warpImage2;
   // warpPerspective(srcP2, warpImage2, H, Size(srcP2.cols*2, srcP2.rows*2), INTER_CUBIC);

   // Mat final(Size(srcP2.cols*2 + src.cols, srcP2.rows*2),CV_8UC3);

   // //velikost img1
   // Mat roi1(final, Rect(0, 0,  src.cols, src.rows));
   // Mat roi2(final, Rect(0, 0, warpImage2.cols, warpImage2.rows));
   // warpImage2.copyTo(roi2);
   // src.copyTo(roi1);
   // imshow("final", final);
   // waitKey(0);
}

Mat anyadirImagen(Mat src, Mat img_gray,Mat srcP2,Mat img_grayP2){
   cv::Ptr<cv::xfeatures2d::SIFT> detector = cv::xfeatures2d::SIFT::create();
   std::vector<cv::KeyPoint> keypoints, keypointsP2;
   detector->detect(img_gray, keypoints);
   detector->detect(img_grayP2,keypointsP2);
   Mat descriptorP1,descriptorP2;
   detector->compute(img_gray,keypoints,descriptorP1);
   detector->compute(img_grayP2,keypointsP2,descriptorP2);
   //Apartado2
   std::vector< std::vector<DMatch> > matches12;
   Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
   matcher->knnMatch( descriptorP1, descriptorP2, matches12, 2);
   
   //Apartado3
   std::vector<DMatch> good_matches12;
   for(int i=0; i < matches12.size(); i++)
   {
      if(matches12[i][0].distance < ratio * matches12[i][1].distance)
         good_matches12.push_back(matches12[i][0]);
   }

   std::vector<Point2f> im1;
   std::vector<Point2f> im2;
   for( size_t i = 0; i < good_matches12.size(); i++ )
   {
      //-- Get the keypoints from the good matches
      im1.push_back( keypoints[ good_matches12[i].queryIdx ].pt );
      im2.push_back( keypointsP2[ good_matches12[i].trainIdx ].pt );
   }
   //Mat H = findHomography( im1, im2, RANSAC );
   Mat H = findHomography( im1, im2, RANSAC );
   // Transformar perspectiva para imagen 2
   std::vector<Point2f> cuatroPuntos;
   cuatroPuntos.push_back(Point2f (0,0));
   cuatroPuntos.push_back(Point2f (src.size().width,0));
   cuatroPuntos.push_back(Point2f (0, src.size().height));
   cuatroPuntos.push_back(Point2f (src.size().width, src.size().height));


   Mat MDestino;
   perspectiveTransform(Mat(cuatroPuntos), MDestino, H);

   // Calcular esquinas de imagen 2
   double min_x, min_y, tam_x, tam_y;
   float min_x1, min_x2, min_y1, min_y2, max_x1, max_x2, max_y1, max_y2;
   min_x1 = min(MDestino.at<Point2f>(0).x, MDestino.at<Point2f>(1).x);
   min_x2 = min(MDestino.at<Point2f>(2).x, MDestino.at<Point2f>(3).x);
   min_y1 = min(MDestino.at<Point2f>(0).y, MDestino.at<Point2f>(1).y);
   min_y2 = min(MDestino.at<Point2f>(2).y, MDestino.at<Point2f>(3).y);
   max_x1 = max(MDestino.at<Point2f>(0).x, MDestino.at<Point2f>(1).x);
   max_x2 = max(MDestino.at<Point2f>(2).x, MDestino.at<Point2f>(3).x);
   max_y1 = max(MDestino.at<Point2f>(0).y, MDestino.at<Point2f>(1).y);
   max_y2 = max(MDestino.at<Point2f>(2).y, MDestino.at<Point2f>(3).y);
   min_x = min(min_x1, min_x2);
   min_y = min(min_y1, min_y2);
   tam_x = max(max_x1, max_x2);
   tam_y = max(max_y1, max_y2);

   // Matriz de transformación
   Mat Htr = Mat::eye(3,3,CV_64F);
   if (min_x < 0){
      tam_x = srcP2.size().width - min_x;
      Htr.at<double>(0,2)= -min_x;
   }
   if (min_y < 0){
      tam_y = srcP2.size().height - min_y;
      Htr.at<double>(1,2)= -min_y;
   }

   // Construir panorama
   Mat Panorama;
   Panorama = Mat(Size(tam_x,tam_y), CV_32F);
   warpPerspective(srcP2, Panorama,  Htr, Panorama.size(), INTER_LINEAR, BORDER_CONSTANT,   0);
   warpPerspective(src, Panorama, (Htr*H), Panorama.size(), INTER_LINEAR, BORDER_TRANSPARENT,0);

   return Panorama;
}


int main( int argc, char* argv[] )
{
   int fotos_tomadas = 0;
   Mat frame,panorama,panorama_gray,primeraImagen,primeraImagenGray,imagen,imagen_gray;
   VideoCapture cap;
   int deviceID = 0;             // 0 = open default camera
   int apiID = cv::CAP_ANY;      // 0 = autodetect default API
   cap.open(deviceID, apiID);
   if (!cap.isOpened()) {
      cout << "ERROR! Unable to open camera\n";
      return -1;
   }
   cout << "Empezando a grabar" << endl
      << "Pulsa ESC para acabar la toma de imagenes" << endl
      << "Pulsa t para tomar una foto y anyadirla al panorama" <<endl;

   for (;;)
   {
      // wait for a new frame from camera and store it into 'frame'
      cap.read(frame);
      // check if we succeeded
      if (frame.empty()) {
         cout << "ERROR! blank frame grabbed\n";
         break;
      }
      char key = (char) cv::waitKey(30);
      imshow("Live", frame);
      if (key == 27){
         break;
      }
      if(key=='t'){
         //Quieres sacar una foto y añadirla al frame
         //cout<<"Probando teclas"<<endl;
         fotos_tomadas++;
         if(fotos_tomadas == 1){
            cout<<"Se necesitan al menos dos fotos para generar el panorama"<<endl;
            primeraImagen = frame;
            cvtColor(primeraImagen, primeraImagenGray, COLOR_BGR2GRAY);
         }else if(fotos_tomadas == 2){
            imagen = frame;
            cvtColor(imagen, imagen_gray, COLOR_BGR2GRAY);
            panorama = anyadirImagen(primeraImagen,primeraImagenGray,imagen,imagen_gray);
            cvtColor(panorama, panorama_gray, COLOR_BGR2GRAY);
            imshow("Panorama",panorama);
            //waitKey(0);
         }else{
            imagen = frame;
            cvtColor(imagen, imagen_gray, COLOR_BGR2GRAY);
            panorama = anyadirImagen(panorama,panorama_gray,imagen,imagen_gray);
            cvtColor(panorama, panorama_gray, COLOR_BGR2GRAY);
            imshow("Panorama",panorama);
            //waitKey(0);
         }
      }
   }	


   return 0;
}
//Sift es mejor
//Que parametros ir cambiado: de primeras se toca el numero de features y un poco los thresholds
//Que cosa hay que mirar para ver cual es mejor: se podrá mirar al hacer lo del panorama


// Mat srcP1 = imread("../BuildingScene/poster1.jpg");
   // Mat srcP2 = imread("../BuildingScene/poster2.jpg");
   // Mat srcP3 = imread("../BuildingScene/poster3.jpg");
   // Mat srcC1 = imread("../BuildingScene/ciudad1.jpeg");
   // Mat srcC2 = imread("../BuildingScene/ciudad2.jpeg");
   // Mat srcC3 = imread("../BuildingScene/ciudad3.jpeg");
   // int down_width = 512;
	// int down_height = 384;
	// resize(srcP1, srcP1, Size(down_width, down_height), INTER_LINEAR);
   // resize(srcP2, srcP2, Size(down_width, down_height), INTER_LINEAR);
   // resize(srcP3, srcP3, Size(down_width, down_height), INTER_LINEAR);
   // resize(srcC1, srcC1, Size(down_width, down_height), INTER_LINEAR);
   // resize(srcC2, srcC2, Size(down_width, down_height), INTER_LINEAR);
   // resize(srcC3, srcC3, Size(down_width, down_height), INTER_LINEAR);
   // //Pasar a nivel de grises
   // Mat img_grayP1, img_grayP2, img_grayP3;
   // cvtColor(srcP1, img_grayP1, COLOR_BGR2GRAY);
   // cvtColor(srcP2, img_grayP2, COLOR_BGR2GRAY);
   // cvtColor(srcP3, img_grayP3, COLOR_BGR2GRAY);
   // Mat img_grayC1, img_grayC2, img_grayC3;
   // cvtColor(srcC1, img_grayC1, COLOR_BGR2GRAY);
   // cvtColor(srcC2, img_grayC2, COLOR_BGR2GRAY);
   // cvtColor(srcC3, img_grayC3, COLOR_BGR2GRAY);
   
   // //Imagen 2D
   // probarSurf(srcP1,img_grayP1,srcP2,img_grayP2,srcP3,img_grayP3);
   // destroyAllWindows();
   // probarSift(srcP1,img_grayP1,srcP2,img_grayP2,srcP3,img_grayP3);
   // destroyAllWindows();
   // probarHarris(srcP1,img_grayP1,srcP2,img_grayP2,srcP3,img_grayP3);
   // destroyAllWindows();
   // probarOrb(srcP1,img_grayP1,srcP2,img_grayP2,srcP3,img_grayP3);
   // destroyAllWindows();
   // probarAkaze(srcP1,img_grayP1,srcP2,img_grayP2,srcP3,img_grayP3);
   
   // //Escenario 3D
   // probarSurf(srcC1,img_grayC1,srcC2,img_grayC2,srcC3,img_grayC3);
   // destroyAllWindows();
   // probarSift(srcC1,img_grayC1,srcC2,img_grayC2,srcC3,img_grayC3);
   // destroyAllWindows();
   // probarHarris(srcC1,img_grayC1,srcC2,img_grayC2,srcC3,img_grayC3);
   // destroyAllWindows();
   // probarOrb(srcC1,img_grayC1,srcC2,img_grayC2,srcC3,img_grayC3);
   // destroyAllWindows();
   // probarAkaze(srcC1,img_grayC1,srcC2,img_grayC2,srcC3,img_grayC3);
   // destroyAllWindows();

   //Apartado2
   //La idea es coger las dos primeras imagenes y sacar los keypoints
   //Sacar la imagen unida que sera la final
   //A partir de ahi vas añadiendo las nuevas imagenes a la imagen final

   //Prueba con el poster
   // Mat panorama;
   // panorama = anyadirImagen(srcP1,img_grayP1,srcP2,img_grayP2);
   // Mat img_grayPanorama;
   // cvtColor(panorama, img_grayPanorama, COLOR_BGR2GRAY);
   // imshow("Panorama1",panorama);
   // waitKey(0);
   // panorama = anyadirImagen(panorama,img_grayPanorama,srcP3,img_grayP3);
   // imshow("Panorama2",panorama);
   // waitKey(0);

   // Mat b1 = imread("../BuildingScene/building1.JPG");
   // Mat b2= imread("../BuildingScene/building2.JPG");
   // Mat b3 = imread("../BuildingScene/building3.JPG");
   // Mat b4 = imread("../BuildingScene/building4.JPG");
   // Mat b5 = imread("../BuildingScene/building5.JPG");
   
	// resize(b1, b1, Size(down_width, down_height), INTER_LINEAR);
   // resize(b2, b2, Size(down_width, down_height), INTER_LINEAR);
   // resize(b3, b3, Size(down_width, down_height), INTER_LINEAR);
   // resize(b4, b4, Size(down_width, down_height), INTER_LINEAR);
   // resize(b5, b5, Size(down_width, down_height), INTER_LINEAR);
   // Mat gb1, gb2, gb3,gb4,gb5;
   // cvtColor(b1, gb1, COLOR_BGR2GRAY);
   // cvtColor(b2, gb2, COLOR_BGR2GRAY);
   // cvtColor(b3, gb3, COLOR_BGR2GRAY);
   // cvtColor(b4, gb4, COLOR_BGR2GRAY);
   // cvtColor(b5, gb5, COLOR_BGR2GRAY);
   // Mat panorama,gp;
   // panorama = anyadirImagen(b1,gb1,b2,gb2);
   // imshow("Panorama",panorama);
   // waitKey(0);
   // cvtColor(panorama, gp, COLOR_BGR2GRAY);
   // panorama = anyadirImagen(panorama,gp,b3,gb3);
   // imshow("Panorama",panorama);
   // waitKey(0);
   // cvtColor(panorama, gp, COLOR_BGR2GRAY);
   // panorama = anyadirImagen(panorama,gp,b4,gb4);
   // imshow("Panorama",panorama);
   // waitKey(0);
   // cvtColor(panorama, gp, COLOR_BGR2GRAY);
   // panorama = anyadirImagen(panorama,gp,b5,gb5);
   // resize(panorama, panorama, Size(1024, 768), INTER_LINEAR);
   // imshow("Panorama",panorama);
   // waitKey(0);


