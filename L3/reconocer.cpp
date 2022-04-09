#include "reconocer.hpp"

void reconocer(string nomfich){

    //Se lee la imagen a reconocer
    Mat img = imread(nomfich);
    // Convert to graycsale
    Mat img_gray;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    // Blur the image for better edge detection
    Mat img_blur;
    GaussianBlur(img_gray, img_blur, Size(3,3), 0);

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
    
    float mahalanobis;
    for(size_t i = 0; i < contours.size();i++){ // Se recorren los objetos de la imagen 
        mahalanobis = 0;
        if(areasContornos[i] >= 1000){  //Se descartan los blobs de tamaño menor que 1000 (indeseados)
            //Leer el fichero de aprender
            //Se calcula la distancia de mahalanobis.
            /*for(int i = 0; i < parametros.rows; i++) {
                mahalanobis += (pow(parametros[i]-media[i],2))/varianza[i];
            }*/
        }

    }
}

void leerDatosAprendizaje(string nomfich, vector<vector<float>> &datosAprendidos){
    ifstream f;
    string linea;
    f.open(nomfich);
    if(!f) {
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
    
    vector<float> dataAux;
    while (!f.eof()){
        getline(f, linea);
        cout << linea << endl;
        for(int j = 0; j < 9; j++){
            getline(f, linea, ',');
            cout << linea << endl;
            dataAux.push_back(stof(linea));
        }
        getline(f, linea);
        cout << linea << endl;
        dataAux.push_back(stof(linea));
        datosAprendidos.push_back(dataAux);
        dataAux.clear();
    }
    
    f.close();
}