#include <path.h>
#include <string.h>

const char* path_get_file_name(const char* path) {
  unsigned int length = strlen(path);
  if(length == 0 || path[length - 1] == '/')
    return path;

  for(int i = length - 1; i >= 0; i --)
    if(path[i] == '/')
      return path + i + 1;
  return path;
}
