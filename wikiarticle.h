#ifndef WIKIARTICLE_H
#define WIKIARTICLE_H
//#include "database/macros.h"
#include <boost/unordered_set.hpp>
//#define WIKIARTICLE_TABLE_NAME "WikiArticle"
/*class WikiArticle : public BasisItem
{
public:
    WikiArticle(SQLConnection *conn, int ID=-1,string titel="");
    void reserve();
    CDStore(string,Titel)
    bool operator==(const WikiArticle* &p) const
      {
        return Titel() == p->Titel();
      }
};*/
#include <iostream>
using namespace std;
class WikiArticle
{
public:
    WikiArticle(string Title, int ID);
    bool operator==(const WikiArticle* &p) const
      {
        cout<<"comparing is sharing"<<endl;
        int i;
        cin>>i;
        return __title == p->__title;
      }
    const int ID() const;
    const string Titel() const;
private:
        string __title;
        int __id;
};

std::size_t hash_value(WikiArticle* const &p);
#endif // WIKIARTICLE_H
