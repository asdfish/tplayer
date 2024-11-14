#include <stroke_functions.h>
#include <main.h>

int stroke_function_quit(const struct Argument* argument) {
  running = false;
  return EXIT_SUCCESS;
}
