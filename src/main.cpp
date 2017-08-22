#include <iostream>
#include <vector>

#include <file_interface.hpp>
#include <article.hpp> 
#include <block.hpp>
#include <stdio.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    
    FileInterface  inter("test.csv", "r");
    FileInterface out("out.csv", "wb+");

    Article article = inter.readRawArticle();
    article = inter.readRawArticle();
    while(!inter.isEOF()) {    
        FileSystemBlock block;
        if(block.tryAdd(article)) {
            cout <<  article.getData().m_title << endl;
            cout << FileSystemBlock(block.toByteArray()).getArticles().getData().m_id << endl;
            // out.write(*block);
        }
        else cout << "wsat";

        article = inter.readRawArticle();
    }

    inter.close();
    out.close();

    // FileInterface in("out.csv", "rb");
    
    // while(!in.isEOF()) {
    //     in.read();
    //     // FileSystemBlock* block = new FileSystemBlock(in.read());
    //     // cout << block->getArticles().getData().m_id << endl;
    // }

    return 0;
}
