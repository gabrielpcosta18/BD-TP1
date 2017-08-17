#ifndef _ARTICLE_
#define _ARTICLE_

#define SNIPPET_SIZE 1024
#define AUTHORS_SIZE 1024
#define TITLE_SIZE 300
#define DATE_SIZE 20
#define BYTES_SIZE 2380

class Article {
    public:
    struct Data {
        int m_id;
        int m_year;
        int m_citations;
        char m_date[DATE_SIZE];
        char m_title[TITLE_SIZE];
        char m_authors[AUTHORS_SIZE];
        char m_snippet[SNIPPET_SIZE];
    };
    char* toByteArray();
    const Data& getData();
    const int getHashCode();
    static const int getSizeOfData();
    Article();
    Article(char* bytes);
    Article(int id, int year, int citations, char date[DATE_SIZE], char title[TITLE_SIZE], char authors[AUTHORS_SIZE], char snippet[SNIPPET_SIZE]);

    private:
    Data data;
};

#endif
