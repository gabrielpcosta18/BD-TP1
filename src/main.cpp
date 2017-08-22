#include <iostream>
#include <vector>

#include <file_interface.hpp>
#include <article.hpp> 
#include <block.hpp>
#include <stdio.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    
    // FileInterface  in("test.csv", "r");
    // FileInterface out("out.csv", "wb+");

    // Article article = in.readRawArticle();

    // while(!in.isEOF()) {    
    //     FileSystemBlock block;
    //     if(block.tryAdd(article.toByteArray(), Article::getSizeOfData())) {
    //         out.write(block.toByteArray(), FileSystemBlock::getBlockSize());
    //         cout << "Sucesso ";
    //     }
    //     else cout << "Fail ";

    //     article = in.readRawArticle();
    // }

    // in.close();
    // out.close();

    FileInterface in("out.csv", "rb");
    
    while(!in.isEOF()) {
        FileSystemBlock block(in.read(FileSystemBlock::getBlockSize()));
        cout << block.getArticle().getData().m_id << endl;
    }

    in.close();

    return 0;
}
