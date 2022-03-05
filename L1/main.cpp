#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include "iostream"
#include <math.h>

using namespace std;
using namespace cv;

Mat frame, mask, cartoon;
int slider_G, slider_R, slider_B;
int a,b,sigmaX;
int cambioColor;
int barril,almohada;

/*
* equalize: Funcion que ecualiza la imagen
    a = contraste
    b = brillo
*/
Mat equalize(Mat img, int a, int b){
    
    Mat img_aux = img.clone();
    //Se transforman los valores de intensidad de la imagen con los valores suministrados por el usuario
    img_aux = a*img + b;
    // Create a window
    return img_aux;
}

/*
* atomaticEcualize: ecualiza la imagen con los mejores valores de brillo y contraste
*/
Mat atomaticEqualize(Mat img){
    Mat img2;
    //Se llama a la funcion de openCV que ecualiza el histograma acumulado con las mejores a y b
    equalizeHist(img, img2);
    return img2;
}


/*
* getSkin: Funcion que detecta la piel
*/
Mat getSkin(Mat input){
    //Valores para detectar la piel
    int Y_MIN = 80; 
    int Y_MAX = 255;
    int Cr_MIN = 135; 
    int Cr_MAX = 173; 
    int Cb_MIN = 80; 
    int Cb_MAX = 135;

    //Se declara la matriz para la mascara
    Mat skin;

    //Se convierte el frame BGR a YCrCb
    cvtColor(input,skin,COLOR_BGR2YCrCb);

    //Se crea la mascara utilizando la funcion in range
    inRange(skin,Scalar(Y_MIN,Cr_MIN,Cb_MIN),Scalar(Y_MAX,Cr_MAX,Cb_MAX),skin);

    //Se devuelve la mascara
    return skin;
}

// on_trackbar_ALIEN: Funcion encargada de responder al movimiento del trackbar 
// en el efecto de Alien 
void on_trackbar_ALIEN(int, void*) {
   add(frame, Scalar(slider_B,slider_G,slider_R), frame, mask);
}

/*
    reducing_color reduce los colores de la imagen
*/
void reducing_Color(Mat &image, int div=64){ 
   int   total_rows = image.rows;
   int total_columns = image.cols * image.channels();
   for (int j = 0; j < total_rows; j++){
      uchar* data = image.ptr<uchar>(j);
      for (int i = 0; i < total_columns; i++){ 
          //Para cada pixel se divide entre un valor (entre 1 y 255) y se multiplica por el
          //Al ser enteros colores similares acabaran con el mismo valor
          //Se vuelve a multiplicar por div tras dividirlo para recuperar el valor original (verde oscuro y verde claro acabaran con el mismo color)
          //sino un color acabaría en otra gama de colores
         data[i] = data[i] / div * div + div / 2;
      }
   }  
}

/*
    distorsionBarril distorsiona la imagen con el efecto de barril
*/
Mat distorsionBarril(Mat src){
   
    //Se declara la matriz destino
    Mat dst;
    
    //Se crea y se rellena con 0
    dst.create(src.rows,src.cols,CV_8UC(3));
    dst = Scalar::all(0);
    
    // Variables para el cambio de coordenadas
    double xcen = src.cols/2;
    double ycen = src.rows/2;
    
    int r2,r4;
    int xu,yu;

    //Se define el factor de distorsion
    double K1 = barril * pow(10,-6);
    double K2 = 0;
    
    //Se itera por filas y columnas
    for (int x = 0; x < src.cols; x++) {
        for (int y = 0; y < src.rows; y++) {
            //Se obtiene la distancia al cuadrado al cuadrado del píxel a distorsionar al centro
            r2 = pow((x - xcen),2) + pow((y - ycen),2);
            r4 = pow(r2,2);
            //Se el pixel correspondiente de la imagen original
            xu = x + (x - xcen)* K1 * r2 + (x - xcen)* K2 * r4;
            yu = y + (y - ycen)* K1 * r2 + (y - ycen)* K2 * r4;
            
            //Si la nueva coordenada no excede los limites de la imagen, 
            //se asigna el pixel a la matriz destino (dst)
            if(xu < src.cols && yu < src.rows && xu > 0 && yu > 0){
                dst.at<Vec3b>(y,x)[0] = src.at<Vec3b>(yu,xu)[0];
                dst.at<Vec3b>(y,x)[1] = src.at<Vec3b>(yu,xu)[1];
                dst.at<Vec3b>(y,x)[2] = src.at<Vec3b>(yu,xu)[2];
            }
        }
    }

    return dst;
}

