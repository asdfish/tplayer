#include <directory.h>

#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DIRECTORY_FUNCTION(EXCLUSION, RETURN_ITEM) {                                         \
  DIR* directory_pointer = opendir(directory_path);                                          \
  if(directory_pointer == NULL)                                                              \
    goto failure_exit;                                                                       \
                                                                                             \
  unsigned int directory_length = 0;                                                         \
                                                                                             \
  struct dirent* dirent_pointer = NULL;                                                      \
  while((dirent_pointer = readdir(directory_pointer)) != NULL)                               \
    if(EXCLUSION)                                                                            \
      directory_length ++;                                                                   \
  closedir(directory_pointer);                                                               \
                                                                                             \
  if(directory_length == 0)                                                                  \
    goto failure_exit;                                                                       \
                                                                                             \
  if(chdir(directory_path) != 0)                                                             \
    goto failure_exit;                                                                       \
                                                                                             \
  const char** output_paths = (const char**) malloc(directory_length * sizeof(const char*)); \
  if(output_paths == NULL)                                                                   \
    goto failure_exit;                                                                       \
                                                                                             \
  directory_pointer = opendir(directory_path);                                               \
  if(directory_pointer == NULL)                                                              \
    goto free_output_paths;                                                                  \
                                                                                             \
  unsigned int i = 0;                                                                        \
  while((dirent_pointer = readdir(directory_pointer)) != NULL)                               \
    if(EXCLUSION) {                                                                          \
      output_paths[i] = RETURN_ITEM;                                                         \
                                                                                             \
      if(output_paths[i] == NULL)                                                            \
        goto free_output_paths_contents;                                                     \
                                                                                             \
      i ++;                                                                                  \
    }                                                                                        \
                                                                                             \
  closedir(directory_pointer);                                                               \
                                                                                             \
  if(directory_paths != NULL)                                                                \
    *directory_paths = output_paths;                                                         \
  if(directory_paths_length != NULL)                                                         \
    *directory_paths_length = directory_length;                                              \
  return 0;                                                                                  \
                                                                                             \
free_output_paths_contents:                                                                  \
  for(unsigned int j = 0; j < i; j ++)                                                       \
    free((char*) output_paths[j]);                                                           \
free_output_paths:                                                                           \
  free(output_paths);                                                                        \
failure_exit:                                                                                \
  return -1;                                                                                 \
}

int directory_directory_names(const char* directory_path, const char*** directory_paths, unsigned int* directory_paths_length)
  DIRECTORY_FUNCTION(
    dirent_pointer->d_type == DT_DIR &&
    strcmp(dirent_pointer->d_name, ".") != 0 &&
    strcmp(dirent_pointer->d_name, "..") != 0,
    strdup(dirent_pointer->d_name)
  )

int directory_directory_paths(const char* directory_path, const char*** directory_paths, unsigned int* directory_paths_length)
  DIRECTORY_FUNCTION(
    dirent_pointer->d_type == DT_DIR &&
    strcmp(dirent_pointer->d_name, ".") != 0 &&
    strcmp(dirent_pointer->d_name, "..") != 0,
    realpath(dirent_pointer->d_name, NULL)
  )

int directory_file_names(const char* directory_path, const char*** directory_paths, unsigned int* directory_paths_length)
  DIRECTORY_FUNCTION(
    dirent_pointer->d_type == DT_REG,
    strdup(dirent_pointer->d_name)
  )

int directory_file_paths(const char* directory_path, const char*** directory_paths, unsigned int* directory_paths_length)
  DIRECTORY_FUNCTION(
    dirent_pointer->d_type == DT_REG,
    realpath(dirent_pointer->d_name, NULL)
  )

int directory_file_paths_from_extension(const char* directory_path, const char* file_extension, const char*** directory_paths, unsigned int* directory_paths_length)
  DIRECTORY_FUNCTION(
    dirent_pointer->d_type == DT_REG &&
    strcmp(dirent_pointer->d_name + strlen(dirent_pointer->d_name) - strlen(file_extension), file_extension) == 0,
    realpath(dirent_pointer->d_name, NULL)
  )
