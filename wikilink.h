#ifndef WIKILINK_H
#define WIKILINK_H
#include "database/macros.h"
#include "wikiarticle.h"
#define WIKILINK_TABLE_NAME "WikiLink"
extern int count_link;
/*class WikiLink : public BasisItem
{
public:
    WikiLink(SQLConnection * conn, int ID=-1, WikiArticle *von=NULL, WikiArticle *zu=NULL);
    WikiLink(SQLConnection * conn, int ID=-1, int von=0, int zu=0);
    void reserve();
    int ziel();
//    relatedDStore(WikiArticle,"von",von)
 //   relatedDStore(WikiArticle,"zu",zu)
};*/
class WikiLink
{
public:
    //Von ist der Start link ist das ziel
    WikiLink(int von, int zu);
    //macht aus dem ganzen Objekt eine Dateisache
    string toFile();
    int zu() const;
    int von() const;
private:
    int __von,__zu;
};

#endif // WIKILINK_H
