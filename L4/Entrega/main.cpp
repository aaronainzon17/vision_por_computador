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

/**
 * La funcion muestra los keypoints y los matches de varias imagenes usando el detector Harris para detectar keypoints
 * y Fuerza Bruta para detectar los matches entre keypoints. Además se usará el ratio al segundo vecino para detectar buenos matches
**/ 
void probarHarris(Mat src, Mat img_gray,Mat srcP2,Mat img_grayP2,Mat srcP3,Mat img_grayP3){
    //Se crea el detector
   Ptr<ORB> detector = ORB::create(500,1.2f,8,31, 0, 2,ORB::HARRIS_SCORE, 31,20);
   std::vector<KeyPoint> keypoints, keypointsP2, keypointsP3;
   //Se obtienen los keypoints de cada imagen
   detector->detect( img_gray, keypoints);
   detector->detect(img_grayP2,keypointsP2);
   detector->detect(img_grayP3,keypointsP3);
   Mat descriptorP1,descriptorP2,descriptorP3;
   //Se obtienen los descriptores de cada keypoint
   detector->compute(img_gray,keypoints,descriptorP1);
   detector->compute(img_grayP2,keypointsP2,descriptorP2);
   detector->compute(img_grayP3,keypointsP3,descriptorP3);
   std::vector< std::vector<DMatch> > matches12, matches13,matches21,matches23,matches31,matches32;
   //Se obtienen los matches entre keypoints con el metodo de Fuerza Bruta
   Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
   matcher->knnMatch( descriptorP1, descriptorP2, matches12, 2);
   matcher->knnMatch( descriptorP1, descriptorP3, matches13, 2);
   matcher->knnMatch( descriptorP2, descriptorP3, matches23, 2);
   Mat result1,result2,result3;
   //Se dibujan los matches entre keypoints
   drawMatches(src, keypoints, srcP2, keypointsP2, matches12 ,result1);
   imshow("Matches Harris_1-2",result1);
   waitKey(0);
   drawMatches(src, keypoints, srcP3, keypointsP3, matches13 ,result2);
   imshow("Matches Harris_1-3",result2);
   waitKey(0);
   drawMatches(srcP2, keypointsP2, srcP3, keypointsP3, matches23 ,result3);
   //Apartado3
   //Se obtienen los "buenos matches" con el ratio al segundo vecino
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
   //Se muestran los buenos matches
   Mat goodResult1,goodResult2,goodResult3;
   drawMatches(src, keypoints, srcP2, keypointsP2, good_matches12 ,goodResult1,Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
   imshow("Good Matches Harris_1-2",goodResult1);
   waitKey(0);
   drawMatches(src, keypoints, srcP3, keypointsP3, good_matches13 ,goodResult2,Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
   imshow("Good Matches Harris_1-3",goodResult2);
   waitKey(0);
   drawMatches(srcP2, keypointsP2, srcP3, keypointsP3, good_matches23 ,goodResult3,Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
   // imshow("Good Matches Harris_2-3",goodResult3);
   // waitKey(0);
 
}

/**
 * La funcion muestra los keypoints y los matches de varias imagenes usando el detector Surf para detectar keypoints
 * y Flann para detectar los matches entre keypoints. Además se usará el ratio al segundo vecino para detectar buenos matches
**/ 
void probarSurf(Mat src, Mat img_gray,Mat srcP2,Mat img_grayP2,Mat srcP3,Mat img_grayP3){
   int minHessian = 400;
   //Se crea el detector
   Ptr<SURF> detector = SURF::create(minHessian);
   std::vector<KeyPoint> keypoints, keypointsP2, keypointsP3;
   //Se obtienen los keypoints de cada imagen
   detector->detect(img_gray, keypoints);
   detector->detect(img_grayP2,keypointsP2);
   detector->detect(img_grayP3,keypointsP3);
   Mat descriptorP1,descriptorP2,descriptorP3;
   //Se obtienen los descriptores de cada keypoint
   detector->compute(img_gray,keypoints,descriptorP1);
   detector->compute(img_grayP2,keypointsP2,descriptorP2);
   detector->compute(img_grayP3,keypointsP3,descriptorP3);
   std::vector< std::vector<DMatch> > matches12, matches13,matches23;
   //Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
   Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
   //Se obtienen los matches entre keypoints con el metodo Flann
   matcher->knnMatch( descriptorP1, descriptorP2, matches12, 2);
   matcher->knnMatch( descriptorP1, descriptorP3, matches13, 2);
   matcher->knnMatch( descriptorP2, descriptorP3, matches23, 2);
   Mat result1,result2,result3;
   //Se dibujan los matches entre keypoints
   drawMatches(src, keypoints, srcP2, keypointsP2, matches12 ,result1);
   imshow("Matches Surf_1-2",result1);
   waitKey(0);
   drawMatches(src, keypoints, srcP3, keypointsP3, matches13 ,result2);
   imshow("Matches Surf_1-3",result2);
   waitKey(0);
   drawMatches(srcP2, keypointsP2, srcP3, keypointsP3, matches23 ,result3);
   //Apartado3
   //Se obtienen los "buenos matches" con el ratio al segundo vecino
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
   //Se muestran los buenos matches
   Mat goodResult1,goodResult2,goodResult3;
   drawMatches(src, keypoints, srcP2, keypointsP2, good_matches12 ,goodResult1,Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
   imshow("Good Matches Surf_1-2",goodResult1);
   waitKey(0);
   drawMatches(src, keypoints, srcP3, keypointsP3, good_matches13 ,goodResult2,Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
   imshow("Good Matches Surf_1-3",goodResult2);
   waitKey(0);
   drawMatches(srcP2, keypointsP2, srcP3, keypointsP3, good_matches23 ,goodResult3,Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS); 
}

/**
 * La funcion muestra los keypoints y los matches de varias imagenes usando el detector Sift para detectar keypoints
 * y Flann para detectar los matches entre keypoints. Además se usará el ratio al segundo vecino para detectar buenos matches
**/ 
void probarSift(Mat src, Mat img_gray,Mat srcP2,Mat img_grayP2,Mat srcP3,Mat img_grayP3){
   cv::Ptr<cv::xfeatures2d::SIFT> detector = cv::xfeatures2d::SIFT::create();
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
   //Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
   Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
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
   // imshow("Matches Sift_2-3",result3);
   // waitKey(0);
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

}

/**
 * La funcion muestra los keypoints y los matches de varias imagenes usando el detector Orb para detectar keypoints
 * y Fuerza Bruta para detectar los matches entre keypoints. Además se usará el ratio al segundo vecino para detectar buenos matches
**/ 
void probarOrb(Mat src, Mat img_gray,Mat srcP2,Mat img_grayP2,Mat srcP3,Mat img_grayP3){
   Ptr<ORB> detector = ORB::create(500,1.2f,8,31, 0, 2,ORB::FAST_SCORE, 31,20);
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
   //Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
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
}

/**
 * La funcion muestra los keypoints y los matches de varias imagenes usando el detector Akaze para detectar keypoints
 * y Fuerza Bruta para detectar los matches entre keypoints. Además se usará el ratio al segundo vecino para detectar buenos matches
**/ 
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
   //Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
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

}




/**
 * La funcion encuentra los keypoints y los matches de las dos imagenes
 * Se utiliza el detector Sift y Flann para detectar matches
**/ 
void FindMatches(Mat BaseImage, Mat SecImage, std::vector<DMatch>& GoodMatches, std::vector<KeyPoint>& BaseImage_kp, std::vector<KeyPoint>& SecImage_kp)
{

   cv::Ptr<cv::xfeatures2d::SIFT> Sift = cv::xfeatures2d::SIFT::create(1000,3,0.04,10,1.6);
	cv::Mat BaseImage_des, SecImage_des;
	cv::Mat BaseImage_Gray, SecImage_Gray;
	cv::cvtColor(BaseImage, BaseImage_Gray, cv::COLOR_BGR2GRAY);
	cv::cvtColor(SecImage, SecImage_Gray, cv::COLOR_BGR2GRAY);
	Sift->detectAndCompute(BaseImage_Gray, cv::noArray(), BaseImage_kp, BaseImage_des);
	Sift->detectAndCompute(SecImage_Gray, cv::noArray(), SecImage_kp, SecImage_des);

   Ptr<DescriptorMatcher> Matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
	std::vector<std::vector<cv::DMatch>> InitialMatches;
	Matcher->knnMatch(BaseImage_des, SecImage_des, InitialMatches, 2);
   //Se filtran los matches con el ratio al segundo vecino
	for (int i = 0; i < InitialMatches.size(); ++i)
	{
		if (InitialMatches[i][0].distance < 0.75 * InitialMatches[i][1].distance)
		{
			GoodMatches.push_back(InitialMatches[i][0]);
		}
	}
}

/**
 * La funcion devuelve la homografia entre los keypoints de las dos imagenes
 * 
**/ 
void FindHomography(std::vector<cv::DMatch> Matches, std::vector<cv::KeyPoint> BaseImage_kp, std::vector<cv::KeyPoint> SecImage_kp, cv::Mat& HomographyMatrix)
{
	//Si hay muy pocos matches no se hace nada
	if (Matches.size() < 4)
	{	
		std::cout << "\nNot enough matches found between the images.\n";
		exit(0);
	}
	std::vector<cv::Point2f> BaseImage_pts, SecImage_pts;
	for (int i = 0 ; i < Matches.size() ; i++)
	{
		cv::DMatch Match = Matches[i];
		BaseImage_pts.push_back(BaseImage_kp[Match.queryIdx].pt);
		SecImage_pts.push_back(SecImage_kp[Match.trainIdx].pt);
	}

	//Se obtiene la homografia
	HomographyMatrix = cv::findHomography(SecImage_pts, BaseImage_pts, cv::RANSAC, (4.0));
}



void GetNewFrameSizeAndMatrix(cv::Mat &HomographyMatrix, int* Sec_ImageShape, int* Base_ImageShape, int* NewFrameSize, int* Correction)
{
	//Se obtiene el tamanyo de la imagen
	int Height = Sec_ImageShape[0], Width = Sec_ImageShape[1];

   //Se almacenan los esquinas de la imagen con la forma:
   // [x1,x2,x3,x4] , [y1,y2,y3,y4] , [1,1,1,1]
   // (xi,yi) es la coordenada de cada esquina
	double initialMatrix[3][4] = { {0, (double)Width - 1, (double)Width - 1, 0},
								   {0, 0, (double)Height - 1, (double)Height - 1},
								   {1.0, 1.0, 1.0, 1.0} };
	cv::Mat InitialMatrix = cv::Mat(3, 4, CV_64F, initialMatrix);// .inv();

   //Encontrar las coordenadas de las esquinas despues de la transformacion
   //Si H representa que punto de una imagen corresponde en otra
   //Entonces multiplicar las esquinas por la homografia nos dice donde estaran las esquinas en el panorama
	cv::Mat FinalMatrix = HomographyMatrix * InitialMatrix;
	cv::Mat x = FinalMatrix(cv::Rect(0, 0, FinalMatrix.cols, 1));
	cv::Mat y = FinalMatrix(cv::Rect(0, 1, FinalMatrix.cols, 1));
	cv::Mat c = FinalMatrix(cv::Rect(0, 2, FinalMatrix.cols, 1));

	cv::Mat x_by_c = x.mul(1 / c);
	cv::Mat y_by_c = y.mul(1 / c);

	// Se encuentra la direccion de la imagen a anyadir al panorama ya que se puede anyadir en cualquier direccion
	double min_x, max_x, min_y, max_y;
	cv::minMaxLoc(x_by_c, &min_x, &max_x);
	cv::minMaxLoc(y_by_c, &min_y, &max_y);
	min_x = (int)round(min_x); max_x = (int)round(max_x);
	min_y = (int)round(min_y); max_y = (int)round(max_y);
	
	int New_Width = max_x, New_Height = max_y;
	Correction[0] = 0; Correction[1] = 0;
	if (min_x < 0)
	{
		New_Width -= min_x;
		Correction[0] = abs(min_x);
	}
	if (min_y < 0)
	{
		New_Height -= min_y;
		Correction[1] = abs(min_y);
	}

   //Se corrige el tamanyo
	New_Width = (New_Width < Base_ImageShape[1] + Correction[0]) ? Base_ImageShape[1] + Correction[0] : New_Width;
	New_Height = (New_Height < Base_ImageShape[0] + Correction[1]) ? Base_ImageShape[0] + Correction[1] : New_Height;

   //Se encuentran las coordenadas de las esquinas de la imagen si todas estuvieran dentro del marco.
	cv::add(x_by_c, Correction[0], x_by_c);
	cv::add(y_by_c, Correction[1], y_by_c);

	cv::Point2f OldInitialPoints[4], NewFinalPonts[4];
	OldInitialPoints[0] = cv::Point2f(0, 0);
	OldInitialPoints[1] = cv::Point2f(Width - 1, 0);
	OldInitialPoints[2] = cv::Point2f(Width - 1, Height - 1);
	OldInitialPoints[3] = cv::Point2f(0, Height - 1);
	for (int i = 0; i < 4; i++) 
		NewFinalPonts[i] = cv::Point2f(x_by_c.at<double>(0, i), y_by_c.at<double>(0, i));


   //Se actualiza la homografia para que la imagen nueva este dentro del frame
	HomographyMatrix = cv::getPerspectiveTransform(OldInitialPoints, NewFinalPonts);

	// Setting variable for returning
	NewFrameSize[0] = New_Height; NewFrameSize[1] = New_Width;
}

/**
 * Se juntan las funciones anteriores para obtener el panorama
 * 
**/ 
cv::Mat StitchImages(cv::Mat BaseImage, cv::Mat SecImage)
{
	std::vector<cv::DMatch> Matches;
	std::vector<cv::KeyPoint> BaseImage_kp, SecImage_kp;
	FindMatches(BaseImage, SecImage, Matches, BaseImage_kp, SecImage_kp);

	cv::Mat HomographyMatrix;
	FindHomography(Matches, BaseImage_kp, SecImage_kp, HomographyMatrix);
	
	int Sec_ImageShape[2] = { SecImage.rows, SecImage.cols };
	int Base_ImageShape[2] = { BaseImage.rows, BaseImage.cols };
	int NewFrameSize[2], Correction[2];
	
	GetNewFrameSizeAndMatrix(HomographyMatrix, Sec_ImageShape, Base_ImageShape, NewFrameSize, Correction);

	
	//Se coloca la imagen con la otra
	cv::Mat StitchedImage;
	cv::warpPerspective(SecImage, StitchedImage, HomographyMatrix, cv::Size(NewFrameSize[1], NewFrameSize[0]));	
	BaseImage.copyTo(StitchedImage(cv::Rect(Correction[0], Correction[1], BaseImage.cols, BaseImage.rows)));

	return StitchedImage;
}



int main( int argc, char* argv[] )
{

if(argc != 2){
   cout<<"Numero de parametros erroneo. Formato: ./main <opcion>"<<endl;
   cout<<"Opcion 1 -> Se ejecutaran las distintas pruebas para extraer los keypoints"<<endl;
   cout<<"Opcion 2 -> Se obtendra un panorama de las fotos de la camara"<<endl;
   return 0;
}

//////////////////////////////////////////////////
//Apartado1
/////////////////////////////////////////////////
int opcion = atoi(argv[1]);
int down_width = 512;
	int down_height = 384;
   //resize(srcP1, srcP1, Size(down_width, down_height), INTER_LINEAR);
if(opcion == 1){
   Mat srcP1 = imread("../BuildingScene/poster1.jpg");
   Mat srcP2 = imread("../BuildingScene/poster2.jpg");
   Mat srcP3 = imread("../BuildingScene/poster3.jpg");
   Mat srcC1 = imread("../BuildingScene/ciudad1.jpeg");
   Mat srcC2 = imread("../BuildingScene/ciudad2.jpeg");
   Mat srcC3 = imread("../BuildingScene/ciudad3.jpeg");
   
	resize(srcP1, srcP1, Size(down_width, down_height), INTER_LINEAR);
   resize(srcP2, srcP2, Size(down_width, down_height), INTER_LINEAR);
   resize(srcP3, srcP3, Size(down_width, down_height), INTER_LINEAR);
   resize(srcC1, srcC1, Size(down_width, down_height), INTER_LINEAR);
   resize(srcC2, srcC2, Size(down_width, down_height), INTER_LINEAR);
   resize(srcC3, srcC3, Size(down_width, down_height), INTER_LINEAR);
   //Pasar a nivel de grises
   Mat img_grayP1, img_grayP2, img_grayP3;
   cvtColor(srcP1, img_grayP1, COLOR_BGR2GRAY);
   cvtColor(srcP2, img_grayP2, COLOR_BGR2GRAY);
   cvtColor(srcP3, img_grayP3, COLOR_BGR2GRAY);
   Mat img_grayC1, img_grayC2, img_grayC3;
   cvtColor(srcC1, img_grayC1, COLOR_BGR2GRAY);
   cvtColor(srcC2, img_grayC2, COLOR_BGR2GRAY);
   cvtColor(srcC3, img_grayC3, COLOR_BGR2GRAY);
   
   //Imagen 2D
   probarSurf(srcP1,img_grayP1,srcP2,img_grayP2,srcP3,img_grayP3);
   destroyAllWindows();
   probarSift(srcP1,img_grayP1,srcP2,img_grayP2,srcP3,img_grayP3);
   //probarSiftFlann(srcP1,img_grayP1,srcP2,img_grayP2,srcP3,img_grayP3);
   destroyAllWindows();
   probarHarris(srcP1,img_grayP1,srcP2,img_grayP2,srcP3,img_grayP3);
   destroyAllWindows();
   probarOrb(srcP1,img_grayP1,srcP2,img_grayP2,srcP3,img_grayP3);
   destroyAllWindows();
   probarAkaze(srcP1,img_grayP1,srcP2,img_grayP2,srcP3,img_grayP3);
   
   //Escenario 3D
   probarSurf(srcC1,img_grayC1,srcC2,img_grayC2,srcC3,img_grayC3);
   destroyAllWindows();
   probarSift(srcC1,img_grayC1,srcC2,img_grayC2,srcC3,img_grayC3);
   destroyAllWindows();
   probarHarris(srcC1,img_grayC1,srcC2,img_grayC2,srcC3,img_grayC3);
   destroyAllWindows();
   probarOrb(srcC1,img_grayC1,srcC2,img_grayC2,srcC3,img_grayC3);
   destroyAllWindows();
   probarAkaze(srcC1,img_grayC1,srcC2,img_grayC2,srcC3,img_grayC3);
   destroyAllWindows();

}else{
   //////////////////////////////////////////////////
   //Apartado2
   /////////////////////////////////////////////////
   // Mat srcP1 = imread("../BuildingScene/poster1.jpg");
   // Mat srcP2 = imread("../BuildingScene/poster2.jpg");
   // Mat pegar = StitchImages(srcP1, srcP2);
   // int down_width = 1024;
	// int down_height = 768;
   // resize(pegar, pegar, Size(down_width, down_height), INTER_LINEAR);
   // imshow("Prueba", pegar);
   // waitKey(0);

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
            panorama = StitchImages(primeraImagen, primeraImagen);
            imshow("Panorama",panorama);
            //cvtColor(primeraImagen, primeraImagenGray, COLOR_BGR2GRAY);
         }else{
            imagen = frame;
            panorama = StitchImages(panorama, imagen);
            imshow("Panorama",panorama);
            //waitKey(0);
         }
      }
   }	

}

   return 0;
}
