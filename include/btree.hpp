#ifndef _BTREE_
#define _BTREE_

#include <file_interface.hpp>

#define MAX_KEY 682 /* 2n = 682, n = 341, quantidade maxima/ minima de elementos por pagina */
#define BLOCK_SIZE 4096

class Node
{
    public:
    unsigned int offset = -1;
};

class Page
{
    public: 
    struct Data 
    {
        unsigned short keyNumber = 0;
        Node nodes[MAX_KEY];
        short pointers[MAX_KEY + 1] = {-1};
        // char empty[10] = {0};
    };

    Data data;

    char* toByteArray()
    {
         return reinterpret_cast<char*>(&data);
    }

    Page()
    {
    }

    Page(char* bytes)
    {
        data = *(reinterpret_cast<Data*>(bytes));
    }

};

class Btree
{
    public:
    FileInterface stream;
    short pages;
    int root;
    int pointer;  
    
    unsigned long int search(int ID);
    unsigned long int insert(Node node); /* na Btree ordenada por titulo, vai precisar de um int offset */
    unsigned long int splitPage(Page page, int offsetPage, int father);
    Btree(string fileName);
};

#endif
