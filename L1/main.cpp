#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "iostream"

using namespace std;
using namespace cv;

// static void on_trackbar( int, void* )
// {
//    alpha = (double) alpha_slider/alpha_slider_max ;
//    beta = ( 1.0 - alpha );
//    addWeighted( src1, alpha, src2, beta, 0.0, dst);
//    imshow( "Linear Blend", dst );
// }

void equalize(Mat img, float a, float b){
    Mat img_aux = img.clone();
    img = a*img + b;
    cout << "Matriz ecualizada" << endl;
    cout << img << endl;
    // Create a window
    namedWindow("ecualizacion_a_mano", 1);
    imshow("ecualizacion_a_mano", img);
    waitKey(0);
}


void atomaticEqualize(Mat img){
    //Se ecualiza la imagen
    equalizeHist(img, img);
    //Se crea la ventana
    namedWindow("ecualizacion_automatica", WINDOW_NORMAL);
    imshow("ecualizacion_automatica", img);
    waitKey(0);
    //destroyAllWindows();  //opcional no se si ponerlo
     
}


//Deteccion de piel
Mat getSkin(Mat input){
    //Valores para detectar la piel
    int Y_MIN;
    int Y_MAX;
    int Cr_MIN;
    int Cr_MAX;
    int Cb_MIN;
    int Cb_MAX;
    Y_MIN  = 0;
    Y_MAX  = 255;
    Cr_MIN = 133;
    Cr_MAX = 173;
    Cb_MIN = 77;
    Cb_MAX = 127;

    Mat skin;
    //first convert our RGB image to YCrCb
    cvtColor(input,skin,COLOR_BGR2YCrCb);

    //uncomment the following line to see the image in YCrCb Color Space
    //cv::imshow("YCrCb Color Space",skin);

    //filter the image in YCrCb color space
    inRange(skin,Scalar(Y_MIN,Cr_MIN,Cb_MIN),Scalar(Y_MAX,Cr_MAX,Cb_MAX),skin);

    return skin;
}


int main(int, char**) {
    // open the first webcam plugged in the computer
    VideoCapture camera(0);
    if (!camera.isOpened()) {
        cerr << "ERROR: Could not open camera" << endl;
        return 1;
    }

    // this will contain the image from the webcam
    Mat img;
        
    // capture the next frame from the webcam
    camera >> img;
    Mat imgAColor = img.clone();
    cvtColor( img, img, COLOR_BGR2GRAY );
    cout << "Que efecto quieres aplicar ?" << endl;
    cout << " 1: Contraste \n 2: Alien \n 3: Poster \n 4: Distorsion" << endl;
    int val;
    cin >> val;
    
    if (val == 1){
        cout << "Aplicando efecto de CONTRASTE" << endl;
        //equalize(img,1.5,10);
        //equalize(img,2,20);
        //equalize(img,100,100);
        atomaticEqualize(img);
    }else if (val == 2) {
        cout << "Aplicando efecto de ALIEN" << endl;
        Mat alienImage = getSkin(imgAColor);
        namedWindow("alien", WINDOW_NORMAL);
        imshow("alien", alienImage);
        waitKey(0);
    }else if (val == 3){
        cout << "Aplicando efecto de POSTER" << endl;
    }else if (val == 4) {
        cout << "Aplicando efecto de DISTORSION" << endl;
    }
    
    
    // display the frame until you press a key
    //while (1) {
    //    // show the image on the window
    //    imshow("Webcam", img);
    //    // wait (10ms) for a key to be pressed
    //    if (waitKey(10) >= 0)
    //        break;
    //}
    return 0;
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