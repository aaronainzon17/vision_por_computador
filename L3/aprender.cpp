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

double sacarMedia(vector<double> muestra){
    double media=0;
    for(double valor : muestra){
        media += valor;
    }
    return media/(double)muestra.size();
}

double sacarVarianza(vector<double> muestra, double media){
    double varianza=0;
    for(double valor : muestra){
        varianza += pow(valor - media,2);
    }
    return varianza/((double)(muestra.size()-1));
}

void aprenderTodo(){
    //Se crea el fichero en el que escribir
    //El formato sera:
        //objeto
        //mediaArea,mediaDiametro,varianzaArea,varianzaDiametro
    ofstream myfile;
    myfile.open ("objetos");
    //myfile << "Writing this to a file.\n";
    

    //Para cada objeto sacar los descriptores de las fotos y sacar la media
    //Aqui se almacena cada descriptor de cada foto (seran 5) y se saca la media y varianza de cada descriptor
    vector<double> inv1,inv2,inv3;
    vector<double> area;
    vector<double> diametro;
    double mediaArea,mediaDiametro, varianzaArea,varianzaDiametro,mediaInv1,mediaInv2,mediaInv3,varianzaInv1,varianzaInv2,varianzaInv3;

    vector<string> figuras;
    figuras.push_back("circulo");figuras.push_back("rectangulo");figuras.push_back("rueda");figuras.push_back("triangulo");figuras.push_back("vagon");
    string path = "../imagenesL3/";
    vector<string> fichFigura;
    fichFigura.push_back("1.pgm");fichFigura.push_back("2.pgm");fichFigura.push_back("3.pgm");fichFigura.push_back("4.pgm");fichFigura.push_back("5.pgm");
    string fichero;
    Moments momento_x;
    double area_x, diametro_x;
    Point2f mc_x;
    double huMoments[7];
    for(string s : figuras){
        //Para cada figura
        myfile << s<<"\n";
        for(string f : fichFigura){
            fichero = path + s + f;
            aprender(fichero, momento_x, mc_x, area_x,diametro_x);
            //Cojo el moments y saco los tres primeros invariantes
            HuMoments(momento_x,huMoments);
            inv1.push_back(huMoments[0]);
            inv2.push_back(huMoments[1]);
            inv3.push_back(huMoments[2]);
            area.push_back(area_x);
            diametro.push_back(diametro_x);
        }
        //Sacar la media y la varianza de cada 
        mediaArea = sacarMedia(area);
        mediaDiametro = sacarMedia(diametro);
        varianzaArea = sacarVarianza(area,mediaArea);
        varianzaDiametro = sacarVarianza(diametro,mediaDiametro);
        mediaInv1 = sacarMedia(inv1);
        mediaInv2 = sacarMedia(inv2);
        mediaInv3 = sacarMedia(inv3);
        varianzaInv1 = sacarVarianza(inv1,mediaInv1);
        varianzaInv2= sacarVarianza(inv2,mediaInv2);
        varianzaInv3= sacarVarianza(inv3,mediaInv3);
        myfile<<mediaArea<<","<<varianzaArea<<","<<mediaDiametro<<","<<varianzaDiametro<< mediaInv1<<","<<varianzaInv2<<","<<mediaInv2<<","<<varianzaInv2<<","<<mediaInv3<<","<<varianzaInv3<<"\n";
        inv1.clear();
        inv2.clear();
        inv3.clear();
        area.clear();
        diametro.clear();
    }



    
    myfile.close();
}