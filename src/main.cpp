#include <iostream>
#include <vector>

#include <article.hpp> 
#include <constants.hpp>
#include <file_interface.hpp>

using namespace std;

int readColumn(FILE *stream, char* columnFormat, void* variable, int isEndline) {
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

void readColumnByChar(FILE *stream, char* variable) {
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

int main() {
    ios::sync_with_stdio(false);
    // FILE *stream = fopen("out.csv", "r");
    // vector<Article> articles;
    // while(!feof(stream)) {
    //     char read[4096];
    //     fread(read, 8, 1, stream);
    //     articles.push_back(Article(read));
    //     cout << articles[articles.size() - 1].getData().m_id << endl;
    // }
    
    // FILE *stream = fopen("test.csv", "r");
    // int id, citations, year;
    // char title[TITLE_SIZE * 4], authors[AUTHORS_SIZE * 4], snippet[SNIPPET_SIZE * 4], date[DATE_SIZE * 4];
    
    // vector<Article> articles; 
    // if(stream != NULL) {
    //     while(!feof(stream)) {            
    //         if(!readColumn(stream, "\"%d\";", &id, 0)) {
    //             cout << "Parser failed: Column title couldn't be parsed" << endl;
    //             break;
    //         }

    //         readColumnByChar(stream, title);

    //         if(!readColumn(stream, "\"%d\";", &year, 0)) {
    //             year = 0;
    //         }

    //         readColumnByChar(stream, authors);

    //         if(!readColumn(stream, "\"%d\";", &citations, 0)) {
    //             citations = 0;
    //         }

    //         if(!readColumn(stream, "\"%[^\"]\";", date, 0)) {
    //             date[0] = '\0';
    //         }

    //         if(!readColumn(stream, "%[^\n]\n", snippet, 1)) {
    //             snippet[0] = '\0';
    //         }
            
    //         articles.push_back(Article(id, year, citations, date, title, authors, snippet));
    //     }
    // }

    // cout << endl;

    // FILE *out = fopen("out.csv", "wb+");
    // for(int i = 0; i < articles.size(); ++i) {
    //     cout << articles[i].toByteArray() << '\n';
    //     fwrite(articles[i].toByteArray(), sizeof(articles[i].toByteArray()), 1, out);
    // }
    // fclose(stream);
    // // for(int i = 0; i < articles.size(); ++i) {
    // //     Article a(articles[i].toByteArray());
    // //     cout << a.getData().m_title << endl;
    // // }
    
    FileInterface  interface("artigo.csv", "r");
    vector<Article> articles = interface.loadRawArticle();
    for(int i = 0; i < articles.size(); ++i) 
        cout << articles[i].getData().m_id << endl;
    return 0;
}
