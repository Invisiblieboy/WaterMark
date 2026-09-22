#include <filesystem>
#include <iostream>
#include <strings.h>//strcasecmp
#include <system_error>
#include <unistd.h>

#include "ArgumentParser.h"
#include "Logger.h"

namespace fs = std::filesystem;
namespace itmo_bmp {
namespace {
bool check_read_permission(const fs::path &path) {
  return access(path.c_str(), R_OK) == 0;
}

bool is_directory_writable(const fs::path &dir_path) {
  std::error_code ec;
  if (!fs::is_directory(dir_path, ec)) {
    return false;
  }

  return access(dir_path.c_str(), W_OK | X_OK) == 0;
}

bool has_bmp_extension(const fs::path &path) {
  return strcasecmp(path.extension().c_str(), ".bmp") == 0;
}

bool validate_input_path(const char *c_path) {
  if (!c_path) {
    Logger::Error("Передан нулевой указатель (nullptr)!");
    return false;
  }

  std::error_code ec;
  fs::path path(c_path);

  if (!fs::exists(path, ec)) {
    Logger::Error("Путь не существует или недоступен: %s", path.c_str());
    return false;
  }

  if (!fs::is_regular_file(path, ec)) {
    Logger::Error("Указанный путь ведет к папке, а не к файлу: %s",
                  path.c_str());
    return false;
  }

  if (!has_bmp_extension(path)) {
    Logger::Error("Расширенние файла не .bmp");
    return false;
  }

  if (!check_read_permission(path)) {
    Logger::Error("Недостаточно прав для чтения");
    return false;
  }

  return true;
}

bool validate_and_create_output_path(const char *c_path) {
  if (!c_path) {
    Logger::Error("Передан нулевой указатель для выходного пути!");
    return false;
  }

  fs::path path(c_path);

  if (!path.has_filename() || path.filename() == "." ||
      path.filename() == "..") {
    Logger::Error("Выходной путь не содержит имени файла: %s", c_path);
    return false;
  }

  if (!has_bmp_extension(path)) {
    Logger::Error("Выходной файл должен иметь расширение .bmp");
    return false;
  }

  fs::path dir_path = path.parent_path();
  if (dir_path.empty()) {
    dir_path = fs::current_path();
  }

  std::error_code ec;
  if (!fs::exists(dir_path, ec)) {
    fs::create_directories(dir_path, ec);
    if (ec) {
      Logger::Error("Не удалось создать директорию: %s", dir_path.c_str());
      return false;
    }
  }

  if (!is_directory_writable(dir_path)) {
    Logger::Error("Нет прав на запись в директорию: %s", dir_path.c_str());
    return false;
  }

  if (fs::exists(path, ec)) {
    Logger::Warning("Файл %s уже существует. Перезаписать? (y/n):",
                    path.filename().c_str());
    char answer;
    std::cin >> answer;
    if (answer != 'y' && answer != 'Y') {
      Logger::Info("Операция отменена пользователем");
      return false;
    }
  }

  return true;
}
} // namespace

void print_usage(const char *proga) {
  Logger::Info("Пример вызова:\n\t%s </path/to/file.bmp> "
               "</path/to/watermark.bmp> <path/to/result.bmp>",
               proga);
}

bool parse_and_validate_arguments(int argc, char *argv[],
                                  ValidatedArguments *args) {
  if (argc != 4) {
    Logger::Error("Неверное количество переданных аргументов");
    print_usage(argv[0]);
    return false;
  }

  if (!validate_input_path(argv[1]) || !validate_input_path(argv[2]) ||
      !validate_and_create_output_path(argv[3])) {
    print_usage(argv[0]);
    return false;
  }

  args->main_path_picture = argv[1];
  args->watermark_path_picture = argv[2];
  args->result_path_picture = argv[3];

  return true;
}
} // namespace itmo_bmp