/*
    distorsionBarril distorsiona la imagen con el efecto de almohada
*/
Mat distorsionAlmohada(Mat src){
   
    //Mat dst = Mat::zeros(src.rows,src.cols,CV_64FC1);
    Mat dst(src.rows,src.cols,CV_8UC1,Scalar(1,3));
    dst.create(src.rows,src.cols,CV_8UC(3));
    int xcen = src.cols/2;
    int ycen = src.rows/2;
    int xd,yd;
    int r2,r4;
    int xu,yu;
    //double K1 =1.0e-6;
    //Se define el factor de distorsion
    double K1 = -almohada * pow(10,-6);
    double K2 = 0;
    //cout<<"Hola1"<<endl;
    for (int i=0; i<src.rows; i++) {
        for (int j=0; j<src.cols; j++) { //int j=0; j<src.cols*src.channels(); j++
            xd = j;
            yd = i;
            r2 = (xd - xcen)*(xd - xcen) + (yd - ycen)*(yd - ycen);
            r4 =r2*r2;
            xu = xd + (xd - xcen )* K1 * r2 + (xd - xcen )* K2 * r4;
            yu = yd + (yd - ycen )* K1 * r2 + (yd - ycen )* K2 * r4;
            if(xu >= src.cols || yu >= src.rows){
                dst.at<Vec3b>(yd,xd)[0] = 0;
                dst.at<Vec3b>(yd,xd)[1] = 0;
                dst.at<Vec3b>(yd,xd)[2] = 0;
            }else{
                dst.at<Vec3b>(yd,xd)[0] = src.at<Vec3b>(yu,xu)[0];
                dst.at<Vec3b>(yd,xd)[1] = src.at<Vec3b>(yu,xu)[1];
                dst.at<Vec3b>(yd,xd)[2] = src.at<Vec3b>(yu,xu)[2];
            }
        }
    }

    return dst;
}

void cartoonize(){
    //Convertir la imagen a escala de grises
    Mat grayImage;
    cvtColor(frame, grayImage, COLOR_BGR2GRAY);

    //Se le aplica ligero desenfoque
    GaussianBlur(grayImage, grayImage, Size(3, 3), (double)sigmaX/100);

    //Deteccion de bordes 
    Mat edgeImage;
    Laplacian(grayImage, edgeImage, -1, 5);
    convertScaleAbs(edgeImage, edgeImage);
    
    //Invertir colores 
    edgeImage = 255 - edgeImage;

    //Eliminacion de ruido
    threshold(edgeImage, edgeImage, 150, 255, THRESH_BINARY);
    
    //Desendoque de la imagen manteniendo los contornos
    Mat edgePreservingImage;
    edgePreservingFilter(frame, edgePreservingImage, 2, 50, 0.4);
    
    cartoon = Scalar::all(0);

    // Combiancion de imagen para dibujo final
    bitwise_and(edgePreservingImage, edgePreservingImage, cartoon, edgeImage);
}

