#ifndef FILESYSTEM_H
#define FILESYSTEM_H

enum DirentCriteria {
  DIRENT_IS_DIRECTORY,
  DIRENT_IS_FILE,
};
enum DirentInfo {
  DIRENT_NAME,
  DIRENT_PATH,
};

int get_dirents_info(const char* path, enum DirentCriteria criteria, enum DirentInfo info_type, const char*** output, unsigned int* output_length);
int get_dirents_length(const char* path, enum DirentCriteria critera, unsigned int* output);

#endif
