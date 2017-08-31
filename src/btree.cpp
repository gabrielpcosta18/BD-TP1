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
    
    stream.write(Page().toByteArray(), BLOCK_SIZE);
}

short insertPage(Page page, Node node)
{
    int i = 0;
    int j = 0;
    bool finish = false;
    short bestPosition = -1;

    page.data.keyNumber++;

    for(i = 0; i < page.data.keyNumber && !finish; i++ ) {
        if(page.data.nodes[i].offset > node.offset)  {
            bestPosition = i;
            finish = true;
        }
    }

    if(bestPosition < 0) {
        bestPosition = page.data.keyNumber -1;
    }

    j = bestPosition;

    while(page.data.nodes[j].offset != -1) j++;

    while(bestPosition != j) {
        page.data.pointers[j+1] = page.data.pointers[j];
        page.data.pointers[j] = -1;

        page.data.nodes[j] = page.data.nodes[j-1];
        j--;
    }

    page.data.pointers[bestPosition+1] = page.data.pointers[bestPosition];
    page.data.pointers[bestPosition] = -1;
    page.data.nodes[bestPosition] = node;

    return bestPosition;
}


unsigned long int Btree::splitPage(Page page, int offsetPage, int father)
{
    Page newPage;
    Page pageFather;
    unsigned long int local = 0;
    unsigned long int newPageNumber = 0;
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
#ifdef DEBUG
    printf("Split: %d para %d e %d\n", pagina, nova, pai);
#endif

    if(page.data.keyNumber == root) {
        root = father;
#ifdef DEBUG
        printf("Atualizando raiz: %d\n", arv->raiz);
#endif
    }

    return father;
}

unsigned long int Btree::insert(Node node)
{
    int i = 0;
    int pageFather = -1;
    int currentPage;
    bool end_page = false;

    Page page(stream.read(BLOCK_SIZE, root * BLOCK_SIZE));
    currentPage = page.data.keyNumber;

    if(currentPage == root && page.data.keyNumber == MAX_KEY)
    {
        currentPage = splitPage(page , currentPage, -1);
        page = Page(stream.read(BLOCK_SIZE, root * BLOCK_SIZE));
        pageFather = -1;
    }

    while(true)
    {
        for(i = 0; i <= page.data.keyNumber; i++)
        {
            if(i == page.data.keyNumber || page.data.nodes[i].offset > node.offset)
            {
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
                    end_page = true;
                }
                else
                {
                    insertPage(page, node);
                    stream.write(page.toByteArray(), BLOCK_SIZE, currentPage * BLOCK_SIZE);
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
        for(i = 0; i < page.data.keyNumber &&  !end_page; i++)
        {
            if(page.data.nodes[i].offset > ID)
            {
                if(page.data.pointers[i] >= 0)
                {
                    page = Page(stream.read(BLOCK_SIZE, page.data.pointers[i] * BLOCK_SIZE));
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
            if(page.data.pointers[i] >= 0) {
                end_page = false;
                page = Page(stream.read(BLOCK_SIZE, page.data.pointers[i] * BLOCK_SIZE));
            } else end_tree = true;
        }
    }
    return -1;

}

