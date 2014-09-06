#ifndef TREEWIKIARTICLE_H
#define TREEWIKIARTICLE_H
#include "Tree/treemanager.h"
#include "wikiarticle.h"
#include <map>
using namespace std;
class TreeWikiArticle// : public TreeManager<WikiArticle>
{
public:
    TreeWikiArticle(SQLConnection *db);
    WikiArticle * find(std::string titel);
private:
    SQLConnection* db;
    std::map<string,WikiArticle*> karte;
};

#endif // TREEWIKIARTICLE_H
