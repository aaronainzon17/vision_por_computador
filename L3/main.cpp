#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include "iostream"
#include <math.h>
#include "descriptores.hpp"

using namespace std;
using namespace cv;

/*Implementacion de la matriz del kernel a mano y tras eso utilizar el la func filter2D para aplicar el gradiente*/

int thresh = 100;
const int max_thresh = 255;

RNG rng(12345);

int main(int, char**) {

    // Reading image
    Mat img = imread("../imagenesL3/reco1.pgm");
    // Display original image
    imshow("original Image", img);
    waitKey(0);

    // Convert to graycsale
    Mat img_gray;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    // Blur the image for better edge detection
    Mat img_blur;
    GaussianBlur(img_gray, img_blur, Size(3,3), 0);
    imshow("Escala de grises", img_gray);
    waitKey(0);
    
    // Mat otsu;
    // threshold(img_blur,otsu,0,255, THRESH_BINARY_INV | THRESH_OTSU);
    // imshow("Otsu img", otsu);
    // waitKey(0);
    Mat otsu = sacarOtsu(img_blur);

    
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
    

    //Eliminar los que sean menores que un determinado tamanyo
    vector<vector<Point>> contours;
    findContours(figure_bin, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    
    //Se sacan los momentos de los contornos
    vector<Moments> mu(contours.size() );
    for( size_t i = 0; i < contours.size(); i++ )
    {
        mu[i] = moments( contours[i] );
    }

    //Siguiente parte
    vector<Point2f> mc( contours.size() );
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
    double areasContornos[contours.size()];
    double diametroContornos[contours.size()];
    cout << "\t Info: Area and Contour Length \n";
    for( size_t i = 0; i < contours.size(); i++ )
    {
        cout << " * Contour[" << i << "] - Area (M_00) = " << std::fixed << std::setprecision(2) << mu[i].m00
             << " - Area OpenCV: " << contourArea(contours[i]) << " - Length: " << arcLength( contours[i], true ) << endl;
        areasContornos[i] = contourArea(contours[i]);
        diametroContornos[i] = arcLength( contours[i], true );
    }

    //If area del contorno es menor que 1000 entonces se ignora
    //Si contourArea(contours[i]) < 1000 entonces se elimna



    
    return 0;
}
 /*cargo la imagen
 cargo el analisis de conectividad 
 me quedo con un solo bloob 
 y evaluo con lo de mahalanovis y tal */





 /*
    
    Explicacion de monica
    Para reconoces usar el metodo de minima distancia
    Para cada tipo de objeto tendras una media y una varianza
 Tendras unas cuantas imagenes deentrenamiento para cada clase
    Esta bien guardarlos en un fichero
    en la primera fila por ejemplo la clase vagón
    y sera
                area            perimetro
           media  varianza   media   varianza
    vagon
    ....
    ....

    Luego le pasaremos al programa los descriptores y los blobs y dira que figura es

    Sacas la distancia de mahalanobis de x respecto a las clases que tengo
    Luego dado el valor de alpha y m (dimensionalidad de mi espacio de descriptores)

    podemos hacer cargar una imagen, me quedo con una sola imagen, la que quiero reconocer, y voy diciendo cual es
    */






   //Mat adaptative;

    //adaptiveThreshold(img_blur, adaptative, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV,11,2);
    //adaptiveThreshold(img_blur,adaptative,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY_INV,11,2);
    //imshow("Adaptative img", adaptative);
    //waitKey(0);

    //Mat adaptative_g;

    //adaptiveThreshold(img_blur, adaptative, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV,11,2);
    //adaptiveThreshold(img_blur,adaptative_g,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY_INV,11,2);
    //imshow("Adaptative_g img", adaptative_g);
    //waitKey(0);

    //Se va utilizar otsu porque se pueden obtener lod blobs de la imagen de forma muy sencilla
    //Gracias a que deja todos los objetos blancos

    //cv::connectedComponents (InputArray image, OutputArray labels, int connectivity=8, int ltype=CV_32S)
    //https://anishdubey.com/connected-component-analysis-image-opencv














    // cout<<"Acaba findContours"<<endl;
    // int maxContour = 0; 
    // for (int i = 0; i < contours.size(); i++){
    //     double area = contourArea(contours[i]);
    //     cout<<"Area de "<< i << "es "<<area<<endl;
    //     if (area > maxContour) {
    //         maxContour = i;
    //     }
    // }
    
    // cout << "el area es de: " << maxContour << endl;
    
    // waitKey(0);
    
    // //Calcular momentos
    // vector<Moments> mu(contours.size() );
    // for( size_t h = 1; h < contours.size() ; h++ )
    // {
    //     mu[h] = moments( contours[h] ); //Momento del blob correspondiente
    // }

    // //Se calculan los no se que es esto
    // vector<Point2f> mc( contours.size() );
    // for( size_t h = 0; h < contours.size(); h++ )
    // {
    //     //add 1e-5 to avoid division by zero
    //     mc[h] = Point2f( static_cast<float>(mu[h].m10 / (mu[h].m00 + 1e-5)),
    //                     static_cast<float>(mu[h].m01 / (mu[h].m00 + 1e-5)) );
    //     cout << "mc[" << h << "]=" << mc[h] << endl;
    // }
    // imshow("Bloob 1", figure_bin);
    // waitKey(0);
    // // threshold( src_gray, dst, threshold_value, max_binary_value, threshold_type );
    // // imshow( window_name, dst );