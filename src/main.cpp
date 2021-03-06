#include <iostream>
#include <vector>
#include <unistd.h>
#include <file_interface.hpp>
#include <article.hpp> 
#include <block.hpp>
#include <stdio.h>
#include <btree.hpp>
#include <btree_title.hpp>

using namespace std;

void clearScreen()
{
    cout << string( 100, '\n' );
}

void printArticle(Article article) {
    cout << "  - ID: " << article.getData().m_id << endl;
    cout << "  - TITLE: " << article.getData().m_title << endl;
    cout << "  - YEAR: " << article.getData().m_year << endl;
    cout << "  - CITATIONS: " << article.getData().m_citations << endl;
    cout << "  - DATE: " << article.getData().m_date << endl;
    cout << "  - AUTHORS: " << article.getData().m_authors << endl;
    cout << "  - SNIPPET: " << article.getData().m_snippet << endl;
}

int uploadFile() {
    cout << "Digite o caminho para o arquivo a ser importado" << endl;
    cout << "Se o arquivo estiver na pasta do projeto, então basta digitar o nome" << endl;
    cout << ">> ";

    string filepath = "artigo.csv";
    cin >> filepath;

    FileInterface in(filepath, "r");

    if(in.isOpen()) {
        FileInterface out("data", "wb+");

        Btree tree("primaryIndex");
        BtreeTitle titleTree("secondaryIndex");

        cout << "Importando dados. Aguarde" << endl;

        while(!in.isEOF()) {    
            Article article = in.readRawArticle();
            unsigned long long int blockSize = FileSystemBlock::getBlockSize();

            FileSystemBlock block;
            if(block.tryAdd(article.toByteArray(), Article::getSizeOfData())) {
                out.write(block.toByteArray(), FileSystemBlock::getBlockSize(), 
                    blockSize * (article.getData().m_id - 1));

                tree.insert(Node(article.getData().m_id));
                titleTree.insert(TitleNode((char*)article.getData().m_title, article.getData().m_id));
            }
        }

        in.close();
        out.close();
        
        cout << "Arquivos gerados com sucesso" << endl;
        return 0;
    }
    else {
        cout << "Falha na operação de leitura." << endl;
        cout << "Arquivo para upload não foi encontrado." << endl;

        return 1;
    }
}

int findRec() {
    cout << "Digite o ID a ser pesquisado" << endl;
    cout << ">> ";

    int id = 1600000;
    cin.ignore();
    cin >> id;

    FileInterface in("data", "rb");
    if(!in.isOpen()) {
        cout << "Arquivo de dados não encontrado. Faça o upload" << endl;
        return 1;
    }

    FileSystemBlock block(in.read(FileSystemBlock::getBlockSize(),
        (id - 1)*FileSystemBlock::getBlockSize()));

    Article article = block.getArticle(); 
    if(article.getData().m_id != 1  || id == 1) {
        cout << "Statistics" << endl;
        cout << "  - Read blocks: " << 1 << endl;
        cout << "  - Total blocks: " <<  in.getFileSize()/FileSystemBlock::getBlockSize() << endl << endl;
        
        cout << "Data recovered" << endl;
        printArticle(block.getArticle());
        cout << endl;

        in.close();
        return 0;
    }
    else {
        cout << "Não existe artigo com esse código no arquivo de dados" << endl;
        return 1;
    }
}

int seekID() {
    cout << "Digite o ID a ser pesquisado" << endl;
    cout << ">> ";

    int id;
    cin >> id;

    Btree tree("primaryIndex");
    int offset = tree.search(id);

    if(offset > 0) {
        FileInterface in("data", "rb");
        if(!in.isOpen()) {
            cout << "Arquivo de dados não encontrado. Faça o upload" << endl;
            return 1;
        }

        FileSystemBlock block(in.read(FileSystemBlock::getBlockSize(),
            (offset - 1)*FileSystemBlock::getBlockSize()));

        cout << "Data recovered" << endl;
        printArticle(block.getArticle());
        cout << endl;        
    }
    else {
        cout << "Não existe artigo com esse código no arquivo de dados" << endl;
        return 1;
    }
}

int seekTitle() {
    cout << "Digite o Título a ser pesquisado" << endl;
    cout << ">> ";

    char* title = new char[300];
    fflush(stdin);
    cin.ignore();
    cin.getline(title, 300);

    BtreeTitle tree("secondaryIndex");
    int offset = tree.search(title);
 
    if(offset > 0) {
        cout << offset << endl;
        FileInterface in("data", "rb");
        if(!in.isOpen()) {
            cout << "Arquivo de dados não encontrado. Faça o upload" << endl;
            return 1;
        }

        FileSystemBlock block(in.read(FileSystemBlock::getBlockSize(),
            (offset - 1)*FileSystemBlock::getBlockSize()));
        printArticle(block.getArticle());
    }
    else {
        cout << "Não existe artigo com esse título no arquivo de dados" << endl;
        return 1;
    }
}

int main() {
    ios::sync_with_stdio(true);
    
    while(true) {
        cout << "Opções: 1 - UploadFile; 2 - FindRec; 3 - SeekID; 4 - SeekTitle; 5 ou outros - Exit" << endl;
        cout << ">> ";

        int option = 2;
        fflush(stdin);
        cin >> option;
        
        clearScreen();
        int opResult = 0;
        switch(option) {
            case 1:
                opResult = uploadFile();
                break;
            case 2:
                opResult = findRec();
                break;
            case 3:
                opResult = seekID();
                break;
            case 4:
                opResult = seekTitle();
                break;
            default:
                return 0;
        }

        cout << endl << endl;
    }

    return 0;
}
