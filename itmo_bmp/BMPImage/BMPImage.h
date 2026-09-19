#pragma once
#include <cstdint>

namespace itmo_bmp{
    class Pixel {
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
    };

    enum status_code{
        kOK, kFileNotFound, kPermissionDenied, kInvalidArg, kUnknownError
    };

    class BMPImage {
    public:
        BMPImage();
        ~BMPImage();

        // Публичное API класса
        status_code load(const char* file_name);
        status_code save(const char* file_name);

        status_code gen_empty_sheet(const uint32_t width, const uint32_t height, const uint8_t color_depth);

        // Точечная работа с пикселями
        Pixel* get_pixel_color(const uint32_t x, const uint32_t y);
        status_code set_pixel_color(const uint32_t x, const uint32_t y, const Pixel* pixel);

    
    private:
        uint32_t width_;
        uint32_t height_;
        uint8_t color_depth_;
        Pixel* image_ ;

        Pixel* memory_preparation_(const uint32_t len);
    };
} // namespace itmo_bmp