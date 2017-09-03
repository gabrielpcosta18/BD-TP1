#include <btree.hpp>
#include <iostream>

using namespace std;

#define MAX_KEY 682 /* 2n = 510, n = 255, quantidade maxima/ minima de elementos por pagina */
#define BLOCK_SIZE 4096

Btree::Btree(string fileName)
{
    stream = FileInterface(fileName, "rb");
    if(!stream.isOpen())
    {
        stream = FileInterface(fileName, "wb+");
        Page page = Page();
        stream.write(page.toByteArray(), BLOCK_SIZE, 0);        
    }

    pages = 1;
    root = 0;
    pointer = -BLOCK_SIZE;
}

short insertPage(Page& page, Node node)
{
    // cout << "FUNCTION: insertPage" << endl;
    int i = 0;
    int j = 0;
    bool finish = false;
    short bestPosition = -1;

    page.data.keyNumber++;

    for(i = 0; i < page.data.keyNumber && !finish; ++i) {
        if(page.data.nodes[i].offset > node.offset)  {
            bestPosition = i;
            finish = true;
        }
    }
    // cout << "BEST POSITION = " << bestPosition << endl;
    if(bestPosition < 0) {
        bestPosition = page.data.keyNumber - 1;
    }

    j = bestPosition;
    // cout << "page.data.nodes[j].offset = " << page.data.nodes[0].offset << endl;  
    while(page.data.nodes[j].offset != -1) j++;

    // cout << "J = " << j << endl;
    while(bestPosition != j) {
        page.data.pointers[j + 1] = page.data.pointers[j];
        page.data.pointers[j] = -1;

        page.data.nodes[j] = page.data.nodes[j-1];
        j--;
    }

    page.data.pointers[bestPosition + 1] = page.data.pointers[bestPosition];
    page.data.pointers[bestPosition] = -1;
    page.data.nodes[bestPosition] = node;

    // cout << "bestPosition = " << bestPosition << endl; 
    // cout << "page.data.nodes[bestPosition].offset = " << page.data.nodes[bestPosition].offset << endl;

    return bestPosition;
}


unsigned long int Btree::splitPage(Page& page, int offsetPage, int father)
{
    Page newPage;
    Page pageFather;
    unsigned long int local = 0;
    unsigned long int newPageNumber =  0;
    short middle = 0;
    int i = 0;

    if(father >= 0) {
        pageFather = Page(stream.read(BLOCK_SIZE, father * BLOCK_SIZE));
    }

    middle = (MAX_KEY - 1)/2;

    for(i = middle + 1; i < MAX_KEY; ++i) {
        newPage.data.nodes[i -(middle + 1)] = page.data.nodes[i];
        page.data.nodes[i].offset = -1; 
        newPage.data.pointers[i - middle] = page.data.pointers[i + 1];
        page.data.pointers[i + 1] = -1;
    }
    newPage.data.keyNumber = middle;
    page.data.keyNumber = middle;

    /* Move agora o ponteiro final de pag para nova */
    newPage.data.pointers[0] = page.data.pointers[middle + 1];
    page.data.pointers[middle + 1] = -1;
    
    // Equivalente ao página_escrever com último parâmetro igual a -1
    pointer += BLOCK_SIZE;
    stream.write(newPage.toByteArray(), BLOCK_SIZE);
    newPageNumber = pointer / BLOCK_SIZE;

    // Equivalente ao página inserir
    //local = page.data.nodes[middle].offset * BLOCK_SIZE;
    local = insertPage(pageFather, page.data.nodes[middle]); // stream.write(pageFather.toByteArray(), BLOCK_SIZE, local);
    page.data.nodes[middle].offset = -1;

    /* Coloca os pointers no pai. */
    pageFather.data.pointers[local] = offsetPage;
    pageFather.data.pointers[local+1] = newPageNumber;
    
    /* Atualiza a ·rvore */
    if(father == -1) pages += 2; else pages++;
    // pai = pagina_escrever(arv, &pageFather, pai);

    // pagina_escrever(arv, pag, pagina);
    stream.write(pageFather.toByteArray(), BLOCK_SIZE, BLOCK_SIZE * father);
    stream.write(page.toByteArray(), BLOCK_SIZE, BLOCK_SIZE * offsetPage);

    if(page.data.keyNumber == root) {
        root = father;
    }

    return father;
}

