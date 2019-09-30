#include "FileUtils.hpp"
#include "Parser.hpp"

#include <unistd.h> // close()
#include <fcntl.h> // open()
#include <dirent.h> // opendir(), closedir(), dirent
#include <string.h> // for strcat()
#include <time.h> // for time()

// #include <thread>

int main() {
    printf("start: %lu\n", (unsigned long) time(NULL));
    
    char dirString[] = "../content/";
    DIR *directory = opendir(dirString);
    struct dirent *de;
    if(directory == NULL) {
        printf("error opening directory\n");
        return -1;
    }
    
    while((de = readdir(directory)) != NULL) {
        if(!isFileXml(de->d_name, de->d_namlen)) continue;
           
        char* filePath = (char*) calloc(sizeof(dirString) + de->d_namlen, sizeof(char));
        
        strcat(filePath, dirString);
        strcat(filePath, de->d_name);
        
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
    
    printf("end: %lu\n", (unsigned long) time(NULL));

    return 0;
}
