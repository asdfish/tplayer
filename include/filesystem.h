#ifndef FILESYSTEM_H
#define FILESYSTEM_H

enum DirentCriteria {
  DIRENT_IS_DIRECTORY,
  DIRENT_IS_FILE,
};

int get_dirent_names(const char* path, enum DirentCriteria criteria, const char*** output, unsigned int* output_length);
int get_dirent_names_length(const char* path, enum DirentCriteria critera, unsigned int* output);

#endif
