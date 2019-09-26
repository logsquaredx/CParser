#include "FileUtils.hpp"
#include "Parser.hpp"

#include <unistd.h> // close()
#include <fcntl.h> // open()
#include <dirent.h> // opendir(), closedir(), dirent
#include <string.h> // for memcpy


// #include <thread>

int main() {
    char dirString[] = "../content/";
    DIR *directory = opendir(dirString);
    struct dirent *de;
    if(directory == NULL) {
        printf("error opening directory\n");
        return -1;
    }
    
    while((de = readdir(directory)) != NULL) {
        if(!isFileXml(de->d_name, de->d_namlen)) continue;
           
        char* filePath = (char*) malloc((sizeof(dirString) + de->d_namlen) * sizeof(char));
        
        printf("%s\n", de->d_name);
        printf("%d\n", de->d_namlen);
        memcpy(filePath, dirString, sizeof(dirString) * sizeof(char));
        printf("%s\n", filePath);
        memcpy(filePath + sizeof(dirString) - 1, de->d_name, de->d_namlen * sizeof(char));
        printf("%s\n", filePath);
        
        break;
        
        int fd = open(filePath, O_RDONLY);
        if(fd < 0) {
            printf("error opening file\n");
            return -1;
        }
        
        size_t size = getFileSize(fd);
        if(size < 0) {
            printf("failed to get file size\n");
            return -1;
        }
        
        char* xmlContent;
        if(mapFile(fd, size, &xmlContent) < 0) {
            printf("failed to mmap file\n");
            return -1;
        }

        char xPath[] = "/elemelemelemelemelem/elem80/elem81/text()";
        if(parse(xmlContent, size, xPath) < 0) {
            printf("parse failed\n");
            return -1;
        }
        
        munmap(xmlContent, size);
        close(fd);
    }
    
    closedir(directory);
    
//    unsigned int nthreads = std::thread::hardware_concurrency();
//    printf("Threads: %d\n", nthreads);
//    std::thread first (parseGlob, 1);
//    std::thread second (parseGlob, 2);
//    first.join();
//    second.join();
    
    return 0;
}
