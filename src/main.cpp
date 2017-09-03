#include <iostream>
#include <vector>
#include <unistd.h>
#include <file_interface.hpp>
#include <article.hpp> 
#include <block.hpp>
#include <stdio.h>
#include <btree.hpp>

using namespace std;

void clearScreen()
{
    cout << string( 100, '\n' );
}

int uploadFile() {
    cout << "Digite o caminho para o arquivo a ser importado" << endl;
    cout << "Se o arquivo estiver na pasta do projeto, então basta digitar o nome" << endl;
    cout << ">> ";

    string filepath;
    cin >> filepath;

    FileInterface in(filepath, "r");

    if(in.isOpen()) {
        FileInterface out("data", "wb+");

        Btree tree("primaryIndex");
        while(!in.isEOF()) {    
            Article article = in.readRawArticle();
            unsigned long long int blockSize = FileSystemBlock::getBlockSize();

            FileSystemBlock block;
            if(block.tryAdd(article.toByteArray(), Article::getSizeOfData())) {
                out.write(block.toByteArray(), FileSystemBlock::getBlockSize(), 
                    blockSize * (article.getData().m_id - 1));
                tree.insert(Node(article.getData().m_id));
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

    int id;
    cin >> id;

    FileInterface in("data", "rb");
    FileSystemBlock block(in.read(FileSystemBlock::getBlockSize(),
        (id - 1)*FileSystemBlock::getBlockSize()));

    Article article = block.getArticle(); 
    if(article.getData().m_id > 0) {
        cout << block.getArticle().getData().m_title << endl;
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
        FileSystemBlock block(in.read(FileSystemBlock::getBlockSize(),
            (offset - 1)*FileSystemBlock::getBlockSize()));
        cout << block.getArticle().getData().m_title << endl;
    }
    else {
        cout << "Não existe artigo com esse código no arquivo de dados" << endl;
        return 1;
    }
}

int seekTitle() {

}

int main() {
    ios::sync_with_stdio(false);
    
    while(true) {
        cout << "Opções: 1 - UploadFile; 2 - FindRec; 3 - SeekID; 4 - SeekTitle; 5 ou outros - Exit" << endl;
        cout << ">> ";

        int option;
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

    // FileInterface in("artigo.csv", "r");
    // FileInterface out("data", "wb+");

    Btree tree("primaryIndex");
    // tree.insert(Node(1));
    cout << tree.search(2623000);
    // while(!in.isEOF()) {    
    //     Article article = in.readRawArticle();
    //     unsigned long long int blockSize = FileSystemBlock::getBlockSize();

    //     FileSystemBlock block;
    //     if(block.tryAdd(article.toByteArray(), Article::getSizeOfData())) {
    //         out.write(block.toByteArray(), FileSystemBlock::getBlockSize(), 
    //             blockSize * (article.getData().m_id - 1));
    //         // cout << "Inserir nó com id = " << article.getData().m_id - 1 << endl; 
    //         tree.insert(Node(article.getData().m_id));
    //     }
    // }

    // in.close();
    // out.close();
    
    // int a[2] = {-1};
    // memset(a, -1, sizeof(a));
    // cout << a[1];
    // // FileInterface inRead("data", "rb");
    // unsigned long long int i = 0;
    // while(!inRead.isEOF()) {
    //     FileSystemBlock block(inRead.read(FileSystemBlock::getBlockSize(), i*FileSystemBlock::getBlockSize()));
    //     if(block.getArticle().getData().m_id != 0)
    //         cout << block.getArticle().getData().m_id << endl;
    //     i++;
    // }

    // inRead.close();

    return 0;
}
