#include <iostream>
#include <article.hpp> 

using namespace std;

int readColumn(FILE *stream, char* columnFormat, void* variable, int isEndline) {
    int read = fscanf(stream, columnFormat, variable);
    if(!read) {
        if(!isEndline) 
            read = fscanf(stream, "%[^;];", variable);
        else read = fscanf(stream, "%[^\n]\n", variable);
        if(read >= 0) {
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
    char title[300], authors[1024], snippet[1024], date[20];
    
    if(stream != NULL) {
        while(true) {
            int read = 0;
            if(!readColumn(stream, "\"%d\";", &id, 0)) {
                cout << "Parser failed: Column title couldn't be parsed" << endl;
                break;
            }

            if(!readColumn(stream, "\"%[^\";]\";", title, 0)) {
                title[0] = '\0';
            }

            if(!readColumn(stream, "\"%d\";", &year, 0)) {
                year = 0;
            }

            if(!readColumn(stream, "\"%[^\";]\";", authors, 0)) {
                authors[0] = '\0';
            }

            if(!readColumn(stream, "\"%d\";", &citations, 0)) {
                citations = 0;
            }

            if(!readColumn(stream, "\"%[^\";]\";", date, 0)) {
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

            Article article(id, year, citations, date, title, authors, snippet);
            char* c =  article.toByteArray();
            cout << sizeof(c) << "   " << sizeof(article.getData()) << endl;
            
            Article article2  = Article(c);
            cout << article2.getData().m_title << endl;
            break;
        }
    }
    return 0;
}
