#include "Parser.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

size_t getElementName(char* xmlChunk, char** elemName) {
    // allocate memory for a 16 character element name
    size_t maxElemSize = 16;
    *elemName = (char*) malloc(maxElemSize * sizeof(char));

    size_t currentElemSize = 0;
    // skip past opening <
    ++xmlChunk;
    while(*xmlChunk != '>') {
        *elemName += currentElemSize;
        **elemName = *xmlChunk;
        *elemName -= currentElemSize;
        
        if(++currentElemSize == maxElemSize) {
            // out of memory, allocate more
            *elemName = (char*) realloc(*elemName, maxElemSize *= 2);
        }
        
        ++xmlChunk;
    }
    
    //*elemName -= currentElemSize;

    return currentElemSize;
}

void parse(char* xmlChunk, size_t chunkSize) {
    for(int i = 0; i < chunkSize; i++) {
        if(*xmlChunk == '<') {
            char* elemName;
            size_t elemSize = getElementName(xmlChunk, &elemName);
            printf("element name size: %zu\n", elemSize);
            printf("elem name: %s\n", elemName);
            break;
        }
    }
}
