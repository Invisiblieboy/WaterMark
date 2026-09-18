#include "BMPImage.h"
#include <iostream>

namespace itmo_bmp {

BMPImage::BMPImage() = default;
BMPImage::~BMPImage() {
    // TODO: освободить image, когда начнёшь его выделять
}
}

int main(){
    itmo_bmp::BMPImage img;

    char* file_name;
    std::cin >> file_name;
    std::cout << img.load(file_name);

    
    return 0;
}