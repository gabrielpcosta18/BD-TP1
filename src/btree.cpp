#include <btree.hpp>
#include <iostream>

using namespace std;

#define MAX_KEY 680 /* 2n = 510, n = 255, quantidade maxima/ minima de elementos por pagina */
#define BLOCK_SIZE 4096

Btree::Btree(string fileName)
{
    stream = FileInterface(fileName, "rb");
    if(!stream.isOpen())
    {
        stream = FileInterface(fileName, "wb+");
        root = 1;
        stream.write(reinterpret_cast<char*>(&root), BLOCK_SIZE);
        Page page = Page();
        stream.write(page.toByteArray(), BLOCK_SIZE, root * BLOCK_SIZE);        
    }
    else {
        char *read = stream.read(BLOCK_SIZE);
        root = *(reinterpret_cast<int*>(read));
        delete[] read;
    }

    pages = 1;
    pointer = 0;
}

short insertPage(Page& page, Node node)
{
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
    if(bestPosition < 0) {
        bestPosition = page.data.keyNumber - 1;
    }

    j = bestPosition;
    while(page.data.nodes[j].offset != -1) j++;

    while(bestPosition != j) {
        page.data.pointers[j + 1] = page.data.pointers[j];
        page.data.pointers[j] = -1;

        page.data.nodes[j] = page.data.nodes[j-1];
        j--;
    }

    page.data.pointers[bestPosition + 1] = page.data.pointers[bestPosition];
    page.data.pointers[bestPosition] = -1;
    page.data.nodes[bestPosition] = node;

    return bestPosition;
}


long long int Btree::splitPage(Page& page, int offsetPage, int father)
{
    Page newPage;
    Page pageFather;
    long long int local = 0;
    long long int newPageNumber =  0;
    short middle = 0;
    int i = 0;

    if(father >= 1) {
        pageFather = Page(stream.read(BLOCK_SIZE, (unsigned long long int) father * (unsigned long long int) BLOCK_SIZE));
        if(pageFather.data.pointers[MAX_KEY] == 0) pageFather.data.pointers[MAX_KEY] = -1;
    }

    middle = (MAX_KEY - 1)/2;

    for(i = middle + 1; i < MAX_KEY; ++i) {
        newPage.data.nodes[i -(middle + 1)] = page.data.nodes[i];
        page.data.nodes[i].offset = -1; 
        newPage.data.pointers[i - middle] = page.data.pointers[i + 1];
        page.data.pointers[i + 1] = -1;
    }
    newPage.data.keyNumber = middle + 1;
    page.data.keyNumber = middle;

    /* Move agora o ponteiro final de pag para nova */
    newPage.data.pointers[0] = page.data.pointers[middle + 1];
    page.data.pointers[middle + 1] = -1;
    
    // Equivalente ao página_escrever com último parâmetro igual a -1
    pointer += BLOCK_SIZE;
    stream.write(newPage.toByteArray(), BLOCK_SIZE);
    newPageNumber = pointer / BLOCK_SIZE;

    // Equivalente ao página inserir
    local = insertPage(pageFather, page.data.nodes[middle]); // stream.write(pageFather.toByteArray(), BLOCK_SIZE, local);
    page.data.nodes[middle].offset = -1;

    /* Coloca os pointers no pai. */
    pageFather.data.pointers[local] = offsetPage;
    pageFather.data.pointers[local+1] = newPageNumber + 1;
    
    /* Atualiza a ·rvore */
    if(father == -1) {
        pages += 2;
        pointer += BLOCK_SIZE;
        father = pointer / BLOCK_SIZE + 1;
    }
    else pages++;
    // pai = pagina_escrever(arv, &pageFather, pai);

    // pagina_escrever(arv, pag, pagina);
    stream.write(pageFather.toByteArray(), BLOCK_SIZE, (unsigned long long int) BLOCK_SIZE * (unsigned long long int) father);
    stream.write(page.toByteArray(), BLOCK_SIZE, (unsigned long long int) BLOCK_SIZE * (unsigned long long int) offsetPage);

    if(offsetPage == root) {
        root = father;
        stream.write(reinterpret_cast<char*>(&root), BLOCK_SIZE, 0);
    }

    return father;
}

