#include "FileUtils.hpp"
#include "Parser.hpp"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>

void parseGlob(int id) {
    printf("in parse glob: %d\n", id);
}

int main() {
    int fd = open("../content/test.xml", O_RDONLY);
    if(fd < 0) {
        printf("error opening file");
        return 1;
    }
    
    size_t size = getFileSize(fd);
    if(size < 0) {
        printf("failed to get file size");
        return -1;
    }
    
    char* xmlContent;
    if(mapFile(fd, size, &xmlContent) < 0) {
        printf("failed to mmap file");
        return -1;
    }

    char xPath[] = {'/', 'e', 'l', 'e', 'm', 'e', 'l', 'e', 'm', 'e','l', 'e', 'm', 'e', 'l', 'e', 'm', 'e', 'l', 'e', 'm',
        '/', 'e', 'l', 'e', 'm', '8', '0',
        '/', 'e', 'l', 'e', 'm', '8', '1',
        '/', 't', 'e', 'x', 't', '(', ')'};
    if(parse(xmlContent, size, xPath) < 0) {
        printf("parse failed");
        return -1;
    }
    
    unsigned int nthreads = std::thread::hardware_concurrency();
    printf("Threads: %d\n", nthreads);
    std::thread first (parseGlob, 1);
    std::thread second (parseGlob, 2);
    first.join();
    second.join();
    
    munmap(xmlContent, size);
    close(fd);
    
    return 0;
}
