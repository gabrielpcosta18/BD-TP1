#ifndef _BTREE_TITLE_
#define _BTREE_TITLE_

#include <file_interface.hpp>

#define MAX_KEY 12 /* 2n = 12, n = 6, quantidade maxima/ minima de elementos por pagina */
#define BLOCK_SIZE 4096

class Node
{
    unsigned int offset = -1;
    char[300] title;
};

class Page
{
    struct Data 
    {
        unsigned short keyNumber = 0;
        Node nodes[MAX_KEY];
        short pointers[MAX_KEY + 1] = {-1};
        unsigned long int pageOffset;
        char empty[420] = {0}; //4096 - 3676 
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
    FileInterface stream;
    short pages;
    int root;
    int pointer;  
    
    unsigned long int search(int ID);
    unsigned long int insert(int ID); /* na Btree ordenada por titulo, vai precisar de um int offset */
    Btree(string fileName);
};

#endif
