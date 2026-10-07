#include "Watermark.h"
#include <cstdint>

namespace itmo_bmp {

Pixel calc_merge_color(Pixel a, Pixel b) {
  return {.r = (uint8_t)((a.r + b.r) / 2),
          .g = (uint8_t)((a.g + b.g) / 2),
          .b = (uint8_t)((a.b + b.b) / 2)};
}

bool apply_watermark(BMPImage &main, BMPImage &watermark,
                    const char *output_path) {
  uint32_t w_main = main.get_width();
  uint32_t h_main = main.get_height();
  uint32_t w_water = watermark.get_width();
  uint32_t h_water = watermark.get_height();
  
  BMPImage res;
  res.gen_empty_sheet(w_main, h_main, main.get_color_depth());
  if (w_water > w_main || h_water > h_main) {
    return false;
  }

  Pixel a, b;
  for (uint32_t i = 0; i < h_main; ++i) {
    for (uint32_t j = 0; j < w_main; ++j) {
      main.get_pixel_color(j, i, &a);
      watermark.get_pixel_color(j % w_water, i % h_water, &b);
      res.set_pixel_color(j, i, calc_merge_color(a, b));
    }
  }
  
  res.save(output_path);
  return true;
}
} // namespace itmo_bmp
