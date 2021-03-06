#ifndef _BTREE_
#define _BTREE_

#include <file_interface.hpp>

#define MAX_KEY 680 /* 2n = 682, n = 341, quantidade maxima/ minima de elementos por pagina */
#define MAX_POINTER 681
#define BLOCK_SIZE 4096

struct Node
{
    int offset = -1;

    Node() {}
    Node(int poffset) {
        offset = poffset;
    }
};

class Page
{
    public: 
    struct Data 
    {
        unsigned short keyNumber = 0;
        Node nodes[MAX_KEY];
        short pointers[MAX_POINTER];
        char empty[10];
    };

    Data data;

    char* toByteArray()
    {
         return reinterpret_cast<char*>(&data);
    }

    Page()
    {
        memset(data.pointers, -1, sizeof(data.pointers));
        memset(data.nodes, -1, sizeof(data.nodes));
    }

    Page(char* bytes)
    {
        data = *(reinterpret_cast<Data*>(bytes));
        delete[] bytes;
    }

};

class Btree
{
    public:
    FileInterface stream;
    short pages;
    int root;
    int pointer;  
    
    int search(int ID);
    long long int insert(Node node); /* na Btree ordenada por titulo, vai precisar de um int offset */
    long long int splitPage(Page& page, int offsetPage, int father);
    Btree(string fileName);
};

#endif
