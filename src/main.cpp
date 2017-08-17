#include <iostream>
#include <vector>

#include <file_interface.hpp>
#include <article.hpp> 
#include <block_size.hpp>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    
    cout << FileSystemBlockSize::getBlockSize();
  
    FileInterface in("out.csv", "r");
    if(in.isOpen()) {
        while(true) {
            Article article =  in.readArticle();
            cout << article.getData().m_id << " " << article.getData().m_title << " " << article.getData().m_date << endl;
        }    
    }
    else {
        FileInterface  inter("artigo.csv", "r");
        vector<Article> articles = inter.loadRawArticles();
        inter.close();
    
        FileInterface out("out.csv", "wb+");
        for(int i = 0; i < articles.size(); ++i) 
            out.writeArticle(articles[i]);    
    
        out.close();    
    }

    return 0;
}
