#include "ArgumentParser.h"
#include <cstdio>

using namespace itmo_bmp;

int main(int argc, char *argv[]) {
  ValidatedArguments validated_args;

  if (!parseAndValidateArguments(argc, argv, &validated_args)) {
    return 1;
  }

  printf("[Debug]: Cood Parsing.\n");
  printf("\tMainPicture: %s\n", validated_args.main_path_picture);
  printf("\tWatermarkPicture: %s\n", validated_args.watermark_path_picture);
  printf("\tResultPicture: %s\n", validated_args.result_path_picture);

  return 0;
}
