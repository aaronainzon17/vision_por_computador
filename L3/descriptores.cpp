#include "descriptores.hpp"

Mat sacarOtsu(Mat img_blur){
    Mat otsu;
    threshold(img_blur,otsu,0,255, THRESH_BINARY_INV | THRESH_OTSU);
    imshow("Otsu img", otsu);
    waitKey(0);

    return otsu;
}


Mat sacarBlobs(Mat otsu){
    Mat img_components;
    
    int nBloobs = connectedComponents(otsu, img_components);

    cout << "Se han reconocido: " << nBloobs << endl;
    
    //imshow("connected components", img_components);
    //waitKey(0);

    Mat figure_bin(img_components.size(), CV_8UC1);
    // B 1 -> raya borde 
    // B 2 -> Circulo
    // B 3 -> Triangulo
    // B 4 -> Vagon
    // B 5 -> Rueda
    
    //Nos quedamos con el contorno mas grande
    //El de mayor area y perimetro
    cout << "filas " << img_components.rows << " columnas " << img_components.cols << endl; 
    waitKey(0);

    //Para mostrar los blobs. Habra que ajustar el tamanyo del blob
    for (int i = 0; i < img_components.rows; i++){
        for (int j = 0; j < img_components.cols; j++){
            if (img_components.at<int>(i,j) >= 1 &&  img_components.at<int>(i,j) <= 6){
                figure_bin.at<uchar>(i,j) = 255;
            }else{
                figure_bin.at<uchar>(i,j) = 0;
            }
            //cout << "[ " << i << "," << j << "]"<< endl;
        }
    }

    imshow("Bloooob", figure_bin);
    //imshow("Bloooob", img_components); 
    waitKey(0);
    return figure_bin;
}

void sacarDescriptores(vector<vector<Point>> contours, Mat figure_bin,RNG rng, vector<Moments>& mu, vector<Point2f>& mc, double areasContornos[],double diametroContornos[]){
    for( size_t i = 0; i < contours.size(); i++ )
    {
        mu[i] = moments( contours[i] );
    }

    //Siguiente parte
    //vector<Point2f> mc( contours.size() );
    for( size_t i = 0; i < contours.size(); i++ )
    {
        //add 1e-5 to avoid division by zero
        mc[i] = Point2f( static_cast<float>(mu[i].m10 / (mu[i].m00 + 1e-5)),
                         static_cast<float>(mu[i].m01 / (mu[i].m00 + 1e-5)) );
        cout << "mc[" << i << "]=" << mc[i] << endl;
    }

    Mat drawing = Mat::zeros( figure_bin.size(), CV_8UC3 );
    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
        drawContours( drawing, contours, (int)i, color, 2 );
        circle( drawing, mc[i], 4, color, -1 );
    }
    imshow( "Contours", drawing );
    waitKey(0);
    // double areasContornos[contours.size()];
    // double diametroContornos[contours.size()];
    cout << "\t Info: Area and Contour Length \n";
    for( size_t i = 0; i < contours.size(); i++ )
    {
        cout << " * Contour[" << i << "] - Area (M_00) = " << std::fixed << std::setprecision(2) << mu[i].m00
             << " - Area OpenCV: " << contourArea(contours[i]) << " - Length: " << arcLength( contours[i], true ) << endl;
        areasContornos[i] = contourArea(contours[i]);
        diametroContornos[i] = arcLength( contours[i], true );
    }
}