int main(int, char**) {
    
    //Abrir la camara de video situada en el puerto 0
    VideoCapture camera(0);
    if (!camera.isOpened()) {
        cerr << "ERROR: Could not open camera" << endl;
        return 1;
    }
    while(1){

        cout << "Que efecto quieres aplicar ?" << endl;
        cout << " 1: Contraste \n 2: Alien \n 3: Poster \n 4: Distorsion \n 5: Dibujo \n 6: terminar el uso de aplicación" << endl;
        int val;
        cin >> val;
        
        if (val == 1){
            cout << "Aplicando efecto de CONTRASTE" << endl;
            namedWindow("Ecualizacion ajustada", 1);
            a=1;
            b=0;
            createTrackbar("Contraste","Ecualizacion ajustada",&a,100);
            createTrackbar("Brillo","Ecualizacion ajustada",&b,100);
            while(1) {
                //Capturar frame 
                camera >> frame;
 
                cvtColor(frame, frame, COLOR_BGR2GRAY);
                imshow("Original Image",frame);
                Mat imagenEcualizada = atomaticEqualize(frame);
                imshow("Imagen ecualizada",imagenEcualizada);

                //Se muestra la que puedes variar parametros
                Mat imagenEcualizada_parametros = equalize(frame,a,b);
                imshow("Ecualizacion ajustada",imagenEcualizada_parametros);
                if (waitKey(10) >= 0){
                    break;
                }
            }
            destroyAllWindows();
        }else if (val == 2) {
            cout << "Aplicando efecto de ALIEN" << endl;
            namedWindow("Alien", WINDOW_AUTOSIZE); // Create Window
        
            slider_B = 0;
            slider_G = 255;
            slider_R = 0;

            createTrackbar( "Blue", "Alien", &slider_B, 255, on_trackbar_ALIEN );
            createTrackbar( "Green", "Alien", &slider_G, 255, on_trackbar_ALIEN );
            createTrackbar( "Red", "Alien", &slider_R, 255, on_trackbar_ALIEN );

            while(1) {
                // capture the next frame from the webcam
                camera >> frame;

                //show the current image
                imshow("Original Image",frame);

                mask = getSkin(frame);
                
                on_trackbar_ALIEN(1,0);

                imshow("Alien",frame);
                if (waitKey(10) >= 0){
                    break;
                }
            }
            destroyAllWindows();
    
        }else if (val == 3){
            cout << "Aplicando efecto de POSTER" << endl;
            namedWindow("Poster", WINDOW_AUTOSIZE); // Create Window
            cambioColor = 64;
            createTrackbar("Escala","Poster",&cambioColor,256);
            while(1) {
                // capture the next frame from the webcam
                camera >> frame;

                //show the current image
                imshow("Original Image",frame);
                if(cambioColor == 0){
                    cambioColor +=1;
                }
                reducing_Color(frame,cambioColor);
                
                
                imshow("Poster",frame);
                if (waitKey(10) >= 0){
                    break;
                }
            }
            destroyAllWindows();

        }else if (val == 4) {
            cout << "Aplicando efecto de DISTORSION" << endl;

            namedWindow("Efecto barril", WINDOW_AUTOSIZE);
            namedWindow("Efecto almohada", WINDOW_AUTOSIZE);
            
            createTrackbar("Escala","Efecto barril",&barril,10);
            createTrackbar("Escala","Efecto almohada",&almohada,10);

            while(1) {
                // capture the next frame from the webcam
                camera >> frame;
                
                imshow("Original Image",frame);

                //Se muestra la que puedes variar parametros
                Mat imagenBarril = distorsionBarril(frame);
                Mat imagenAlmohada = distorsionAlmohada(frame);
                imshow("Efecto barril",imagenBarril);
                imshow("Efecto almohada",imagenAlmohada);
                if (waitKey(10) >= 0){
                    break;
                }
            }
            destroyAllWindows();
        }else if (val == 5){
            cout << "Aplicando efecto de CARTOON" << endl;
            namedWindow("Cartoon", WINDOW_AUTOSIZE); // Create Window
            createTrackbar( "Blur Effect", "Cartoon", &sigmaX, 100);

            while(1) {
                // capture the next frame from the webcam
                camera >> frame;

                //show the current image
                imshow("Original Image",frame);

                cartoonize();

                imshow("Cartoon",cartoon);
                if (waitKey(10) >= 0){
                    break;
                }
            }
            
            destroyAllWindows();
        }else if (val == 6){
            cout << "Dejando de usar la aplicacion" << endl;
            return 0;
        }
    }
    
   
}

/*
//Alien es cargar los canales, cambias 2 y solo dejas uno
Algo de piel: detectar la piel, buscar los rangos maximos de los sitios que tienen color que se asocia con piel. Elegir los pixeles con color piel
Sustituir solo los pixeles que cumplen una propiedad
Tenemos 3 canales, cojes el rojo, teniamos del 0 a 255 y entre esos dices pues quiero 8 categorias y  trasladas cada uno a esa propiedad
Tengo un dato, si cae entre valor o el otro pues lo meto en tal conjunto, facil
Esto para cada canal
8 categorias para cada canal (pueen ser mas)
Distorion: la idea es que tenemos un conjunto de pixeles y en vez de desdistorsionarlos pues le añadimos distorsion, esto con una formula de las transparencias
sacas el radio o algo asi. Con monica vimos lo de aplicar de forma inversa o normal, aqui vemos que info del pixel vamos a plasmar en la nueva imagen
Que valor se le pone a k1, k2? pues escribir la formula y ver con las unidades con las que estamos trabajando
Si esta posicion en la imagen anterior estaba desplazada, si ese r² va a ser300² entonces k1 tiene que ser muy pqueña
*/