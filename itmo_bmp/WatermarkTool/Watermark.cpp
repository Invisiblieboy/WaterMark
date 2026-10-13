#include "Watermark.h"
#include <cstdint>

namespace itmo_bmp {

Pixel calc_merge_color(Pixel a, Pixel b) {
  return {.r = (uint8_t)((a.r + b.r) / 2),
          .g = (uint8_t)((a.g + b.g) / 2),
          .b = (uint8_t)((a.b + b.b) / 2)};
}

status_code apply_watermark(BMPImage &base, BMPImage &watermark,
                    const char *output_path) {
  uint32_t w_base = base.get_width();
  uint32_t h_base = base.get_height();
  uint32_t w_water = watermark.get_width();
  uint32_t h_water = watermark.get_height();
  
  BMPImage res;
  res.gen_empty_sheet(w_base, h_base, base.get_color_depth());
  if (w_water > w_base || h_water > h_base) {
    return status_code::kInvalidFile;
  }

  Pixel a, b;
  for (uint32_t i = 0; i < h_base; ++i) {
    for (uint32_t j = 0; j < w_base; ++j) {
      base.get_pixel_color(j, i, &a);
      watermark.get_pixel_color(j % w_water, i % h_water, &b);
      res.set_pixel_color(j, i, calc_merge_color(a, b));
    }
  }
  
  
  return res.save(output_path);
}
} // namespace itmo_bmp
