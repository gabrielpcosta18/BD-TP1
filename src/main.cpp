#include <iostream>
#include <vector>

#include <file_interface.hpp>
#include <article.hpp> 
#include <block.hpp>
#include <stdio.h>
#include <btree.hpp>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    
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
