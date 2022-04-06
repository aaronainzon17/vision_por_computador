#include "descriptores.hpp"

Mat sacarOtsu(Mat img_blur){
    Mat otsu;
    threshold(img_blur,otsu,0,255, THRESH_BINARY_INV | THRESH_OTSU);
    imshow("Otsu img", otsu);
    waitKey(0);

    return otsu;
}



int sacarBlobs(){

return 0;
}

vector<vector<Point>> sacarContornos(){

}

void sacarDescriptores(){

}