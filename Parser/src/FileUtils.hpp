#ifndef FileUtils_hpp
#define FileUtils_hpp

#include <sys/mman.h>

size_t getFileSize(int fd);
int mapFile(int fd, size_t size, char** mappedFile);

bool isFileXml(char* filename, int filenameSize);

#endif
