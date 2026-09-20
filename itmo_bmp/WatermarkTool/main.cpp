#include "ArgumentParser.h"
#include "Logger.h"

using namespace itmo_bmp;

int main(int argc, char *argv[]) {
  ValidatedArguments validated_args;

  if (!parseAndValidateArguments(argc, argv, &validated_args)) {
    return 1;
  }

  Logger::Info("Parsed data:\n"
               "\tMainPicture: %s\n"
               "\tWatermarkPicture: %s\n"
               "\tResultPicture: %s",
               validated_args.main_path_picture,
               validated_args.watermark_path_picture,
               validated_args.result_path_picture);

  return 0;
}
