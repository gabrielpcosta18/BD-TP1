#include <iostream>
#include <vector>

#include <file_interface.hpp>
#include <article.hpp> 

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    
    FileInterface in("out.csv", "r");
    if(in.isOpen()) {
        while(true) {
            Article article = in.readArticle();
            cout << article.getData().m_id << " " << article.getData().m_title << " " << article.getData().m_date << endl;
        }    
    }
    else {
        FileInterface  interface("artigo.csv", "r");
        vector<Article> articles = interface.loadRawArticles();
        interface.close();
    
        FileInterface out("out.csv", "wb+");
        for(int i = 0; i < articles.size(); ++i) 
            out.writeArticle(articles[i]);    
    
        out.close();    
    }

    return 0;
}
