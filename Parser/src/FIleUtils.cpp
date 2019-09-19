#include "FileUtils.hpp"

#include <stdio.h>
#include <sys/stat.h>
#include <sys/mman.h>

size_t getFileSize(int fd) {
    struct stat s;
    if(fstat(fd, &s) < 0) {
        return -1;
    } else {
        return s.st_size;
    }
}

int mapFile(int fd, size_t size, char** mappedFile) {
    *mappedFile = (char*) mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(mappedFile == MAP_FAILED) {
        return -1;
    } else {
        return 0;
    }
}
