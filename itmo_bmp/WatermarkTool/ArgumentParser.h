#ifndef ARGUMENT_PARSER
#define ARGUMENT_PARSER

namespace itmo_bmp {

struct ValidatedArguments {
  char *main_path_picture;
  char *watermark_path_picture;
  char *result_path_picture;
};

bool parse_and_validate_arguments(int argc, char *argv[],
                               ValidatedArguments *args);

} // namespace itmo_bmp
#endif
