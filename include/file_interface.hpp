#ifndef _FILE_INTERFACE_
#define _FILE_INTERFACE_

#include <stdio.h>
#include <iostream>
#include <article.hpp>
#include <vector>

using namespace std;

class FileInterface {
    public:
    vector<Article> loadRawArticles();
    void writeArticle(Article article);
    Article readArticle();
    Article readRawArticle();
    void close();
    bool isOpen();
    FileInterface();
    FileInterface(string filePath, string mode);

    private:
    FILE *stream;

    int readColumn(char* columnFormat, void* variable, int isEndline);
    void readColumnByChar(char* variable);
};

#endif