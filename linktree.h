#ifndef LINKTREE_H
#define LINKTREE_H
#include <vector>
#include "wikilink.h"
#include "treewikiarticle.h"
using namespace std;
class LinkTree
{
public:
    LinkTree();
    ~LinkTree();
    void add(WikiLink* a);
    void save(string filename);
    void load(string filename);
    //gibt nach den "zu" gesortete Ints von zurück
    int * toVonList();
    //liefert die IDs von dem zurück
    int * toStart_F_Von();
    //first: ist von list
    //second: start_f_von List
    pair<int*,int*> toCL(TreeWikiArticle items);
    size_t size() const;
private:
    vector<WikiLink*> items;

};

#endif // LINKTREE_H
