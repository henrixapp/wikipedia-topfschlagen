#include "linktree.h"

LinkTree::LinkTree()
{
}
LinkTree::~LinkTree()
{
    for(auto i= items.begin();i!=items.end();++i)
    {
        delete (*i);
    }
}
void LinkTree::save(string filename)
{
    ofstream o;
    o.open(filename);
    o<<items.size()<<endl;//endl vergessen---
    int countc =0;
    for(auto i= items.begin();i!=items.end();++i)
    {
        o<<(*i)->toFile()<<endl;
        countc++;
    }
    cout<<"Anzahl: "<<countc<<endl;
    o.close();
}
void LinkTree::load(string filename)
{
    ifstream in(filename);
    int size=0;
    in>>size;
    for(int i=0;i<size;i++)
    {
        int von, zu;
        in>>von;
        in>>zu;
       if(zu==0)
       {cout<<von<<"-->"<<zu<<endl; cin>>zu;}
        auto newthing = new WikiLink(von,zu);
        items.push_back(newthing);
        if(i<(10))
        {
            cout<<(items.at(i))->von()<<endl;
            cout<<(items.at(i))->zu()<<endl;
      }
            // int a; cin>>a;
    }
    cout<<size<<" Elemente geladen."<<endl;
}

void LinkTree::add(WikiLink *a)
{
    //
    items.push_back(a);
}
std::pair<int*,int*> LinkTree::toCL(TreeWikiArticle articles)
{
    //Konvertierung in ein übliches Array
    pair<int*,int*> result;
    int si=items.size();
    int * arrayed = new int[si];
    std::fill_n(arrayed,si,-1);//TODO: richtigesmaximum setzen
    int* start_f_von= articles.toCL();
    {
    std::sort(items.begin(),items.end(),[](const WikiLink* a, const WikiLink* b)->bool
    {
       return a->zu()<b->zu();
    });
    cout<<items[0]->zu()<<" "<<(*(items.end()-1))->zu()<<endl;
     cout<<(*items.begin())->zu()<<" "<<(*(items.end()-1))->zu()<<" "<<(*items.begin())->von()<<endl;
    int currentID=0;
    int groesse=0;
    for(auto i= items.begin();i!=items.end();i++)
    {
       // cout<<"Bearbeite "<<(*i)->von()<<" zu "<<(*i)->zu()<<endl;
        if((*i)->zu()!=currentID)
        {
            //speichern
           // cout<<"Bearbeite "<<(*i)->von()<<" zu "<<(*i)->zu()<<endl;
           if((*i)->zu()<articles.size())
           {
            start_f_von[(*i)->zu()] = (groesse);//size ist die ID vom nächsten :-D
           } else cerr<<"Fehler bei:"<<(*i)->zu()<<endl;
            currentID=(*i)->zu();

        }
        groesse++;
        //outbound protection?
        arrayed[i-items.begin()]=((*i)->von());
    }
}
    int currentFill= size()-1;//Die aktuelle Füllung von hinten
    for(int i= articles.size()-1;i>-1;i--)
    {
        if(start_f_von[i]==-1) start_f_von[i]=currentFill;//alten wert überschreiben
        else currentFill=start_f_von[i];//Neu laden
    }
   // for(int i=0;i<articles.size();i++) if(start_f_von[i]==-1) cerr<<i<<"is fallsed"<<endl;
    result.second = start_f_von;
    result.first= arrayed;
     //müssen wir VonList noch mal pushen oder start_f_von
    return result;
}
size_t LinkTree::size() const
{
    return items.size();
}
