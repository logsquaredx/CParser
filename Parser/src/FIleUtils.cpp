#include "FileUtils.hpp"

#include <stdio.h>
#include <sys/stat.h>

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

bool isFileXml(char* filename, int filenameSize) {
    int xmlSuffixSize = 4;
    char xmlsuffix[] = ".xml";
    bool isXml = true;

    filename += filenameSize - xmlSuffixSize;
    
    for(int i = 0; i < xmlSuffixSize; ++i) {
        if(xmlsuffix[i] != *filename) {
            isXml = false;
            break;
        }
        ++filename;
    }
    
    
    return isXml;
}
