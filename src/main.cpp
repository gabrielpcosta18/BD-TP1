#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <data.hpp> 
#include <sstream>

using namespace std;

int main() {
    FILE *stream = fopen("artigo.csv", "r");
    int id, citations, year;
    char title[900], authors[4096], snippet[4096], date[40];

    if(stream != NULL) {
        while(true) {
            int read = fscanf(stream, "\"%d\";\"%[^\"]\";\"%d\";\"%[^\"]\";\"%[^\"]\";\"%[^\"]\";\"%[^\"]\"\n", &id, title, &year, authors, &citations, date, snippet);
            if(read < 7) {
                // fscanf(stream, "\"%d\";", &id);
                // fscanf(stream, "\"%[^\"]\";", title);
                // fscanf(stream, "\"%d\";", &year);
                // fscanf(stream, "\"%[^\"]\";", authors);
                // fscanf(stream, "\"%d\";", &citations);
                // fscanf(stream, "\"%[^\"]\";", date);
                // fscanf(stream, "%[^\n]\n", snippet);
                read = fscanf(stream, "\"%d\";\"%[^\"]\";\"%d\";\"%[^\"]\";\"%d\";\"%[^\"]\";", &id, title, &year, authors, &citations, date);
                read += fscanf(stream, "%[^\n]\n", snippet);
                cout << id << endl;
                cout << title << endl;
                cout << year << endl;
                cout << authors << endl;
                cout << citations << endl;
                cout << date << endl;
                cout << snippet << endl;
            }

            if(read == 0) 
                break;

            cout << id << endl;
            // cout << title << endl;
            // cout << year << endl;
            // cout << authors << endl;
            // cout << citations << endl;
            // cout << date << endl;
            // cout << snippet << endl;
        }
    }
    return 0;
}
