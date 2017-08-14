#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <regex>

#include <article.hpp> 

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
    // ifstream stream("artigo.csv");
    // stringstream buffer;
    // buffer << stream.rdbuf();
    
    // //regex re("\"(.*)\";\"(.*)\";\"(.*)\";\"(.*)\";\"(.*)\";\"(.*)\";((\".*\")|NULL)");
    // //regex re("\"(.*)\";(\".*\"|NULL)?;(\".*\"|NULL)?;(\".*\"|NULL)?;(\".*\"|NULL)?;(\".*\"|NULL)?;(\".*\"|NULL)?");
    // //regex re("\"(.*)\";(?:\"(.*)\"|(NULL));(?:\"(.*)\"|(NULL));(?:\"(.*)\"|(NULL));(?:\"(.*)\"|(NULL));(?:\"(.*)\"|(NULL));(?:\"(.*)\"|(NULL))");
    // regex re("\"(.*)\";(?:\"((?:.*\\r?\\n?[^\"]*))\"|NULL)?;(?:\"(.*)\"|NULL)?;(?:\"(.*)\"|NULL)?;(?:\"(.*)\"|NULL)?;(?:\"(.*)\"|NULL)?;(?:\"(.*)\"|NULL)?");
    // string fileContent =  buffer.str();
    // smatch m;
    // while (regex_search (fileContent, m, re)) {
    //     // for(int i = 1; i <= 7; ++i) {
    //         cout << m[1] << endl;
    //     // }

    //     // for (auto result:m) { 
    //     //     cout << result << " ";
    //     //     cout << endl;
    //     // }
        
    //     fileContent = m.suffix().str();
    // }

    FILE *stream = fopen("artigo.csv", "r");
    int id, citations, year;
    char title[900], authors[4096], snippet[4096], date[40];
    
    vector<Article> articles; 
    if(stream != NULL) {
        while(!feof(stream)) {            
            if(!readColumn(stream, "\"%d\";", &id, 0)) {
                cout << "Parser failed: Column title couldn't be parsed" << endl;
                break;
            }
            else cout << id << endl;

            readColumnByChar(stream, title);

            if(!readColumn(stream, "\"%d\";", &year, 0)) {
                year = 0;
            }

            readColumnByChar(stream, authors);

            if(!readColumn(stream, "\"%d\";", &citations, 0)) {
                citations = 0;
            }

            if(!readColumn(stream, "\"%[^\"]\";", date, 0)) {
                date[0] = '\0';
            }

            if(!readColumn(stream, "%[^\n]\n", snippet, 1)) {
                snippet[0] = '\0';
            }
            
            // cout << title << endl;
            // cout << year << endl;
            // cout << authors << endl;
            // cout << citations << endl;
            // cout << date << endl;
            // cout << snippet << endl << endl;

            articles.push_back(Article(id, year, citations, date, title, authors, snippet));
        }
    }
    return 0;
}
