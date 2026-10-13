#ifndef WATERMARK_H
#define WATERMARK_H
#include "../BMPImage/BMPImage.h"

namespace itmo_bmp {

bool apply_watermark(BMPImage& main, BMPImage& watermark, const char *output_path);

} // namespace itmo_bmp

#endif // WATERMARK_H
