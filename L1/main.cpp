#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "iostream"

using namespace std;
using namespace cv;

Mat frame, mask;
int slider_G, slider_R, slider_B;
int a,b;

Mat equalize(Mat img, int a, int b){
    Mat img_aux = img.clone();
    img_aux = a*img + b;
    //cout << "Matriz ecualizada" << endl;
    //cout << img << endl;
    // Create a window
    return img_aux;
}


Mat atomaticEqualize(Mat img){
    Mat img2;
    equalizeHist(img, img2);
    //Se crea la ventana

    //namedWindow("ecualizacion_automatica", WINDOW_NORMAL);
    //imshow("ecualizacion_automatica", img);
    //waitKey(0);

    //destroyAllWindows();  //opcional no se si ponerlo
    return img2;
}


/*
* getSkin: Funcion que detecta la piel
*/
Mat getSkin(Mat input){
    //Valores para detectar la piel
    int Y_MIN = 0;
    int Y_MAX = 255;
    int Cr_MIN = 133;
    int Cr_MAX = 173;
    int Cb_MIN = 77;
    int Cb_MAX = 127;

    Mat skin;
    //first convert our RGB image to YCrCb
    cvtColor(input,skin,COLOR_BGR2YCrCb);

    //uncomment the following line to see the image in YCrCb Color Space
    //imshow("YCrCb Color Space",skin);

    //filter the image in YCrCb color space
    inRange(skin,Scalar(Y_MIN,Cr_MIN,Cb_MIN),Scalar(Y_MAX,Cr_MAX,Cb_MAX),skin);

    return skin;
}

void on_trackbar_ALIEN(int, void*) {
   add(frame, Scalar(slider_B,slider_G,slider_R), frame, mask);
}



