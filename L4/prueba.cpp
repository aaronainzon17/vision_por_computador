// #define _USE_MATH_DEFINES

// #include "opencv2/opencv.hpp"
// #include <iostream>
// #include <filesystem>
// #include <cmath>
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



void FindMatches(cv::Mat BaseImage, cv::Mat SecImage, std::vector<cv::DMatch>& GoodMatches, std::vector<cv::KeyPoint>& BaseImage_kp, std::vector<cv::KeyPoint>& SecImage_kp)
{
	using namespace cv;
	//using namespace cv::xfeatures2d;

	// Using SIFT to find the keypointsand decriptors in the images
	//Ptr<SIFT> Sift = SIFT::create();
    cv::Ptr<cv::xfeatures2d::SIFT> Sift = cv::xfeatures2d::SIFT::create(1000,3,0.04,10,1.6);
	cv::Mat BaseImage_des, SecImage_des;
	cv::Mat BaseImage_Gray, SecImage_Gray;
	cv::cvtColor(BaseImage, BaseImage_Gray, cv::COLOR_BGR2GRAY);
	cv::cvtColor(SecImage, SecImage_Gray, cv::COLOR_BGR2GRAY);
	Sift->detectAndCompute(BaseImage_Gray, cv::noArray(), BaseImage_kp, BaseImage_des);
	Sift->detectAndCompute(SecImage_Gray, cv::noArray(), SecImage_kp, SecImage_des);

	// Using Brute Force matcher to find matches.
	//cv::BFMatcher BF_Matcher;
    Ptr<DescriptorMatcher> BF_Matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
	std::vector<std::vector<cv::DMatch>> InitialMatches;
    //matcher->knnMatch( descriptorP1, descriptorP2, matches12,2);
	BF_Matcher->knnMatch(BaseImage_des, SecImage_des, InitialMatches, 2);

	// Applying ratio test and filtering out the good matches.
	for (int i = 0; i < InitialMatches.size(); ++i)
	{
		if (InitialMatches[i][0].distance < 0.75 * InitialMatches[i][1].distance)
		{
			GoodMatches.push_back(InitialMatches[i][0]);
		}
	}
}


void FindHomography(std::vector<cv::DMatch> Matches, std::vector<cv::KeyPoint> BaseImage_kp, std::vector<cv::KeyPoint> SecImage_kp, cv::Mat& HomographyMatrix)
{
	// If less than 4 matches found, exit the code.
	if (Matches.size() < 4)
	{	
		std::cout << "\nNot enough matches found between the images.\n";
		exit(0);
	}
	// Storing coordinates of points corresponding to the matches found in both the images
	std::vector<cv::Point2f> BaseImage_pts, SecImage_pts;
	for (int i = 0 ; i < Matches.size() ; i++)
	{
		cv::DMatch Match = Matches[i];
		BaseImage_pts.push_back(BaseImage_kp[Match.queryIdx].pt);
		SecImage_pts.push_back(SecImage_kp[Match.trainIdx].pt);
	}

	// Finding the homography matrix(transformation matrix).
	HomographyMatrix = cv::findHomography(SecImage_pts, BaseImage_pts, cv::RANSAC, (4.0));
}



void GetNewFrameSizeAndMatrix(cv::Mat &HomographyMatrix, int* Sec_ImageShape, int* Base_ImageShape, int* NewFrameSize, int* Correction)
{
	// Reading the size of the image
	int Height = Sec_ImageShape[0], Width = Sec_ImageShape[1];

	// Taking the matrix of initial coordinates of the corners of the secondary image
	// Stored in the following format : [[x1, x2, x3, x4], [y1, y2, y3, y4], [1, 1, 1, 1]]
	// Where(xi, yi) is the coordinate of the i th corner of the image.
	double initialMatrix[3][4] = { {0, (double)Width - 1, (double)Width - 1, 0},
								   {0, 0, (double)Height - 1, (double)Height - 1},
								   {1.0, 1.0, 1.0, 1.0} };
	cv::Mat InitialMatrix = cv::Mat(3, 4, CV_64F, initialMatrix);// .inv();


	// Finding the final coordinates of the corners of the image after transformation.
	// NOTE: Here, the coordinates of the corners of the frame may go out of the
	// frame(negative values).We will correct this afterwards by updating the
	// homography matrix accordingly.
	cv::Mat FinalMatrix = HomographyMatrix * InitialMatrix;

	cv::Mat x = FinalMatrix(cv::Rect(0, 0, FinalMatrix.cols, 1));
	cv::Mat y = FinalMatrix(cv::Rect(0, 1, FinalMatrix.cols, 1));
	cv::Mat c = FinalMatrix(cv::Rect(0, 2, FinalMatrix.cols, 1));

	
	cv::Mat x_by_c = x.mul(1 / c);
	cv::Mat y_by_c = y.mul(1 / c);

	// Finding the dimentions of the stitched image frame and the "Correction" factor
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

	// Again correcting New_Widthand New_Height
	// Helpful when secondary image is overlaped on the left hand side of the Base image.
	New_Width = (New_Width < Base_ImageShape[1] + Correction[0]) ? Base_ImageShape[1] + Correction[0] : New_Width;
	New_Height = (New_Height < Base_ImageShape[0] + Correction[1]) ? Base_ImageShape[0] + Correction[1] : New_Height;


	// Finding the coordinates of the corners of the image if they all were within the frame.
	cv::add(x_by_c, Correction[0], x_by_c);
	cv::add(y_by_c, Correction[1], y_by_c);


	cv::Point2f OldInitialPoints[4], NewFinalPonts[4];
	OldInitialPoints[0] = cv::Point2f(0, 0);
	OldInitialPoints[1] = cv::Point2f(Width - 1, 0);
	OldInitialPoints[2] = cv::Point2f(Width - 1, Height - 1);
	OldInitialPoints[3] = cv::Point2f(0, Height - 1);
	for (int i = 0; i < 4; i++) 
		NewFinalPonts[i] = cv::Point2f(x_by_c.at<double>(0, i), y_by_c.at<double>(0, i));


	// Updating the homography matrix.Done so that now the secondary image completely
	// lies inside the frame
	HomographyMatrix = cv::getPerspectiveTransform(OldInitialPoints, NewFinalPonts);

	// Setting variable for returning
	NewFrameSize[0] = New_Height; NewFrameSize[1] = New_Width;

}

