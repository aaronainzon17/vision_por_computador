#include "aprender.hpp"

RNG rng(12345);

void aprender(string nomfich, Moments& mu_x, Point2f& mc_x, double& area_x, double& diametro_x)
{
    //Devuelve el mu , mc , area , diametro del objeto del fichero
    // Reading image
    Mat img = imread(nomfich);
    //Mat img = imread("../imagenesL3/circulo1.pgm");
    // Display original image
    // imshow("original Image", img);
    // waitKey(0);

    // Convert to graycsale
    Mat img_gray;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    // Blur the image for better edge detection
    Mat img_blur;
    GaussianBlur(img_gray, img_blur, Size(3,3), 0);
    // imshow("Escala de grises", img_gray);
    // waitKey(0);

    Mat otsu = sacarOtsu(img_blur);
    // // B 1 -> raya borde 
    // // B 2 -> Circulo
    // // B 3 -> Triangulo
    // // B 4 -> Vagon
    // // B 5 -> Rueda
    Mat figure_bin  = sacarBlobs(otsu);
    

    //Eliminar los que sean menores que un determinado tamanyo
    vector<vector<Point>> contours;
    findContours(figure_bin, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    
    //Se sacan los momentos de los contornos
    
    vector<Moments> mu(contours.size() );
    double areasContornos[contours.size()];
    double diametroContornos[contours.size()];
    vector<Point2f> mc( contours.size() );

    sacarDescriptores(contours, figure_bin, rng ,mu , mc, areasContornos, diametroContornos);


    for(size_t i = 0; i < contours.size();i++){
        if(areasContornos[i] >= 1000){  //En la imagen aparecen varios blobs pero se elige el mas grande que sera el objeto
            mu_x = mu.at(i);
            mc_x = mc.at(i);
            area_x = areasContornos[i];
            diametro_x = diametroContornos[i];
        }
    }

    //Para ese fichero se han obtenidos los descriptores
}


void aprenderTodo(){
    //Para cada objeto sacar los descriptores de las fotos y sacar la media
    //Aqui se almacena cada descriptor de cada foto (seran 5) y se saca la media y varianza de cada descriptor
    vector<Moments> momentos;
    vector<double> area;
    vector<double> diametro;
    double mediaArea,mediaDiametro;

    vector<string> figuras;
    figuras.push_back("circulo");figuras.push_back("rectangulo");figuras.push_back("rueda");figuras.push_back("triangulo");figuras.push_back("vagon");
    string path = "../imagenesL3/";
    vector<string> fichFigura;
    fichFigura.push_back("1.pgm");fichFigura.push_back("2.pgm");fichFigura.push_back("3.pgm");fichFigura.push_back("4.pgm");fichFigura.push_back("5.pgm");
    int i=0;
    string fichero;
    for(string s : figuras){
        //Para cada figura
        i=0;
        for(string f : fichFigura){

        }
    }



    

}