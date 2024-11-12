#include <filesystem.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// private
inline bool dirent_meets_criteria(struct dirent* dirent_pointer, enum DirentCriteria critera) {
  switch(critera) {
    case DIRENT_IS_DIRECTORY:
      return dirent_pointer->d_type == DT_DIR && strcmp(dirent_pointer->d_name, ".") != 0 && strcmp(dirent_pointer->d_name, "..") != 0;
    case DIRENT_IS_FILE:
      return dirent_pointer->d_type == DT_REG;
    default:
      return false;
  }
}

// public
int get_dirent_names(const char* path, enum DirentCriteria criteria, const char*** output, unsigned int* output_length) {
  if(path == NULL || output == NULL || output_length == NULL)
    goto exit_failure;
  if(strlen(path) == 0)
    goto exit_success;

  unsigned int length = 0;
  if(get_dirent_names_length(path, criteria, &length) != 0)
    goto exit_success;
  if(length == 0)
    goto set_output_length;

  if((*output = (const char**) malloc(length * sizeof(const char*))) == NULL)
    goto exit_failure;

  DIR* directory_pointer = NULL;
  if((directory_pointer = opendir(path)) == NULL)
    goto free_output;

  unsigned int i = 0;
  struct dirent* dirent_pointer = NULL;
  while((dirent_pointer = readdir(directory_pointer)) != NULL) {
    if(i >= length)
      break;

    if(dirent_meets_criteria(dirent_pointer, criteria)) {
      *(*output + i) = NULL;
      if((*(*output + i) = strdup(dirent_pointer->d_name)) == NULL)
        goto free_output_contents;

      i ++;
    }
  }

  closedir(directory_pointer);

set_output_length:
  *output_length = length;
exit_success:
  return EXIT_SUCCESS;

free_output_contents:
  for(unsigned int j = 0; j < i; j ++) {
    free((char*) *(*output + j));
    *(*output + j) = NULL;
  }
  closedir(directory_pointer);
free_output:
  free(*output);
  *output = NULL;
exit_failure:
  return EXIT_FAILURE;
}

int get_dirent_names_length(const char* path, enum DirentCriteria criteria, unsigned int* output) {
  if(path == NULL || output == NULL)
    return EXIT_FAILURE;
  if(strlen(path) == 0)
    return EXIT_SUCCESS;

  DIR* directory_pointer = NULL;
  if((directory_pointer = opendir(path)) == NULL)
    return EXIT_FAILURE;

  unsigned int length = 0;
  struct dirent* dirent_pointer = NULL;
  while((dirent_pointer = readdir(directory_pointer)) != NULL)
    if(dirent_meets_criteria(dirent_pointer, criteria))
      length ++;

  *output = length;

  closedir(directory_pointer);
  return EXIT_SUCCESS;
}
