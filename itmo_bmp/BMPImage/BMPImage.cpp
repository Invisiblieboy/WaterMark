#include "BMPImage.h"

#include <fstream>
#include <iostream>
#include <vector>

#include "BMPHeaders.h"

// TODO Комменты к коду + если файл записан наоборот снизу вверх
namespace itmo_bmp {
BMPImage::BMPImage() {}
BMPImage::~BMPImage() {}

uint32_t BMPImage::get_width() { return width_; }

uint32_t BMPImage::get_height() { return height_; }

uint32_t BMPImage::get_color_depth() { return color_depth_; }

status_code BMPImage::load(const char* file_name) {
  std::ifstream file(file_name, std::ios::binary);

  if (!file.is_open()) {
    return status_code::kInvalidFile;
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
    return status_code::kInvalidFile;  // не BMP
  }

  // Переходим к пикселям по off_bits
  file.seekg(file_header.off_bits, std::ios::beg);

  gen_empty_sheet(info_header.width, info_header.height, info_header.bit_count);
  uint32_t row_size = ((info_header.bit_count * width_ + 31) / 32) * 4;
  std::vector<uint8_t> row(row_size);

  for (uint32_t i = 0; i < height_; ++i) {
    file.read(reinterpret_cast<char*>(row.data()), row_size);

    uint32_t y = height_ - 1 - i;
    for (uint32_t x = 0; x < width_; x++) {
      Pixel pxl;
      if (color_depth_ == 8) {
        pxl.b = row[x];
        pxl.g = row[x];
        pxl.r = row[x];
      } else if (color_depth_ == 24) {
        pxl.b = row[3 * x];
        pxl.g = row[3 * x + 1];
        pxl.r = row[3 * x + 2];
      } else
        return status_code::kInvalidFile;
      set_pixel_color(x, y, pxl);
    }
  }
  file.close();
  return status_code::kOK;
}

status_code BMPImage::save(const char* file_name) {
  if (image_.size() == 0) {
    return status_code::kUnknownError;
  }
  std::ofstream file(file_name, std::ios::binary);
  if (!file.is_open()) {
    return status_code::kInvalidFile;
  }

  uint32_t row_size = ((24 * width_ + 31) / 32) * 4;
  uint32_t pixel_size = row_size * height_;

  BMPFileHeader fh{};
  fh.type = 0x4D42;  // BMP
  fh.off_bits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
  fh.size = fh.off_bits + pixel_size;

  BMPInfoHeader ih{};
  ih.size = sizeof(BMPInfoHeader);
  ih.width = width_;
  ih.height = height_;
  ih.planes = 1;
  ih.bit_count = 24;
  ih.size_image = pixel_size;

  file.write(reinterpret_cast<const char*>(&fh), sizeof(fh));
  file.write(reinterpret_cast<const char*>(&ih), sizeof(ih));

  std::vector<uint8_t> row(row_size);

  for (uint32_t i = 0; i < height_; ++i) {
    uint32_t y = height_ - 1 - i;
    for (uint32_t x = 0; x < width_; x++) {
      Pixel pxl;
      get_pixel_color(x, y, &pxl);
      row[3 * x] = pxl.b;
      row[3 * x + 1] = pxl.g;
      row[3 * x + 2] = pxl.r;
    }
    file.write(reinterpret_cast<char*>(row.data()), row_size);
  }
  return status_code::kOK;
}

status_code BMPImage::get_pixel_color(const uint32_t x, const uint32_t y,
                                      Pixel* pixel) {
  // Изменяет пиксель под указателем на нужный(по координатам)
  if (x >= width_ || y >= height_) {
    return status_code::kInvalidArg;
  }
  *pixel = image_[y * width_ + x];
  return status_code::kOK;
}

status_code BMPImage::gen_empty_sheet(const int32_t width, const int32_t height,
                                      const uint16_t color_depth) {
  if (color_depth != 8 && color_depth != 24) {
    return status_code::kInvalidArg;
  }

  width_ = width;
  height_ = height > 0 ? height : -height;
  color_depth_ = color_depth;
  image_ = std::vector<Pixel>(width_ * height_);
  return status_code::kOK;
}
status_code BMPImage::set_pixel_color(uint32_t x, uint32_t y,
                                      const Pixel pixel) {
  if (x >= width_ || y >= height_) {
    return status_code::kInvalidArg;
  }
  image_[y * width_ + x] = pixel;

  return status_code::kOK;
}
}  // namespace itmo_bmp

int main() {
  itmo_bmp::BMPImage img;

  img.load("test.bmp");
  itmo_bmp::Pixel red;
  red.r = 255;
  itmo_bmp::Pixel blue;
  blue.b = 255;
  for (int i = 0; i < 90; i += 2) {
    img.set_pixel_color(100 + i, 100 - i, blue);
  }
  for (int i = 50; i < 400; i += 2) {
    img.set_pixel_color(i, i, red);
  }
  img.save("test2.bmp");
  itmo_bmp::BMPImage image2 = img;
  return 0;
}