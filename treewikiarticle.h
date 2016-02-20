#ifndef TREEWIKIARTICLE_H
#define TREEWIKIARTICLE_H
//#include "Tree/treemanager.h"
#include "wikiarticle.h"
#include <map>
using namespace std;
class TreeWikiArticle// : public TreeManager<WikiArticle>
{
public:
    TreeWikiArticle();
    WikiArticle * find(std::string titel);
    WikiArticle * find(int id) const;
    void add(string title);
    void save(string filename);
    void load(string filename);
    int* toCL();
    size_t size() const;
    int heighestID();
private:
    std::map<string,WikiArticle*> karte;
    map<int,WikiArticle*> karte_second;
};

#endif // TREEWIKIARTICLE_H
