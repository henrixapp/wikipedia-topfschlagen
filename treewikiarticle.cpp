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
TreeWikiArticle::TreeWikiArticle()
{
    int count;
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
WikiArticle * TreeWikiArticle::find(string id)
{
    WikiArticle* zw = NULL;
    if(id=="") return zw;//Titellos gibt es nicht
    auto i = karte.find(boost::to_lower_copy(id));
    if(i!=karte.end()) zw = (*i).second;
    //cout<<"zw:"<<zw<<endl;
    if(zw==NULL)
    {

                i = karte.find(id);
                if(i!=karte.end()) zw = (*i).second;
                if(zw!=NULL)
                {
                    cout<<"mismatch, but caught"<<endl;
                }
                cerr<<"Error...--> not found:"<<id<<endl;
               // int id;
                //cin>>id;
                //kein lazyloading--> erstellen
               // zw = new WikiArticle(titel,count++);//ID ist aktuelle Größe

               // if(zw!=NULL)karte.insert(std::pair<string,WikiArticle*>(titel,zw));

                //insert(decidor,zw);
                count_ar++;
                //if(count_ar%10000==0)cout<<"Höhe: "<<karte.size()<<" Elemente: "<<count_ar<<endl;
    } else {
       // cout<<"Gefunden:"<<titel;
      //  int id;
        //cin>>id;
        //if(karte.size()%10000==0)cout<<"Höhe: "<<karte.size()<<" Elemente: "<<count_ar<<endl<<"FOUND \a"<<endl;;
        //cin>>count_ar;
    }
    if((zw==NULL)&&(count_ar%1000==0))cout<<"Fehler:"<<id<<endl;
    return zw;
}
void TreeWikiArticle::add(string title)
{
    WikiArticle * zw = new WikiArticle(title,karte.size());//ID ist aktuelle Größe

    karte.insert(std::pair<string,WikiArticle*>(boost::to_lower_copy(title),zw));
}
WikiArticle * TreeWikiArticle::find(int id)
{
    for(auto i= karte.begin();i!=karte.end();i++) if((*i).second->ID()==id) return (*i).second;
}

void TreeWikiArticle::save(string filename)
{
    cout<<"SAVING to "<<filename<<endl;
    ofstream datei(filename);
    datei<<karte.size()<<endl;
    for(auto i = karte.begin();i!= karte.end();++i)
    {
         datei<<(*i).second->ID();
         datei<<" ";
        datei<<(*i).second->Titel();
        datei<<"]";
    }
    datei.close();
}
void TreeWikiArticle::load(string filename)
{
    ifstream datei(filename);
    int size=0;
    datei>>size;
    cout<<"Elemente:"<<size<<endl;
    for(int i=0;i<size;++i)
    {
        int ID;
        string title;
        datei>>ID;
        std::getline(datei,title,']');
        boost::algorithm::replace_all(title," ","");
       // cout<<"<"<<title<<">"<<endl;int sizae;
        //cin>>sizae;
       // datei.getline(title,1000,']');
        WikiArticle*  zw = new WikiArticle(title,ID);
        karte.insert(std::pair<string,WikiArticle*>(boost::to_lower_copy(title),zw));
        if(i%5000==0)cout<<i<<". geladen"<<endl;
        if((i>(size-10))||(ID>size))cout<<ID<<">"<<size<<"-->"<<title<<endl;
    }
    cout<<karte.size()<<" Artikel geladen. Bitte geladen..."<<endl;
    //cin>>size;
}
int* TreeWikiArticle::toCL()
{
    int* items = new int[karte.size()];
    int size = *(items-1);
    cout<<"Groeßed:"<<size<<"Erstes"<<*items<<"Erstw"<<items[0]<<endl;
    std::fill_n(items,karte.size(),0);
    cout<<"Groeßed:"<<size<<"Erstes"<<*items<<"Erstw"<<items[0]<<endl;
    return items;
}
size_t TreeWikiArticle::size() const
{
    return karte.size();
}
