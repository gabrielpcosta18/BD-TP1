#include <iostream>
#include <vector>

#include <file_interface.hpp>
#include <article.hpp> 

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    // FILE *stream = fopen("out.csv", "r");
    // vector<Article> articles;
    // while(!feof(stream)) {
    //     char read[4096];
    //     fread(read, 8, 1, stream);
    //     articles.push_back(Article(read));
    //     cout << articles[articles.size() - 1].getData().m_id << endl;
    // }   

    // FILE *out = fopen("out.csv", "wb+");
    // for(int i = 0; i < articles.size(); ++i) {
    //     cout << articles[i].toByteArray() << '\n';
    //     fwrite(articles[i].toByteArray(), sizeof(articles[i].toByteArray()), 1, out);
    // }
    // fclose(stream);
    // // for(int i = 0; i < articles.size(); ++i) {
    // //     Article a(articles[i].toByteArray());
    // //     cout << a.getData().m_title << endl;
    // // }
    
    FileInterface  interface("artigo.csv", "r");
    vector<Article> articles = interface.loadRawArticles();
    for(int i = 0; i < articles.size(); ++i) 
        cout << articles[i].getData().m_id << endl;

    return 0;
}
