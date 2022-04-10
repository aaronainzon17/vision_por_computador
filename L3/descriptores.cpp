#include "descriptores.hpp"

Mat sacarOtsu(Mat img_blur){
    Mat otsu;
    //Se segmenta la imagen con el metodo Otsu para convertirla en una imagen binaria
    threshold(img_blur,otsu,0,255, THRESH_BINARY_INV | THRESH_OTSU);

    return otsu;
}


Mat sacarBlobs(Mat otsu){
    Mat img_components;
    //Se obtienen los blobs de la imagen binaria
    int nBloobs = connectedComponents(otsu, img_components);
    Mat figure_bin(img_components.size(), CV_8UC1);
    //Se vuelve a convertir en imagen binaria tras obtener los blobs
    for (int i = 0; i < img_components.rows; i++){
        for (int j = 0; j < img_components.cols; j++){
            if (img_components.at<int>(i,j) >= 1 &&  img_components.at<int>(i,j) <= 6){
                figure_bin.at<uchar>(i,j) = 255;
            }else{
                figure_bin.at<uchar>(i,j) = 0;
            }
        }
    }
    //imshow("Blobs",figure_bin);
    return figure_bin;
}

void sacarDescriptores(vector<vector<Point>> contours, Mat figure_bin,RNG rng, vector<Moments>& mu, vector<Point2f>& mc, double areasContornos[],double diametroContornos[]){
    //Dados los contornos de la imagen se obtienen los momentos
    for( size_t i = 0; i < contours.size(); i++ )
    {
        mu[i] = moments( contours[i] );
    }
    //Se obtiene el centro del contorno
    for( size_t i = 0; i < contours.size(); i++ )
    {
        //add 1e-5 to avoid division by zero
        mc[i] = Point2f( static_cast<float>(mu[i].m10 / (mu[i].m00 + 1e-5)),
                         static_cast<float>(mu[i].m01 / (mu[i].m00 + 1e-5)) );
    }

    Mat drawing = Mat::zeros( figure_bin.size(), CV_8UC3 );
    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
        drawContours( drawing, contours, (int)i, color, 2 );
        circle( drawing, mc[i], 4, color, -1 );

    }
    // imshow("contornos",drawing);
    // waitKey(0);

    //Se obtienen el area y el perimetro de los contornos
    for( size_t i = 0; i < contours.size(); i++ )
    {
        areasContornos[i] = contourArea(contours[i]);
        diametroContornos[i] = arcLength( contours[i], true );
    }
}