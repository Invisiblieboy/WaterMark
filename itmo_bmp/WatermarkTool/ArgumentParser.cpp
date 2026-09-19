#include <cstring>
#include <filesystem>
#include <iostream>
#include <system_error>

#include "ArgumentParser.h"

namespace fs = std::filesystem;
namespace itmo_bmp {
bool validatePath(const char *c_path) {
  if (!c_path) {
    std::cerr << "Ошибка: Передан нулевой указатель (nullptr)!\n";
    return false;
  }

  std::error_code ec;
  fs::path path = fs::absolute(c_path);

  if (!fs::exists(path, ec)) {
    fprintf(stderr, "[Error]: Путь не существует или недоступен: %s.\n",
            path.c_str());
    return false;
  }

  if (!fs::is_regular_file(path, ec)) {
    fprintf(stderr,
            "[Error]: Указанный путь ведет к папке, а не к файлу: %s.\n",
            path.c_str());
    return false;
  }
  if (!strcmp(path.extension().c_str(), "bmp")) {
    fprintf(stderr, "[Error]: Расширенние файла не .bmp.\n");
    return false;
  }
  return true;
}

bool parseAndValidateArguments(int argc, char *argv[],
                               ValidatedArguments *args) {
  if (argc != 4) {
    std::cerr << "[Error]: Неверное количество переданных аргументов\n";
    fprintf(stderr,
            "\tПример вызова: %s </path/to/file.bmp> </path/to/watermark.bmp> "
            "<path/to/result.bmp>\n",
            argv[0]);
    return false;
  }

  if (!validatePath(argv[1]) || !validatePath(argv[2])) {
    fprintf(stderr,
            "\tПример вызова: %s </path/to/file.bmp> </path/to/watermark.bmp> "
            "<path/to/result.bmp>\n",
            argv[0]);
    return false;
  }

  args->main_path_picture = argv[1];
  args->watermark_path_picture = argv[2];
  args->result_path_picture = argv[3];

  return true;
}
} // namespace itmo_bmp
