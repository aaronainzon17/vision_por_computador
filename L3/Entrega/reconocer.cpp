#include "reconocer.hpp"

void reconocer(string nomfich){
    cout<<"Analizando imagen "<<nomfich<<endl;			
    const double valChi = 11.0705;  //Se han usado 5 grados de libettad con una probabilidad del 5%
    vector<vector<float>> datosAprendidos;
    leerDatosAprendizaje("objetos", datosAprendidos);
    aprenderTodo();
    //Se lee la imagen a reconocer
    Mat img = imread(nomfich);
    imshow("Imagen original",img);
    // Convert to graycsale
    Mat img_gray;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    // Blur the image for better edge detection
    Mat img_blur;
    GaussianBlur(img_gray, img_blur, Size(3,3), 0);
    //Se segmenta la imagen y se obtienen los blobs
    Mat otsu = sacarOtsu(img_blur);
    Mat figure_bin  = sacarBlobs(otsu);
    
    vector<vector<Point>> contours;
    //Se extraen los contornos de la imagen
    findContours(figure_bin, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    
    //Se sacan los momentos de los contornos
    vector<Moments> mu(contours.size() );
    double areasContornos[contours.size()];
    double diametroContornos[contours.size()];
    vector<Point2f> mc( contours.size() );
    vector<vector<Point>> contornosCirculo , contornosRectangulo, contornosRueda, contornosTriangulo, contornosVagon;   //Se guardan los cortonos que pertenecen a cada clase
    //vector<vector<Point>> contornoClases;
    double huMoments[7];
    Moments momento_x;
    double inv1,inv2,inv3,area_x,diametro_x;
    sacarDescriptores(contours, figure_bin, rng ,mu , mc, areasContornos, diametroContornos);
    double mahalanobis;
    vector<float> datos;
    for(size_t i = 0; i < contours.size();i++){ // Se recorren los objetos de la imagen 
        mahalanobis = 0;
        if(areasContornos[i] >= 1000){  //Se descartan los blobs de tamaño menor que 1000 (indeseados)
            //Se extraen los descriptores de cada contorno
            momento_x = mu.at(i);
            HuMoments(momento_x,huMoments);
            area_x = areasContornos[i];
            diametro_x = diametroContornos[i];
            inv1 = huMoments[0];
            inv2 = huMoments[1];
            inv3 = huMoments[2];
            //Se obtiene la distancia de mahalanobis del contorno respecto de las figuras aprendidas
            for(size_t j=0; j<datosAprendidos.size();j++){
                datos = datosAprendidos.at(j);
                mahalanobis = pow(area_x - datos.at(0),2)/datos.at(1) + pow(diametro_x - datos.at(2),2)/datos.at(3) +
                                pow(inv1 - datos.at(4),2)/datos.at(5) + pow(inv2 - datos.at(6),2)/datos.at(7) +
                                pow(inv3 - datos.at(8),2)/datos.at(9);
                mahalanobis = sqrt(mahalanobis);
                //Si el valor de mahalanobis es menor que el valor de la distribucion Chi-cuadrado entonces se ha reconocido un objeto
                if(mahalanobis <= valChi){  //Se ha detectado el objeto
                    //Se almacena el contorno es un vector que representa a la clase de la figura (asi se puede mostrar despues)
                    if(j==0){   //Se detecta circulo
                        contornosCirculo.push_back(contours.at(i));
                    }
                    if(j==1){   //rectangulo
                        contornosRectangulo.push_back(contours.at(i));
                    }
                    if(j==2){   //reuda
                        contornosRueda.push_back(contours.at(i));
                    }  
                    if(j==3){   //triangulo
                        contornosTriangulo.push_back(contours.at(i));
                    }
                    if(j==4){   //vagon
                        contornosVagon.push_back(contours.at(i));
                    }
                }
            }
            //cout<<""<<endl;
        }

    }
    string titulo;
    //Para cada clase se comprueba los contornos que contiene y se muestra por pantalla
    if(!contornosCirculo.empty()){
        //Mostrar los circulos
        if(contornosCirculo.size()==1){
            titulo = "Circulo detectado";
            cout<<titulo<<endl;
        }else{
            titulo = "Tengo dudas con el circulo";
            cout<<titulo<<endl;
        }
        Mat drawing = Mat::zeros( figure_bin.size(), CV_8UC3 );
        for( size_t i = 0; i< contornosCirculo.size(); i++ )
        {
            Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
            drawContours( drawing, contornosCirculo, (int)i, color, 2, LINE_8, RETR_TREE, 0 );
        }
        imshow( titulo, drawing );
        //waitKey(0);
    }else{
        cout<<"No se ha detectado ningun circulo"<<endl;
    }
    if(!contornosRectangulo.empty()){
        //Mostrar los circulos
        if(contornosRectangulo.size()==1){
            titulo = "Rectangulo detectado";
            cout<<titulo<<endl;
        }else{
            titulo = "Tengo dudas con el rectangulo";
            cout<<titulo<<endl;
        }
        Mat drawing = Mat::zeros( figure_bin.size(), CV_8UC3 );
        for( size_t i = 0; i< contornosRectangulo.size(); i++ )
        {
            Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
            drawContours( drawing, contornosRectangulo, (int)i, color, 2, LINE_8, RETR_TREE, 0 );
        }
        imshow( titulo, drawing );
        //waitKey(0);
    }else{
        cout<<"No se ha detectado ningun rectangulo"<<endl;
    }
    if(!contornosRueda.empty()){
        //Mostrar los circulos
        if(contornosRueda.size()==1){
            titulo = "Rueda detectada";
            cout<<titulo<<endl;
        }else{
            titulo = "Tengo dudas con la rueda";
            cout<<titulo<<endl;
        }
        Mat drawing = Mat::zeros( figure_bin.size(), CV_8UC3 );
        for( size_t i = 0; i< contornosRueda.size(); i++ )
        {
            Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
            drawContours( drawing, contornosRueda, (int)i, color, 2, LINE_8, RETR_TREE, 0 );
        }
        imshow( titulo, drawing );
        //waitKey(0);

    }else{
        cout<<"No se ha detectado ninguna rueda"<<endl;
    }
    if(!contornosTriangulo.empty()){
        //Mostrar los circulos
        if(contornosTriangulo.size()==1){
            titulo = "Triangulo detectado";
            cout<<titulo<<endl;
        }else{
            titulo = "Tengo dudas con el triangulo";
            cout<<titulo<<endl;
        }
        Mat drawing = Mat::zeros( figure_bin.size(), CV_8UC3 );
        for( size_t i = 0; i< contornosTriangulo.size(); i++ )
        {
            Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
            drawContours( drawing, contornosTriangulo, (int)i, color, 2, LINE_8, RETR_TREE, 0 );
        }
        imshow( titulo, drawing );
        //waitKey(0);

    }else{
        cout<<"No se ha detectado ningun triangulo"<<endl;
    }
    if(!contornosVagon.empty()){
        //Mostrar los circulos
        if(contornosVagon.size()==1){
            titulo = "Vagon detectado";
            cout<<titulo<<endl;
        }else{
            titulo = "Tengo dudas con el vagon";
            cout<<titulo<<endl;
        }
        Mat drawing = Mat::zeros( figure_bin.size(), CV_8UC3 );
        for( size_t i = 0; i< contornosVagon.size(); i++ )
        {
            Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
            drawContours( drawing, contornosVagon, (int)i, color, 2, LINE_8, RETR_TREE, 0 );
        }
        imshow( titulo, drawing );
        //waitKey(0);

    }else{
        cout<<"No se ha detectado ningun vagon"<<endl;
    }
    waitKey(0);
    cout<<endl;
    destroyAllWindows();
    
}

void leerDatosAprendizaje(string nomfich, vector<vector<float>> &datosAprendidos){
    ifstream f;
    string linea;
    f.open(nomfich);
    if(!f) {
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
    //Se extraen los valores del fichero y se almacenan en un vector
    vector<float> dataAux;
    while (!f.eof()){
        getline(f, linea);
        if(linea != ""){
            for(int j = 0; j < 9; j++){
                getline(f, linea, ',');
                dataAux.push_back(stof(linea));
            }
            getline(f, linea);
            dataAux.push_back(stof(linea));
            datosAprendidos.push_back(dataAux);
            dataAux.clear();
        }
    }
    
    f.close();
}
