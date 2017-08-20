#include <file_interface.hpp>

#include <stdio.h>
#include <iostream>
#include <article.hpp>
#include <vector>

using namespace std;

int FileInterface::readColumn(char* columnFormat, void* variable, int isEndline) {
    int read = fscanf(stream, columnFormat, variable);
    if(!read) {
        if(!isEndline) 
            read = fscanf(stream, "%[^;];", variable);
        else read = fscanf(stream, "%[^\n]\n", variable);
        if(read == 0) {
            if(!isEndline)
                read = fscanf(stream, ";", variable);
            else read = fscanf(stream, "\n", variable);
            if(!read) {
                cout << "Parser failed" << endl;
            }
        }
    }

    return read;
}

void FileInterface::readColumnByChar(char* variable) {
    int position = 0;
    char c;

    if(!feof(stream)) {
        c = fgetc(stream);
        if(c == '"') {
            bool ready = false;
            char buffer = '\0';
            while(!feof(stream)) {
                c = fgetc(stream);
                if(ready && c == ';') {
                    variable[position] = '\0';
                    return;
                }
                else if(c == '"') {
                    ready = true;
                    buffer = '"';
                }
                else {
                    if(buffer == '"') {
                        variable[position] = '"';
                        position++;    
                    }

                    variable[position] = c;
                    position++;
                    
                    ready = false;
                    buffer = '\0';
                }
            }
        }   
        else if(c == 'N') {
            c = fgetc(stream);
            c = fgetc(stream);
            c = fgetc(stream);
            c = fgetc(stream);
            variable[0] = '\0';

            return;
        } 
        else if(c == ';') {
            variable[0] = '\0';
            
            return;
        }
    }
}

vector<Article> FileInterface::loadRawArticles() {
    int id_bfr, citations_bfr, year_bfr;
    char title_bfr[TITLE_SIZE * 4], authors_bfr[AUTHORS_SIZE * 4], 
        snippet_bfr[SNIPPET_SIZE * 4], date_bfr[DATE_SIZE * 4];

    vector<Article> articles;
    while(!feof(stream)) {
        if(!readColumn( "\"%d\";", &id_bfr, 0)) {
            cout << "Parser failed: Column title couldn't be parsed" << endl;
            break;
        }

        readColumnByChar( title_bfr);

        if(!readColumn( "\"%d\";", &year_bfr, 0)) {
            year_bfr = 0;
        }

        readColumnByChar( authors_bfr);

        if(!readColumn( "\"%d\";", &citations_bfr, 0)) {
            citations_bfr = 0;
        }

        if(!readColumn( "\"%[^\"]\";", date_bfr, 0)) {
            date_bfr[0] = '\0';
        }

        if(!readColumn( "%[^\n]\n", snippet_bfr, 1)) {
            snippet_bfr[0] = '\0';
        }
        
        articles.push_back(Article(id_bfr, year_bfr, citations_bfr, 
            date_bfr, title_bfr, authors_bfr, snippet_bfr));
    }

    return articles;
}

void FileInterface::writeArticle(Article article) {
    //char* bytes = article.toByteArray();
    fwrite(&article.getData(), Article::getSizeOfData(), 1, stream);
}

Article FileInterface::readArticle() {
    //char bytes[Article::getSizeOfData()];
    Article result;
    fread(&result, Article::getSizeOfData(), 1, stream);
    return result;
}

Article FileInterface::readRawArticle() {
    int id_bfr, citations_bfr, year_bfr;
    char title_bfr[TITLE_SIZE * 4], authors_bfr[AUTHORS_SIZE * 4], 
        snippet_bfr[SNIPPET_SIZE * 4], date_bfr[DATE_SIZE * 4];

    if(!feof(stream)) {
        if(!readColumn( "\"%d\";", &id_bfr, 0)) {
            cout << "Parser failed: Column title couldn't be parsed" << endl;
            return Article(-1);
        }

        readColumnByChar( title_bfr);

        if(!readColumn( "\"%d\";", &year_bfr, 0)) {
            year_bfr = 0;
        }

        readColumnByChar( authors_bfr);

        if(!readColumn( "\"%d\";", &citations_bfr, 0)) {
            citations_bfr = 0;
        }

        if(!readColumn( "\"%[^\"]\";", date_bfr, 0)) {
            date_bfr[0] = '\0';
        }

        if(!readColumn( "%[^\n]\n", snippet_bfr, 1)) {
            snippet_bfr[0] = '\0';
        }
        
        return Article(id_bfr, year_bfr, citations_bfr, 
            date_bfr, title_bfr, authors_bfr, snippet_bfr);
    }

    return Article(-1);
}

void FileInterface::close() {
    fclose(stream);
}

bool FileInterface::isOpen() {
    return stream != NULL;
}

FileInterface::FileInterface() {}

FileInterface::FileInterface(string filePath, string mode) {
    stream = fopen(filePath.c_str(), mode.c_str());
}