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
    o.open(filename,ios::out|ios::binary);
    int size=items.size();
    o.write((char*)&size,sizeof(int));//endl vergessen---
    int countc =0;
    for(auto i= items.begin();i!=items.end();++i)
    {
        //o<<(*i)->toFile()<<endl;
        int von,zu;
        von= (*i)->von();
        zu=(*i)->zu();
        o.write((char*)&von,sizeof(int));
        o.write((char*)&zu,sizeof(int));
        countc++;
    }
    cout<<"Anzahl: "<<countc<<endl;
    o.close();
}
void LinkTree::load(string filename)
{
    ifstream in(filename,ios::in|ios::binary);
    int size=0;
    in.read((char*)&size,sizeof(int));
    for(int i=0;i<size;i++)
    {
        int von, zu;
        in.read((char*)&von,sizeof(int));
        in.read((char*)&zu,sizeof(int));
        auto newthing = new WikiLink(von,zu);
        items.push_back(newthing);
            // int a; cin>>a;
    }
    cout<<size<<" Links geladen."<<endl;
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
