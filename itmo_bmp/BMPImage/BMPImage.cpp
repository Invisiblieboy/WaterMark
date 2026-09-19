#include "BMPImage.h"

namespace itmo_bmp {
BMPImage::BMPImage() : width_(-1), height_(-1), image_(nullptr) {}
BMPImage::~BMPImage() {
    if (image_ != nullptr){
        delete image_;
    }
}

status_code BMPImage::load(const char* file_name) {
    return kOK;  // заглушка
}

Pixel* BMPImage::memory_preparation_(const uint32_t len) {
    Pixel* ptr = new Pixel[len];
    return ptr;
}

Pixel* BMPImage::get_pixel_color(const uint32_t x, const uint32_t y) {
    return image_ + (y * width_ + x);
}

status_code BMPImage::gen_empty_sheet(const uint32_t width, const uint32_t height, const uint8_t color_depth){
    if (width==0 || height == 0){
        return status_code::kInvalidArg;
    }
    image_ = memory_preparation_(width * height);

    width_ = width;
    height_ = height;
    color_depth_ = color_depth;
    return status_code::kOK;
}

status_code BMPImage::set_pixel_color(uint32_t x, uint32_t y, const Pixel* pixel){
    if (x >= width_ || y >= height_){
        return status_code::kInvalidArg;
    }
    *(image_ + (y * width_ + x)) = *pixel;

    return status_code::kOK;
}
} // namespace itmo_bmp
