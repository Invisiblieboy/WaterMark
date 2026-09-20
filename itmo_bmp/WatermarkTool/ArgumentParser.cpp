#include <filesystem>
#include <iostream>
#include <system_error>

#include "ArgumentParser.h"
#include "Logger.h"

namespace fs = std::filesystem;
namespace itmo_bmp {

bool check_write_permession(const fs::path &path) {
  fs::file_status s = fs::status(path);
  fs::perms prms = s.permissions();

  return (prms & fs::perms::owner_write) != fs::perms::none;
}

bool check_read_permession(const fs::path &path) {
  fs::file_status s = fs::status(path);
  fs::perms prms = s.permissions();

  return (prms & fs::perms::owner_read) != fs::perms::none;
}

bool is_directory_writable(const fs::path &dir_path) {
  try {
    if (!fs::exists(dir_path) || !fs::is_directory(dir_path)) {
      return false;
    }

    fs::perms p = fs::status(dir_path).permissions();

    if ((p & fs::perms::owner_write) != fs::perms::none ||
        (p & fs::perms::group_write) != fs::perms::none ||
        (p & fs::perms::others_write) != fs::perms::none) {
      return true;
    }
  } catch (const fs::filesystem_error &e) {
    std::cerr << "Ошибка файловой системы: " << e.what() << '\n';
  }

  return false;
}

bool validatePath(const char *c_path) {
  if (!c_path) {
    Logger::Error("Передан нулевой указатель (nullptr)!");
    return false;
  }

  std::error_code ec;
  fs::path path = fs::absolute(c_path);

  if (!fs::exists(path, ec)) {
    Logger::Error("Путь не существует или недоступен: %s", path.c_str());
    return false;
  }

  if (!fs::is_regular_file(path, ec)) {
    Logger::Error("Указанный путь ведет к папке, а не к файлу: %s",
                  path.c_str());
    return false;
  }

  if (path.extension() != ".bmp") {
    Logger::Error("Расширенние файла не .bmp");
    return false;
  }

  if (!check_read_permession(path)) {
    Logger::Error("Недостаточно прав для чтения");
    return false;
  }

  return true;
}

bool validateAndCreateOutputPath(const char *c_path) {
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

  if (path.extension() != ".bmp") {
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

void printUsage(const char *proga) {
  Logger::Info("Пример вызова:\n\t%s </path/to/file.bmp> "
               "</path/to/watermark.bmp> <path/to/result.bmp>",
               proga);
}

bool parseAndValidateArguments(int argc, char *argv[],
                               ValidatedArguments *args) {
  if (argc != 4) {
    Logger::Error("Неверное количество переданных аргументов");
    printUsage(argv[0]);
    return false;
  }

  if (!validatePath(argv[1]) || !validatePath(argv[2]) ||
      !validateAndCreateOutputPath(argv[3])) {
    printUsage(argv[0]);
    return false;
  }

  args->main_path_picture = argv[1];
  args->watermark_path_picture = argv[2];
  args->result_path_picture = argv[3];

  return true;
}
} // namespace itmo_bmp
