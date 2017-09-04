#ifndef _FILE_INTERFACE_
#define _FILE_INTERFACE_

#include <stdio.h>
#include <iostream>
#include <article.hpp>
#include <block.hpp>
#include <vector>

using namespace std;

class FileInterface {
    public:        
    void write(char* bytes, unsigned size);
    void write(char* bytes, unsigned size, unsigned long long int position);
    char* read(unsigned size);
    char* read(unsigned size, unsigned long long int position);
    long int getFileSize();
    
    void close();
    bool isOpen();
    bool isEOF();
    
    Article readRawArticle();
    
    FileInterface();
    FileInterface(string filePath, string mode);

    private:
    FILE *stream;

    int readColumn(char* columnFormat, void* variable, int isEndline);
    void readColumnByChar(char* variable);
};

#endif