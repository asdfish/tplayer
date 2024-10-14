#ifndef DIRECTORY_H
#define DIRECTORY_H

extern int directory_directory_names(const char* directory_path, const char*** directory_paths, unsigned int* directory_paths_length);
extern int directory_directory_paths(const char* directory_path, const char*** directory_paths, unsigned int* directory_paths_length);
extern int directory_file_names(const char* directory_path, const char*** directory_paths, unsigned int* directory_paths_length);
extern int directory_file_paths(const char* directory_path, const char*** directory_paths, unsigned int* directory_paths_length);
extern int directory_file_paths_from_extension(const char* directory_path, const char* file_extension, const char*** directory_paths, unsigned int* directory_paths_length);

#endif
