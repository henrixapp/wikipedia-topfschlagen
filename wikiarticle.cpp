#include "wikiarticle.h"
using namespace std;
WikiArticle::WikiArticle(SQLConnection *conn,int ID,string titel):BasisItem(conn,WIKIARTICLE_TABLE_NAME,ID)
{
    reserve();
    if(ID>-1)
    {
        load();
        //cout<<"Geldaen:"<<Titel()<<endl;
        return;
    }
    //cout<<"Adding:"<<titel<<endl;
   // int i;
    //cin>>i;
    setTitel(titel);
}
void WikiArticle::reserve()
{
    registerColumn(types::TEXT,"Titel");
}
std::size_t hash_value(WikiArticle* const &p)
{
    std::size_t seed=0;
    boost::hash_combine(seed,p->Titel());
    return seed;
}
