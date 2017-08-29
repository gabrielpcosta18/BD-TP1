#ifndef _BTREE_
#define _BTREE_

#include <file_interface.hpp>

#define MAX_KEY 510 /* 2n = 510, n = 255, quantidade maxima/ minima de elementos por pagina */
#define BLOCK_SIZE 4096

class Node
{
    unsigned int offset = 0;
};

class Page
{
    struct Data 
    {
        unsigned short keyNumber = 0;
        Node nodes[MAX_KEY];
        short pointers[MAX_KEY + 1] = {0};
        char empty[10] = {0};
    };

    Data data;

    char* toByteArray();
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
    
    int search(int ID);
    void insert(int ID); /* na Btree ordenada por titulo, vai precisar de um int offset */
    Btree(string fileName);
};

#endif