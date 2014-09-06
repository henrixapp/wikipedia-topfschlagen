#ifndef WIKIARTICLE_H
#define WIKIARTICLE_H
#include "database/macros.h"
#include <boost/unordered_set.hpp>
#define WIKIARTICLE_TABLE_NAME "WikiArticle"
class WikiArticle : public BasisItem
{
public:
    WikiArticle(SQLConnection *conn, int ID=-1,string titel="");
    void reserve();
    CDStore(string,Titel)
    bool operator==(const WikiArticle* &p) const
      {
        return Titel() == p->Titel();
      }
};
std::size_t hash_value(WikiArticle* const &p);
#endif // WIKIARTICLE_H