unsigned long int Btree::insert(Node node)
{
    // cout << "FUNCTION: insert" << endl;
    int i = 0;
    int pageFather = -1;
    int currentPage;
    bool endPage = false;

    Page page(stream.read(BLOCK_SIZE, root * BLOCK_SIZE));
    currentPage = root;
    // cout << "CURRENT PAGE = ROOT = " << root << endl;
    // cout << "page.data.keyNumber = " << page.data.keyNumber << endl;
    if(currentPage == root && page.data.keyNumber == MAX_KEY)
    {
        // cout << "AQUI" << endl;
        currentPage = splitPage(page , currentPage, -1);
        page = Page(stream.read(BLOCK_SIZE, root * BLOCK_SIZE));
        pageFather = -1;
    }

    while(true)
    {
        for(i = 0; i <= page.data.keyNumber && !endPage; i++)
        {
            if(i == page.data.keyNumber || page.data.nodes[i].offset > node.offset)
            { 
                // cout << "page.data.pointers[i] = " << page.data.pointers[i] << endl;
                if(page.data.pointers[i] != -1) 
                {
                    pageFather = currentPage;
                    currentPage = page.data.pointers[i];
                    page = Page(stream.read(BLOCK_SIZE, currentPage * BLOCK_SIZE));

                    if(page.data.keyNumber == MAX_KEY)
                    {
                        currentPage = splitPage(page ,currentPage, pageFather);
                        page = Page(stream.read(BLOCK_SIZE, currentPage * BLOCK_SIZE));
                        pageFather = -1;
                    }
                    endPage = true;
                }
                else
                {
                    // cout << "INSERT PAGE" << endl;
                    insertPage(page, node);
                    // cout << "page.data.keyNumber = " << page.data.keyNumber << endl;
                    // cout << "page.data.pointers[i] = " << page.data.pointers[i] << endl;
                    stream.write(page.toByteArray(), BLOCK_SIZE, currentPage * BLOCK_SIZE);
                    return currentPage;
                }
            }
        }
        // cout << endl;
        endPage = false;
    }
    return 0;
}

int Btree::search(int ID)
{
    int i = 0;
    bool endTree = false;
    bool endPage = false;

    Page page(stream.read(BLOCK_SIZE, root * BLOCK_SIZE));
    // cout << "CURRENT PAGE = ROOT = " << root << endl;
    // cout << "page.data.keyNumber = " << page.data.keyNumber << endl;
    while(!endTree)
    {
        for(i = 0; i < page.data.keyNumber &&  !endPage; ++i)
        {
            // cout << "page.data.nodes[i].offset = " << page.data.nodes[i].offset << endl;
            if(page.data.nodes[i].offset > ID)
            {
                // cout << "page.data.pointers[i] = " << page.data.pointers[i] << endl;
                if(page.data.pointers[i] >= 0)
                {
                    page = Page(stream.read(BLOCK_SIZE, page.data.pointers[i] * BLOCK_SIZE));
                    endPage = true;
                } else {
                    // cout << "retornar -1 " << endl;
                    return -1;
                }
            } else
            {
                if(page.data.nodes[i].offset == ID) 
                {
                    return (int)page.data.nodes[i].offset;
                }
            }
        }
        if(endPage) {
            endPage = false;
        }
        else {
            if(page.data.pointers[i] >= 0) {
                endPage = false;
                page = Page(stream.read(BLOCK_SIZE, page.data.pointers[i] * BLOCK_SIZE));
            } else endTree = true;
        }
    }
    return -1;

}