int main(int, char**) {
    
    // open the first webcam plugged in the computer
    VideoCapture camera(0);
    if (!camera.isOpened()) {
        cerr << "ERROR: Could not open camera" << endl;
        return 1;
    }
    while(1){
    //Mat imgAColor = frame.clone();
    //cvtColor( frame, frame, COLOR_BGR2GRAY );
    cout << "Que efecto quieres aplicar ?" << endl;
    cout << " 1: Contraste \n 2: Alien \n 3: Poster \n 4: Distorsion \n 5: terminar el uso de aplicación" << endl;
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
            // capture the next frame from the webcam
            camera >> frame;
            cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
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
        cv::destroyAllWindows();
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
            //frame.setTo(Scalar(0,255,0), mask);
            //add(frame, Scalar(0,255,0), frame, mask);
            on_trackbar_ALIEN(1,0);
            imshow("Alien",frame);
            if (waitKey(10) >= 0){
                break;
            }
        }
        cv::destroyAllWindows();
        //return 0;
   
    }else if (val == 3){
        cout << "Aplicando efecto de POSTER" << endl;
    }else if (val == 4) {
        cout << "Aplicando efecto de DISTORSION" << endl;
        //https://stackoverflow.com/questions/66895102/how-to-apply-distortion-on-an-image-using-opencv-or-any-other-library
    }else if (val == 5){
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






















// #include "opencv2/opencv.hpp"
// #include "opencv2/imgcodecs.hpp"
// #include "opencv2/highgui.hpp"
// #include "iostream"

// using namespace std;
// using namespace cv;

// // static void on_trackbar( int, void* )
// // {
// //    alpha = (double) alpha_slider/alpha_slider_max ;
// //    beta = ( 1.0 - alpha );
// //    addWeighted( src1, alpha, src2, beta, 0.0, dst);
// //    imshow( "Linear Blend", dst );
// // }

// void equalize(Mat img, float a, float b){
//     Mat img_aux = img.clone();
//     img = a*img + b;
//     cout << "Matriz ecualizada" << endl;
//     cout << img << endl;
//     // Create a window
//     namedWindow("ecualizacion_a_mano", 1);
//     imshow("ecualizacion_a_mano", img);
//     waitKey(0);
// }


// void atomaticEqualize(Mat img){
//     //Se ecualiza la imagen
//     equalizeHist(img, img);
//     //Se crea la ventana
//     namedWindow("ecualizacion_automatica", WINDOW_NORMAL);
//     imshow("ecualizacion_automatica", img);
//     waitKey(0);
//     //destroyAllWindows();  //opcional no se si ponerlo
     
// }


// //Deteccion de piel
// Mat getSkin(Mat input){
//     //Valores para detectar la piel
//     int Y_MIN;
//     int Y_MAX;
//     int Cr_MIN;
//     int Cr_MAX;
//     int Cb_MIN;
//     int Cb_MAX;
//     Y_MIN  = 0;
//     Y_MAX  = 255;
//     Cr_MIN = 133;
//     Cr_MAX = 173;
//     Cb_MIN = 77;
//     Cb_MAX = 127;

//     Mat skin;
//     //first convert our RGB image to YCrCb
//     cvtColor(input,skin,COLOR_BGR2YCrCb);

//     //uncomment the following line to see the image in YCrCb Color Space
//     //cv::imshow("YCrCb Color Space",skin);

//     //filter the image in YCrCb color space
//     inRange(skin,Scalar(Y_MIN,Cr_MIN,Cb_MIN),Scalar(Y_MAX,Cr_MAX,Cb_MAX),skin);
//     cout<<"skin es"<<skin<<endl;
//     return skin;

//     // //Convertir imagen en hsv
//     // //Si un pixel es 255 lo pongo verde
//     // for (int i=0; i<input.rows; i++) {
//     //     uchar* data= input.ptr<uchar>(i); // pointer to row i
//     //     uchar* dataMask= skin.ptr<uchar>(i);
//     //     for (int j=0; j<input.cols*input.channels(); j++) {
            
//     //         if(dataMask[j] == 255){
//     //             //data[j]
//     //         }
//     //     }
//     // }
//     Mat dst;
//     //cvtColor(input,input,COLOR_BGR2YCrCb);
//     add(input, Scalar(100, 0, 0), dst, skin);
//     namedWindow("alien", WINDOW_NORMAL);
//     imshow("alien", dst);
//     waitKey(0);
    

// }


// int main(int, char**) {
//     // open the first webcam plugged in the computer
//     VideoCapture camera(0);
//     if (!camera.isOpened()) {
//         cerr << "ERROR: Could not open camera" << endl;
//         return 1;
//     }

//     // this will contain the image from the webcam
//     Mat img;
        
//     // capture the next frame from the webcam
//     camera >> img;
//     Mat imgAColor = img.clone();
//     cvtColor( img, img, COLOR_BGR2GRAY );
//     cout << "Que efecto quieres aplicar ?" << endl;
//     cout << " 1: Contraste \n 2: Alien \n 3: Poster \n 4: Distorsion" << endl;
//     int val;
//     cin >> val;
    
//     if (val == 1){
//         cout << "Aplicando efecto de CONTRASTE" << endl;
//         //equalize(img,1.5,10);
//         //equalize(img,2,20);
//         //equalize(img,100,100);
//         atomaticEqualize(img);
//     }else if (val == 2) {
//         cout << "Aplicando efecto de ALIEN" << endl;
//         Mat alienImage = getSkin(imgAColor);
//         namedWindow("alien", WINDOW_NORMAL);
//         imshow("alien", alienImage);
//         waitKey(0);
//     }else if (val == 3){
//         cout << "Aplicando efecto de POSTER" << endl;
//     }else if (val == 4) {
//         cout << "Aplicando efecto de DISTORSION" << endl;
//     }
    
    
//     // display the frame until you press a key
//     //while (1) {
//     //    // show the image on the window
//     //    imshow("Webcam", img);
//     //    // wait (10ms) for a key to be pressed
//     //    if (waitKey(10) >= 0)
//     //        break;
//     //}
//     return 0;
// }

// /*
// //Alien es cargar los canales, cambias 2 y solo dejas uno
// Algo de piel: detectar la piel, buscar los rangos maximos de los sitios que tienen color que se asocia con piel. Elegir los pixeles con color piel
// Sustituir solo los pixeles que cumplen una propiedad
// Tenemos 3 canales, cojes el rojo, teniamos del 0 a 255 y entre esos dices pues quiero 8 categorias y  trasladas cada uno a esa propiedad
// Tengo un dato, si cae entre valor o el otro pues lo meto en tal conjunto, facil
// Esto para cada canal
// 8 categorias para cada canal (pueen ser mas)
// Distorion: la idea es que tenemos un conjunto de pixeles y en vez de desdistorsionarlos pues le añadimos distorsion, esto con una formula de las transparencias
// sacas el radio o algo asi. Con monica vimos lo de aplicar de forma inversa o normal, aqui vemos que info del pixel vamos a plasmar en la nueva imagen
// Que valor se le pone a k1, k2? pues escribir la formula y ver con las unidades con las que estamos trabajando
// Si esta posicion en la imagen anterior estaba desplazada, si ese r² va a ser300² entonces k1 tiene que ser muy pqueña
// */