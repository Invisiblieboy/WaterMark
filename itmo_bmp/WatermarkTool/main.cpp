#include "ArgumentParser.h"
#include "Logger.h"
#include "../BMPImage/BMPImage.h"
#include "Watermark.h"

using namespace itmo_bmp;

const char* erroreMsg[] = {
  "kOK",
  "kFileNotFound",
  "kPermissionDenied",
  "kInvalidArg",
  "kEmptyFile",
  "kInvalidFile",
  "kUnknownError",
};

bool try_load(BMPImage& image, const char* path) {
  status_code code = image.load(path);
  if (code != status_code::kOK) {
    Logger::Error("Не получилось лол, подробней: %s", erroreMsg[code]);
    return false;
  } else {
    Logger::Info("Всё ок: %s", erroreMsg[code]);
    return true;
  }
}

int main(int argc, char *argv[]) {
  ValidatedArguments validated_args;

  if (!parse_and_validate_arguments(argc, argv, &validated_args)) {
    return 1;
  }

  BMPImage main, watermark;
  if (!try_load(main, validated_args.main_path_picture) ||
      !try_load(watermark, validated_args.watermark_path_picture)) {
    return 1;
  }
  
  if(apply_watermark(main, watermark, validated_args.result_path_picture)) {
    Logger::Info("Вводный знак успешно наложен!");
  } else {
    Logger::Error("ЧТО-ТО пошло не так!");
  }

  return 0;
}
