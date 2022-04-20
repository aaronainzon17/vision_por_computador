#include "aprender.hpp"

void aprender(string nomfich, Moments& mu_x, Point2f& mc_x, double& area_x, double& diametro_x)
{
    Mat img = imread(nomfich);

    // Convert to graycsale
    Mat img_gray;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    // Blur the image for better edge detection
    Mat img_blur;
    GaussianBlur(img_gray, img_blur, Size(3,3), 0);
    Mat otsu = sacarOtsu(img_blur);
    Mat figure_bin  = sacarBlobs(otsu);
    //Eliminar los que sean menores que un determinado tamanyo
    vector<vector<Point>> contours;
    findContours(figure_bin, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    //Se sacan los momentos de los contornos
    vector<Moments> mu(contours.size() );
    double areasContornos[contours.size()];
    double diametroContornos[contours.size()];
    vector<Point2f> mc( contours.size() );
    //Se obtienen los descriptores de la imagen
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
    //Se calcula la media de los valores del vector
    double media=0;
    for(double valor : muestra){
        media += valor;
    }
    return media/(double)muestra.size();
}

double sacarVarianza(vector<double> muestra, double media){
    //Se calcula la varianza de los valores del vector
    double varianza=0;
    for(double valor : muestra){
        varianza += pow(valor - media,2);
    }
    varianza = varianza/((double)(muestra.size()-1));
    //Se prueban dintos valores de la media a priori ya que hay pocas muestras con las que sacar la varianza
    //Se ha decidido que la varianza a priori sea el cuadrado del porcentaje de la media (Se han probado varios valores de porcentaje)

    //varianza = varianza/((double)(muestra.size()));
    //varianza = varianza + pow(media*0.01,2)/((double)(muestra.size()));
    //varianza = varianza + pow(media*0.05,2)/((double)(muestra.size()));
    //varianza = varianza + pow(media*0.06,2)/((double)(muestra.size()));
    //varianza = varianza + pow(media*0.07,2)/((double)(muestra.size()));
    //varianza = varianza + pow(media*0.08,2)/((double)(muestra.size()));
    //varianza = varianza + pow(media*0.11,2)/((double)(muestra.size()));
    return varianza;
    
}

void aprenderTodo(){
    ofstream myfile;
    myfile.open ("objetos");
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
    ///Para cada figura se obtienen sus descriptores
    for(string s : figuras){
        myfile << s<<"\n";
        //De cada uno de los ficheros de entrenamiento se extraen los descriptores del fichero
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
        //Una vez obtenidos los descriptores de los ficheros de entrenamiento se obtiene la media y varianza de cada descriptor y se escribe en un fichero 
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
        myfile<<mediaArea<<","<<varianzaArea<<","<<mediaDiametro<<","<<varianzaDiametro<<","<< mediaInv1<<","<<varianzaInv1<<","<<mediaInv2<<","<<varianzaInv2<<","<<mediaInv3<<","<<varianzaInv3<<"\n";
        inv1.clear();
        inv2.clear();
        inv3.clear();
        area.clear();
        diametro.clear();
    }
    myfile.close();
}