long long int Btree::insert(Node node)
{
    int i = 0;
    int pageFather = -1;
    int currentPage;
    bool endPage = false;

    Page page(stream.read(BLOCK_SIZE, root * BLOCK_SIZE));
    currentPage = root;

    if(page.data.pointers[MAX_KEY] == 0) page.data.pointers[MAX_KEY] = -1;
    if(currentPage == root && page.data.keyNumber == MAX_KEY)
    {
        currentPage = splitPage(page , currentPage, -1);
        page = Page(stream.read(BLOCK_SIZE, (unsigned long long int) root *  (unsigned long long int) BLOCK_SIZE));
        pageFather = -1;
        if(page.data.pointers[MAX_KEY] == 0) page.data.pointers[MAX_KEY] = -1;
    }

    while(true)
    {
        for(i = 0; i <= page.data.keyNumber && !endPage; i++)
        {
            if(i == page.data.keyNumber || page.data.nodes[i].offset > node.offset)
            { 
                if(page.data.pointers[i] != -1) 
                {
                    pageFather = currentPage;
                    currentPage = page.data.pointers[i];
                    page = Page(stream.read(BLOCK_SIZE, (unsigned long long int) currentPage *  (unsigned long long int) BLOCK_SIZE));
                    
                    if(page.data.pointers[MAX_KEY] == 0) page.data.pointers[MAX_KEY] = -1;
                    if(page.data.keyNumber == MAX_KEY)
                    {
                        currentPage = splitPage(page ,currentPage, pageFather);
                        page = Page(stream.read(BLOCK_SIZE, (unsigned long long int) currentPage * (unsigned long long int) BLOCK_SIZE));
                        pageFather = -1;
                        if(page.data.pointers[MAX_KEY] == 0) page.data.pointers[MAX_KEY] = -1;
                    }
                    endPage = true;
                }
                else
                {
                    insertPage(page, node);
                    stream.write(page.toByteArray(), BLOCK_SIZE, (unsigned long long int) currentPage * (unsigned long long int) BLOCK_SIZE);
                    return currentPage;
                }
            }
        }

        endPage = false;
    }
    return 0;
}

int Btree::search(int ID)
{
    int i = 0;
    bool endTree = false;
    bool endPage = false;
    int pagesLoaded = 0;

    Page page(stream.read(BLOCK_SIZE, (unsigned long long int) root * (unsigned long long int) BLOCK_SIZE));
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
                if(page.data.pointers[i] >= 1)
                {
                    page = Page(stream.read(BLOCK_SIZE, (unsigned long long int) page.data.pointers[i] * (unsigned long long int) BLOCK_SIZE));
                    endPage = true;
                    pagesLoaded++;
                } else {
                    // cout << "retornar -1 " << endl;
                    return -1;
                }
            } else
            {
                if(page.data.nodes[i].offset == ID) 
                {
                    cout << "Statistics" << endl;
                    cout << "  - Read Pages " << pagesLoaded << endl;
                    cout << "  - Total blocks: " <<  stream.getFileSize()/FileSystemBlock::getBlockSize() << endl << endl; 
                    return (int)page.data.nodes[i].offset;
                }
            }
        }
        if(endPage) {
            endPage = false;
        }
        else {
            if(page.data.pointers[i] >= 1) {
                endPage = false;
                pagesLoaded++;
                page = Page(stream.read(BLOCK_SIZE, (unsigned long long int) page.data.pointers[i] * (unsigned long long int) BLOCK_SIZE));
            } else endTree = true;
        }
    }

    return -1;
}

