#ifndef ITMO_LOGGER
#define ITMO_LOGGER
#include <cstdio>

namespace itmo_bmp {

class Logger {
private:
  Logger() = delete;

  static constexpr const char *RESET = "\033[0m";
  static constexpr const char *RED = "\033[31m";
  static constexpr const char *GREEN = "\033[32m";
  static constexpr const char *YELLOW = "\033[33m";

public:
  static void Info(const char *msg) {
    std::printf("%s[WatermarkTool:INFO]%s %s\n", GREEN, RESET, msg);
  }

  template <typename... Args> static void Info(const char *fmt, Args... args) {
    std::printf("%s[WatermarkTool:INFO]%s ", GREEN, RESET);
    std::printf(fmt, args...);
    std::printf("\n");
  }

  static void Warning(const char *msg) {
    std::fprintf(stderr, "%s[WatermarkTool:WARNING]%s %s\n", YELLOW, RESET,
                 msg);
  }

  template <typename... Args>
  static void Warning(const char *fmt, Args... args) {
    std::fprintf(stderr, "%s[WatermarkTool:WARNING]%s ", YELLOW, RESET);
    std::fprintf(stderr, fmt, args...);
    std::fprintf(stderr, "\n");
  }

  static void Error(const char *msg) {
    std::fprintf(stderr, "%s[WatermarkTool:ERROR]%s %s\n", RED, RESET, msg);
  }

  template <typename... Args> static void Error(const char *fmt, Args... args) {
    std::fprintf(stderr, "%s[WatermarkTool:ERROR]%s ", RED, RESET);
    std::fprintf(stderr, fmt, args...);
    std::fprintf(stderr, "\n");
  }
};
} // namespace itmo_bmp
#endif
