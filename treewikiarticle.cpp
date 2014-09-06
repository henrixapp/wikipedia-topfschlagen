#include "treewikiarticle.h"
#include "database/databasevector.h"
/*TreeManager<WikiArticle>::RelaxedDecidor decidor =[](WikiArticle * node,WikiArticle* wert)
{
    if((node->Titel())<(wert->Titel()))
    {
        return 1;
    } else if((node->Titel())==(wert->Titel()))
    {
        return 0;
    } else
    {
        return -1;
    }
};
*/
TreeWikiArticle::TreeWikiArticle(SQLConnection *dba)
{
    db= dba;
    /*DatabaseVector<WikiArticle> alle(db,WIKIARTICLE_TABLE_NAME);
    alle.loadAll();
    int count =0;
    for(auto i= alle.begin();i!=alle.end();++i)
    {
        insert(decidor,*i);
        (++count);
        if(true) cout<<"Höhe: "<<height()<<" Elemente: "<<count<<endl;
    }
    cout<<"Height:"<<height()<<endl;*/
}
int count_ar =0;
WikiArticle * TreeWikiArticle::find(string titel)
{
   /* TreeManager<WikiArticle>::Decidor findor =[titel](WikiArticle* node)
    {
       // cout<<node<<"node"<<(node->Titel())<<" vs. "<<titel<<endl;
        if((node->Titel())<(titel))
        {
            return 1;
        } else if((node->Titel())==(titel))
        {
            return 0;
        } else
        {
            return -1;
        }
    };*/
    WikiArticle* zw = NULL;
    auto i = karte.find(titel);
    if(i!=karte.end()) zw = (*i).second;
    //cout<<"zw:"<<zw<<endl;
    if(zw==NULL)
    {
        DatabaseVector<WikiArticle> alle(db,WIKIARTICLE_TABLE_NAME);
                stringstream term;
                boost::algorithm::erase_all(titel,"'");//fixed um 3:17, ersetzen durch prepare
                term<<"Titel = '"<<titel<<"'";
                alle.loadAll(term.str());
                if(!alle.empty()) zw= alle[0];
                if(zw!=NULL)karte.insert(std::pair<string,WikiArticle*>(titel,zw));
                //insert(decidor,zw);
                count_ar++;
                //if(count_ar%10000==0)cout<<"Höhe: "<<karte.size()<<" Elemente: "<<count_ar<<endl;
    } else {

        if(karte.size()%10000==0)cout<<"Höhe: "<<karte.size()<<" Elemente: "<<count_ar<<endl<<"FOUND \a"<<endl;;
        //cin>>count_ar;
    }
    if((zw==NULL)&&(count_ar%1000==0))cout<<"Fehler:"<<titel<<endl;
    return zw;
}
