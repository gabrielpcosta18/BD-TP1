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
    vector<Article> loadRawArticles();
    void write(FileSystemBlock& block);
    FileSystemBlock read();
    Article readRawArticle();
    void close();
    bool isOpen();
    bool isEOF();
    FileInterface();
    FileInterface(string filePath, string mode);

    private:
    FILE *stream;

    int readColumn(char* columnFormat, void* variable, int isEndline);
    void readColumnByChar(char* variable);
};

#endif