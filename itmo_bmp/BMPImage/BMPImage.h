#pragma once
#include <cstdint>

namespace itmo_bmp{
    class Pixel {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    enum status_code{
        kOK, kFileNotFound, kUnknownError
    };

    class BMPImage {
    public:
        BMPImage();
        ~BMPImage();

        // Публичное API класса
        status_code load(char* file_name);
        status_code save(char* file_name);

        // Точечная работа с пикселями
        Pixel get_pixel_color(int x, int y);
        status_code set_pixel_color(int x, int y, Pixel* pixel);
    
    private:
        Pixel** image;
    };
};