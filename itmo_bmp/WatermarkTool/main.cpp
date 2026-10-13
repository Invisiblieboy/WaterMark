#include "ArgumentParser.h"
#include "Logger.h"
#include "../BMPImage/BMPImage.h"
#include "Watermark.h"

using namespace itmo_bmp;

const char* status_msg[] = {
  "kOK",
  "kFileNotFound",
  "kPermissionDenied",
  "kInvalidArg",
  "kEmptyFile",
  "kInvalidFile",
  "kUnknownError",
};

bool try_load(BMPImage& image, const char* path, const char *what) {
  status_code code = image.load(path);
  if (code != status_code::kOK) {
    Logger::Error("Не удалось загрузить %s (%s): %s", what, path, status_msg[code]);;
    return false;
  } else {
    Logger::Info("Загружен файл (%s): %s", what, path);
    return true;
  }
}

int main(int argc, char *argv[]) {
  ValidatedArguments validated_args;

  if (!parse_and_validate_arguments(argc, argv, &validated_args)) {
    return 1;
  }

  BMPImage main, watermark;
  if (!try_load(main, validated_args.main_path_picture, "основное изображение") ||
      !try_load(watermark, validated_args.watermark_path_picture, "водяной знак")) {
    return 1;
  }
  
  if(apply_watermark(main, watermark, validated_args.result_path_picture)) {
    Logger::Info("Водяной знак успешно наложен!");
  } else {
    Logger::Error("Не удалось наложить водяной знак");
  }

  return 0;
}
