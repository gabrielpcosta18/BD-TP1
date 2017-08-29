#include <btree.hpp>

#define MAX_KEY 510 /* 2n = 510, n = 255, quantidade maxima/ minima de elementos por pagina */
#define BLOCK_SIZE 4096

Btree::Btree(string fileName)
{
    stream = FileInterface(fileName, "rb");
    if(!stream.isOpen())
    {
        stream = FileInterface(fileName, "wb");        
    }
    pages = 0;
    root = 0;
    pointer = -4096;
    Page page;

    stream.write()
}

int Btree::insert(Node node)
{
    int i = 0;
    int pageFather = -1;
    int inPage; /*holds the offset of the page you're currently in*/
    int end_page = 0;
    Page page;
    



}

unsigned long int Btree::search(int ID)
{
    int i = 0;
    int end_tree = 0;
    int end_page = 0;

    Page page(stream.read(BLOCK_SIZE, Btree.root));

    while(!end_tree)
    {
        for(i = 0; i < page.keynumber &&  !end_page; i++)
        {
            if(page.nodes[i].offset > ID)
            {
                if(page.pointer[i] >= 0)
                {
                    page(stream.read(BLOCK_SIZE, page.pointer[i]));
                    end_page = 1;
                } else return -1;
            } else
            {
                if(page.nodes[i].offset == 0) 
                {
                    return (unsigned long int)page.nodes[i].offset;
                }
            }
        }
        if(end_page) {
            end_page = 0;
        }
        else {
            if(page.pointer[i] >= 0) {
                end_page = 0;
                page(stream.read(BLOCK_SIZE, page.pointer[i]));
            } else end_tree = 1;
        }
    }
    return -1;

}

