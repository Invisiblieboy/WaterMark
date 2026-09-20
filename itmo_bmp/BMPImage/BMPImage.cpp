#include <fstream>
#include <vector>
#include <iostream>

#include "BMPImage.h"
#include "BMPHeaders.h"

namespace itmo_bmp {
BMPImage::BMPImage() : width_(0), height_(0), image_(nullptr) {}
BMPImage::~BMPImage() {
    if (image_ != nullptr){
        delete[] image_;
    }
}

status_code BMPImage::load(const char* file_name) {
    std::ifstream file(file_name, std::ios::binary);

    if (!file.is_open()) {
        return status_code::kUnknownError;
    }
    
    BMPFileHeader file_header;
    BMPInfoHeader info_header;

    // Чтение заголовков
    file.read(reinterpret_cast<char*>(&file_header), sizeof(file_header));
    file.read(reinterpret_cast<char*>(&info_header), sizeof(info_header));
    
    // Проверка файла
    if (!file) {
        return status_code::kEmptyFile;
    }
    if (file_header.type != 0x4D42) {
        return kUnknownError;           // не BMP
    }
    
    // Переходим к пикселям по offBits
    file.seekg(file_header.offBits, std::ios::beg);

    gen_empty_sheet(info_header.width, info_header.height, info_header.bit_count);
    uint32_t row_size = (( info_header.bit_count * width_ + 31) / 32) * 4;
    std::vector<uint8_t> row(row_size);

    for (int32_t i = 0; i < height_; ++i) {
        file.read(reinterpret_cast<char*>(row.data()), row_size);
        
        int32_t y = height_ - 1 - i;
        // TODO Сделать обработку нестандартных значений (54)
        for (int32_t x = 0; x < width_; x++) {
            Pixel pxl;
            if( color_depth_ == 8){
                pxl.b=row[x];
                pxl.g=row[x];
                pxl.r=row[x];
            } else {
                pxl.b=row[3 * x];
                pxl.g=row[3 * x + 1];
                pxl.r=row[3 * x + 2];
            }
            set_pixel_color(x, y, pxl);
        }
    }
    std::cout << (int)((*get_pixel_color(0,0)).r);
    file.close();
    return status_code::kOK;
}

Pixel* BMPImage::memory_preparation_(const uint32_t len) {
    if (image_ != nullptr){
        delete[] image_;
    }
    Pixel* image_ = new Pixel[len];
    return image_;
}

Pixel* BMPImage::get_pixel_color(const uint32_t x, const uint32_t y) {
    return image_ + (y * width_ + x);
}

status_code BMPImage::gen_empty_sheet(const int32_t width, const int32_t height, const uint16_t color_depth){
    int sw = width < 0 ? -1 : 1 ;
    int sh = height < 0 ? -1 : 1 ;
    image_ = memory_preparation_(width * height * sw * sh);

    width_ = width * sw;
    height_ = height * sh;
    color_depth_ = color_depth;
    return status_code::kOK;
}

status_code BMPImage::set_pixel_color(uint32_t x, uint32_t y, const Pixel pixel){
    if (x >= width_ || y >= height_){
        return status_code::kInvalidArg;
    }
    *(image_ + (y * width_ + x)) = pixel;

    return status_code::kOK;
}
} // namespace itmo_bmp


int main(){
    itmo_bmp::BMPImage img;

    img.load("images/lena.bmp");
    return 0;
}