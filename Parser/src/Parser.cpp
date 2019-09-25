#include "Parser.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

size_t getElementName(char** xmlChunk, char** elemName) {
    // allocate memory for a 16 character element name
    size_t maxElemSize = 16;
    *elemName = (char*) malloc(maxElemSize * sizeof(char));
    if(!elemName) {
        printf("memory allocation failed");
        return -1;
    }
    
    size_t elemSize = 0;
    // skip past opening <
    ++*xmlChunk;
    while(**xmlChunk != '>') {
        **elemName = **xmlChunk;
        
        if(++elemSize == maxElemSize) {
            // out of memory, allocate more
            // reset the pointer to beginning so realloc works
            *elemName = (char*) realloc(*elemName -= elemSize - 1, maxElemSize *= 2);
            // set pointer forward to where it was before realloc
            *elemName += elemSize - 1;
        }
        
        ++*elemName;
        ++*xmlChunk;
    }
    
    *elemName -= elemSize;

    return elemSize;
}

bool compareElementNames(char* one, char* two, size_t size) {
    bool equal = true;
    for(int i = 0; i < size; ++i) {
        if(*one != *two) {
            equal = false;
            break;
        }
        ++one;
        ++two;
    }
    
    return equal;
}

int parse(char* xmlChunk, size_t chunkSize, char* xPath) {
    // skip past '/'
    ++xPath;
    for(int i = 0; i < chunkSize; ++i) {
        if(*xmlChunk == '<') {
            char* elemName;
            size_t elemSize = getElementName(&xmlChunk, &elemName);
            if(elemSize < 0) {
                printf("getElementName failed");
                return -1;
            }
            
            if(compareElementNames(elemName, xPath, elemSize)) {
                printf("elements were equal\n");
            }
            
            // printf("elem name: %s\n", elemName);
            free(elemName);
            break;
        }
        ++xmlChunk;
    }
    
    return 0;
}
