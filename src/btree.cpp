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
    pages = 1;
    root = 0;
    pointer = -BLOCK_SIZE;
    Page page;

    
    stream.write(page.data.toByteArray(), BLOCK_SIZE);
}

unsigned long int splitPage(Page page, int offsetPage, int father)
{
    Page newPage;
    Page pageFather;
    int local = 0;
    unsigned long int newPageNumber = 0;
    short middle = 0;
    int i = 0;

    if(father >= 0) {
        pageFather = Page(stream.read(BLOCK_SIZE, father * BLOCK_SIZE));
    }

    middle = (MAX_KEY-1)/2;

    for(i = middle+1; i < MAX_KEY; i++) {

        newpage.data.nodes[i-(middle+1)] = page.data.nodes[i];
        page.data.nodes[i].offset = -1; 
        newpage.data.pointers[i-middle] = page.data.pointers[i+1];
        page.data.pointers[i+1] = -1;
    }
    newpage.data.keyNumber = middle;
    page.data.keyNumber = middle;

    /* Move agora o ponteiro final de pag para nova */
    newpage.data.pointers[0] = page.data.pointers[middle+1];
    page.data.pointers[middle+1] = -1;
    stream.write(newpage.data.toByteArray(), BLOCK_SIZE);
    newPageNumber = newpage.data.

    /* InÌcio do "promoting". */
    /* Agora insere o elemento mediano na p·gina pai. */
    local = pagina_inserir(arv, &pageFather, page.data.nodes[middle]);
    page.data.nodes[middle].CEP[0] = '\0';

    /* Coloca os pointers no pai. */
    pageFather.pointers[local] = pagina;
    pageFather.pointers[local+1] = nova;
    
    /* Atualiza a ·rvore */
    if(pai == -1) arv->paginas += 2; else arv->paginas++;
    pai = pagina_escrever(arv, &pageFather, pai);

    pagina_escrever(arv, pag, pagina);

#ifdef DEBUG
    printf("Split: %d para %d e %d\n", pagina, nova, pai);
#endif

    if(pagina == arv->raiz) {
        arv->raiz = pai;
#ifdef DEBUG
        printf("Atualizando raiz: %d\n", arv->raiz);
#endif
    }

    return pai;
}

short insertPage(Page page, Node node)
{
    int i = 0;
    int j = 0;
    bool finish = false;
    short bestPosition = -1;

    page.data.keynumber++;

    for(i = 0; i < page.data.keynumber && !finish; i++ ) {
        if(page.data.nodes[i].offset > node.offset)  {
            bestPosition = i;
            finish = true;
        }
    }

    if(bestPosition < 0) {
        bestPosition = page.data.keynumber -1;
    }

    j = bestPosition;

    while(page.data.nodes[j].offset != -1) j++;

    while(bestPosition != j) {
        page.data.pointer[j+1] = page.data.pointer[j];
        page.data.pointer[j] = -1;

        page.data.nodes[j] = page.data.nodes[j-1];
        j--;
    }

    page.data.pointer[bestPosition+1] = page.data.pointer[bestPosition];
    page.data.pointer[bestPosition] = -1;
    page.data.nodes[bestPosition] = node;

    return bestPosition;
}

unsigned long int Btree::insert(Node node)
{
    int i = 0;
    int pageFather = -1;
    int currentPage;
    bool end_page = false;

    Page page(stream.read(BLOCK_SIZE, root * BLOCK_SIZE));
    currentPage = page;

    if(currentPage == root && page.data.keynumber == MAX_KEY)
    {
        currentPage = splitPage(page , currentPage, -1);
        page(stream.read(BLOCK_SIZE, root * BLOCK_SIZE));
        pageFather = -1;
    }

    while(true)
    {
        for(i = 0; i <= page.data.keynumber; i++)
        {
            if(i == page.data.keynumber || page.data.nodes[i].offset > node.offset)
            {
                if(page.data.pointer[i] != -1) 
                {
                    pageFather = currentPage;
                    currentPage = page.data.pointer[i];
                    page(stream.read(BLOCK_SIZE, currentPage * BLOCK_SIZE));

                    if(page.data.keynumber == MAX_KEY)
                    {
                        currentPage = splitPage(page ,currentPage, pageFather);
                        page(stream.read(BLOCK_SIZE, currentPage * BLOCK_SIZE));
                        pageFather = -1;
                    }
                    fim_pagina = true;
                }
                else
                {
                    insertPage(page, node);
                    stream.write(page.data.toByteArray(), BLOCK_SIZE, currentPage * BLOCK_SIZE);
                    return currentPage;
                }
            }
        }
        end_page = false;
    }
    return 0;
}

unsigned long int Btree::search(int ID)
{
    int i = 0;
    bool end_tree = false;
    bool end_page = false;

    Page page(stream.read(BLOCK_SIZE, root * BLOCK_SIZE));

    while(!end_tree)
    {
        for(i = 0; i < page.data.keynumber &&  !end_page; i++)
        {
            if(page.data.nodes[i].offset > ID)
            {
                if(page.data.pointer[i] >= 0)
                {
                    page(stream.read(BLOCK_SIZE, page.data.pointer[i]) * BLOCK_SIZE);
                    end_page = true;
                } else return -1;
            } else
            {
                if(page.data.nodes[i].offset == 0) 
                {
                    return (unsigned long int)page.data.nodes[i].offset;
                }
            }
        }
        if(end_page) {
            end_page = false;
        }
        else {
            if(page.data.pointer[i] >= 0) {
                end_page = false;
                page(stream.read(BLOCK_SIZE, page.data.pointer[i]) * BLOCK_SIZE);
            } else end_tree = true;
        }
    }
    return -1;

}

