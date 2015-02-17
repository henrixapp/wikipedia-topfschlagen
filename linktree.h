#ifndef LINKTREE_H
#define LINKTREE_H
#include <vector>
#include "wikilink.h"
#include "treewikiarticle.h"
using namespace std;
class LinkTree
{
public:
    LinkTree(TreeWikiArticle *items);
    ~LinkTree();
    void add(WikiLink* a);
    void save(string filename);
    void load(string filename);
    //first: ist von list
    //second: start_f_von List
    pair<int*,int*> toCL();
    size_t size() const;
private:
    pair<int*,int*> generateLinks(TreeWikiArticle items);
    vector<WikiLink*> items;
    int * __vonList;
    int * __start_f_von_List;
    //Store a pointer, for use in save.
    TreeWikiArticle* articles;
    int SizevonListe;
};

#endif // LINKTREE_H
