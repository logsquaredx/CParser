#include "Parser.hpp"

size_t getElementName(char** xmlChunk, char** elemName) {
    // skip past opening <
    ++*xmlChunk;
    
    // allocate memory for a 16 character element name
    size_t maxElemSize = 16;
    *elemName = (char*) malloc(maxElemSize * sizeof(char));
    if(!elemName) {
        printf("memory allocation for element name failed\n");
        return -1;
    }
    
    size_t elemSize = 0;
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

bool checkReadyToExtractText(char* xPath) {
    bool ready = true;
    char extractText[] = {'t', 'e', 'x', 't', '(', ')'};
    
    for(int i = 0; i < 6; i++) {
        if(extractText[i] != *xPath) {
            ready = false;
            break;
        }
        
        ++xPath;
    }
    
    return ready;
}

int extractText(char** xmlChunk, char** extractedText) {
    // skip past '>'
    ++*xmlChunk;
    
    size_t maxTextSize = 16;
    *extractedText = (char*) malloc(maxTextSize * sizeof(char));
    if(!extractedText) {
        printf("memory allocation for text extraction failed\n");
        return -1;
    }
    
    size_t textSize = 0;
    while(**xmlChunk != '<') {
        **extractedText = **xmlChunk;
        
        if(++textSize == maxTextSize) {
            // out of memory, allocate more
            // reset the pointer to beginning so realloc works
            *extractedText = (char*) realloc(*extractedText -= textSize - 1, maxTextSize *= 2);
            // set pointer forward to where it was before realloc
            *extractedText += textSize - 1;
        }
        
        ++*extractedText;
        ++*xmlChunk;
    }
    
    *extractedText -= textSize;
    
    return 0;
}

int parse(char* xmlChunk, size_t chunkSize, char* xPath) {
    // skip past '/'
    ++xPath;
    for(int i = 0; i < chunkSize; ++i) {
        if(*xmlChunk == '<') {
            char* elemName;
            size_t elemSize = getElementName(&xmlChunk, &elemName);
            if(elemSize < 0) {
                printf("getElementName failed\n");
                return -1;
            }
            
            if(compareElementNames(elemName, xPath, elemSize)) {
                // found element we were looking for,
                // move past it and the next '/'
                xPath += elemSize + 1;
                
                if(checkReadyToExtractText(xPath)) {
                    char* extractedtext;
                    extractText(&xmlChunk, &extractedtext);
                    
                    printf("extracted text: %s\n", extractedtext);
                    free(elemName);
                    free(extractedtext);
                    break;
                }
            }
            
            // printf("elem name: %s\n", elemName);
            free(elemName);
        }
        ++xmlChunk;
    }
        
    return 0;
}
