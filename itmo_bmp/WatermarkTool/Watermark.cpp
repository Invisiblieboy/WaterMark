#include "Watermark.h"
#include <cstdint>
#include "Logger.h"
namespace itmo_bmp {

Pixel calc_merge_color(Pixel a, Pixel b) {
  return {.r = (uint8_t)((a.r + b.r) / 2),
          .g = (uint8_t)((a.g + b.g) / 2),
          .b = (uint8_t)((a.b + b.b) / 2)};
}

bool apply_watermark(BMPImage &base, BMPImage &watermark,
                     const char *output_path) {
  const uint32_t w_base = base.get_width();
  const uint32_t h_base = base.get_height();
  const uint32_t w_water = watermark.get_width();
  const uint32_t h_water = watermark.get_height();

  if (w_base == 0 || h_base == 0 || w_water == 0 || h_water == 0) {
    Logger::Error("Пустое изображение (основное или водяной знак)");
    return false;
  }

  if (w_water > w_base || h_water > h_base) {
    Logger::Error("Водяной знак больше основного изображения");
    return false;
  }

  BMPImage res;
  res.gen_empty_sheet(w_base, h_base);

  Pixel a, b;
  for (uint32_t i = 0; i < h_base; ++i) {
    for (uint32_t j = 0; j < w_base; ++j) {
      base.get_pixel_color(j, i, &a);
      watermark.get_pixel_color(j % w_water, i % h_water, &b);
      res.set_pixel_color(j, i, calc_merge_color(a, b));
    }
  }

  res.save(output_path);
  return true;
}
} // namespace itmo_bmp
