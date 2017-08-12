#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <data.hpp> 
#include <sstream>

using namespace std;

int readColumn(FILE *stream, char* columnFormat, void* variable, int isEndline) {
    int read = fscanf(stream, columnFormat, variable);
    if(!read) {
        if(!isEndline) 
            read = fscanf(stream, "%[^;];", variable);
        else read = fscanf(stream, "%[^\n]\n", variable);
        if(!read) {
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

int main() {
    FILE *stream = fopen("artigo.csv", "r");
    int id, citations, year;
    char title[900], authors[4096], snippet[4096], date[40];

    if(stream != NULL) {
        while(true) {
            int read = 0;
            if(!readColumn(stream, "\"%d\";", &id, 0)) {
                cout << "Parser failed: Column title couldn't be parsed" << endl;
                break;
            }

            if(!readColumn(stream, "\"%[^\"]\";", title, 0)) {
                title[0] = '\0';
            }

            if(!readColumn(stream, "\"%d\";", &year, 0)) {
                year = 0;
            }

            if(!readColumn(stream, "\"%[^\"]\";", authors, 0)) {
                authors[0] = '\0';
            }

            if(!readColumn(stream, "\"%d\";", &citations, 0)) {
                citations = 0;
            }

            if(!readColumn(stream, "\"%[^\"]\";", date, 0)) {
                date[0] = '\0';
            }

            if(!readColumn(stream, "%[^\n]\n", snippet, 1)) {
                snippet[0] = '\0';
            }
            
            cout << "REGISTRO" << endl;
            cout << id << endl;
            cout << title << endl;
            cout << year << endl;
            cout << authors << endl;
            cout << citations << endl;
            cout << date << endl;
            cout << snippet << endl << endl;
        }
    }
    return 0;
}
