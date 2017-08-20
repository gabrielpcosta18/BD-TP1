#include <article.hpp>

char* Article::toByteArray() {
    return reinterpret_cast<char*>(&data);
}

const Article::Data& Article::getData() {
    return data;
}

const int Article::getHashCode() {
    return data.m_id;
}

const int Article::getSizeOfData() {
    return sizeof(Article::Data);
}

Article::Article() {}

Article::Article(int id) {
    data.m_id = id;
}

Article::Article(char* bytes) {
    data = *(reinterpret_cast<Article::Data*>(bytes));
}

Article::Article(int id, int year, int citations, char date[DATE_SIZE], char title[TITLE_SIZE], char authors[AUTHORS_SIZE], char snippet[SNIPPET_SIZE]) {
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