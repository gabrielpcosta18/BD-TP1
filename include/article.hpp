#ifndef _ARTICLE_
#define _ARTICLE_

#define SNIPPET_SIZE 1024
#define AUTHORS_SIZE 1024
#define TITLE_SIZE 300
#define DATE_SIZE 20

class Article {
    private:
    struct Data {
        int m_id;
        int m_year;
        int m_citations;
        char m_date[DATE_SIZE];
        char m_title[TITLE_SIZE];
        char m_authors[AUTHORS_SIZE];
        char m_snippet[SNIPPET_SIZE];
    };
    Data data;

    public:
    char* toByteArray() {
        return reinterpret_cast<char*>(&data);
    }

    const Data& getData() {
        return data;
    }

    const int getHashCode() {
        return data.m_id;
    }

    Article(char* bytes) {
        data = *(reinterpret_cast<Data*>(bytes));
    }

    Article(int id, int year, int citations, char date[DATE_SIZE], char title[TITLE_SIZE], char authors[AUTHORS_SIZE], char snippet[SNIPPET_SIZE]) {
        data.m_id = id;
        data.m_year = year;
        data.m_citations = citations;
        
        for(int i = 0; i < AUTHORS_SIZE; ++i) {
            data.m_authors[i] = authors[i];
            data.m_snippet[i] = snippet[i];
        }

        for(int i = 0; i < TITLE_SIZE; ++i) {
            data.m_title[i] = title[i];
        }

        for(int i = 0; i < DATE_SIZE; ++i) {
            data.m_date[i] = date[i];
        }
    }
};

#endif