cv::Mat StitchImages(cv::Mat BaseImage, cv::Mat SecImage)
{
	// Finding matches between the 2 images and their keypoints
	std::vector<cv::DMatch> Matches;
	std::vector<cv::KeyPoint> BaseImage_kp, SecImage_kp;
	FindMatches(BaseImage, SecImage, Matches, BaseImage_kp, SecImage_kp);

	// Finding homography matrix.
	cv::Mat HomographyMatrix;
	FindHomography(Matches, BaseImage_kp, SecImage_kp, HomographyMatrix);
	
	
	// Finding size of new frame of stitched images and updating the homography matrix
	int Sec_ImageShape[2] = { SecImage.rows, SecImage.cols };
	int Base_ImageShape[2] = { BaseImage.rows, BaseImage.cols };
	int NewFrameSize[2], Correction[2];
	//NewFrameSize, Correction, HomographyMatrix = 
	GetNewFrameSizeAndMatrix(HomographyMatrix, Sec_ImageShape, Base_ImageShape, NewFrameSize, Correction);

	
	// Finally placing the images upon one another.
	cv::Mat StitchedImage;
	cv::warpPerspective(SecImage, StitchedImage, HomographyMatrix, cv::Size(NewFrameSize[1], NewFrameSize[0]));	
	BaseImage.copyTo(StitchedImage(cv::Rect(Correction[0], Correction[1], BaseImage.cols, BaseImage.rows)));

	return StitchedImage;
}

int main()
{
	// cv::Mat Image1 = cv::imread("../BuildingScene/poster1.jpg");
	// cv::Mat Image2 = cv::imread("../BuildingScene/poster2.jpg");

	// // Checking if images read
	// if (Image1.empty() || Image2.empty())
	// {
	// 	std::cout << "\nImages not read properly or does not exist.\n";
	// 	exit(0);
	// }

	// // Calling function for stitching images.
	// cv::Mat StitchedImage = StitchImages(Image1, Image2);
    // int down_width = 512;
	// int down_height = 384;
    // resize(StitchedImage, StitchedImage, Size(down_width, down_height), INTER_LINEAR);
	// cv::imshow("Stitched Images", StitchedImage);
	// cv::waitKey(0);




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
         //Quieres sacar una foto y a??adirla al frame
         //cout<<"Probando teclas"<<endl;
         fotos_tomadas++;
         if(fotos_tomadas == 1){
            cout<<"Se necesitan al menos dos fotos para generar el panorama"<<endl;
            primeraImagen = frame;
            cvtColor(primeraImagen, primeraImagenGray, COLOR_BGR2GRAY);
         }else if(fotos_tomadas == 2){
            imagen = frame;
            // cvtColor(imagen, imagen_gray, COLOR_BGR2GRAY);
            // panorama = anyadirImagen(primeraImagen,primeraImagenGray,imagen,imagen_gray);
            // //panorama = anyadirImagen(imagen,imagen_gray,primeraImagen,primeraImagenGray);
            // cvtColor(panorama, panorama_gray, COLOR_BGR2GRAY);
            panorama = StitchImages(primeraImagen, imagen);
            imshow("Panorama",panorama);
            //waitKey(0);
         }else{
            imagen = frame;
            cvtColor(imagen, imagen_gray, COLOR_BGR2GRAY);
            panorama = StitchImages(panorama, imagen);
            //panorama = anyadirImagen(imagen,imagen_gray,panorama,panorama_gray);
            cvtColor(panorama, panorama_gray, COLOR_BGR2GRAY);
            imshow("Panorama",panorama);
            //waitKey(0);
         }
      }
   }	

	return 0;
}