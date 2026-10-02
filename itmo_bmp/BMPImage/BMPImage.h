#pragma once
#include <cstdint>
#include <vector>

namespace itmo_bmp{
    struct Pixel {
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
    };

    enum status_code{
        kOK, kFileNotFound, kPermissionDenied, kInvalidArg, kEmptyFile, kInvalidFile, kUnknownError, 
    };

    class BMPImage {
    public:
        BMPImage();
        ~BMPImage();

        // Публичное API класса
        status_code load(const char* file_name);
        status_code save(const char* file_name);

        status_code gen_empty_sheet(const int32_t width, const int32_t height, const uint16_t color_depth);

        // Точечная работа с пикселями
        status_code get_pixel_color(const uint32_t x, const uint32_t y, Pixel* pixel);
        status_code set_pixel_color(const uint32_t x, const uint32_t y, const Pixel pixel);

    
    private:
        uint32_t width_ = 0;
        uint32_t height_ = 0;
        uint8_t color_depth_ = 0;
        std::vector<Pixel> image_ ;
    };
} // namespace itmo_bmp