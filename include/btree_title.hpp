#ifndef _BTREE_TITLE_
#define _BTREE_TITLE_

#include <file_interface.hpp>
#include <string.h>

#define MAX_KEY 12 /* 2n = 12, n = 6, quantidade maxima/ minima de elementos por pagina */
#define BLOCK_SIZE 4096

struct TitleNode
{
    int offset = -1;
    char title[300] = {0};

    TitleNode() {
        offset = -1;
        memset(title, '\0', sizeof(title));
    }

    TitleNode(char ptitle[300], int poffset) {
        offset = poffset;
        strcpy(title, ptitle);
    }
};

class TitlePage
{
    public:
    struct Data 
    {
        unsigned short keyNumber = 0;
        TitleNode nodes[MAX_KEY];
        int pointers[MAX_KEY + 1];
        char empty[394]; //4096 - 3676 
    };

    Data data;
    
    char* toByteArray()
    {
            return reinterpret_cast<char*>(&data);
    }

    TitlePage()
    {
        memset(data.pointers, -1, sizeof(data.pointers));
        for(int i = 0; i < MAX_KEY; ++i) {
            memset(data.nodes[i].title, '\0', sizeof(300));
            data.nodes[i].offset = -1;
        }
        //memset(data.nodes, -1, sizeof(data.nodes));
    }

    TitlePage(char* bytes)
    {
        data = *(reinterpret_cast<Data*>(bytes));
    }

};

class BtreeTitle
{
    public:
    FileInterface stream;
    short pages;
    int root;
    int pointer;  
    
    int search(char title[300]);
    long long int insert(TitleNode node); /* na Btree ordenada por titulo, vai precisar de um int offset */
    long long int splitPage(TitlePage& page, int offsetPage, int father);
    short insertPage(TitlePage& page, TitleNode node);
    BtreeTitle(string fileName);
};

#endif
