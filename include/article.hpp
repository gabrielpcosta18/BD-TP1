#ifndef _ARTICLE_
#define _ARTICLE_

class Article {
    private:
    struct Data {
        int m_id;
        int m_year;
        int m_citations;
        char m_title[300];
        char m_authors[1024];
        char m_snippet[1024];
    };
    Data data;

    public:
    char* toByteArray() {
        return reinterpret_cast<char*>(&data);
    }

    const Data& getData() {
        return data;
    }

    Article(char* bytes) {
        data = *(reinterpret_cast<Data*>(bytes));
    }

    Article(int id, int year, int citations, char date[20], char title[300], char authors[1024], char snippet[1024]) {
        data.m_id = id;
        data.m_year = year;
        data.m_citations = citations;
        
        for(int i = 0; i < 1024; ++i) {
            data.m_authors[i] = authors[i];
            data.m_snippet[i] = snippet[i];
        }

        for(int i = 0; i < 300; ++i) {
            data.m_title[i] = title[i];
        }

        //data.m_title = title;
    }
};

#endif
