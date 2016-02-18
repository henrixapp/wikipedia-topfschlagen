#include "linktree.h"

LinkTree::LinkTree(TreeWikiArticle *items)
{
    __start_f_von_List= NULL;
    __vonList= NULL;
    articles = items;
    SizevonListe=-1;
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
    //AUS load

    //Konvertierung in ein übliches Array
    pair<int*,int*> result;
    int si=items.size();
    int * arrayed = new int[si];
    std::fill_n(arrayed,si,-1);//TODO: richtigesmaximum setzen
    int* start_f_von= articles->toCL();
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
           if((*i)->zu()<articles->size())
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
    for(int i= articles->size()-1;i>-1;i--)
    {
        if(start_f_von[i]==-1) start_f_von[i]=currentFill;//alten wert überschreiben
        else currentFill=start_f_von[i];//Neu laden
    }
   // for(int i=0;i<articles.size();i++) if(start_f_von[i]==-1) cerr<<i<<"is fallsed"<<endl;
    result.second = start_f_von;
    result.first= arrayed;
    //Datei öffnen
    ofstream o;
    o.open(filename,ios::out|ios::binary);
    //Datei Identifikation schreiben.(zurZeit deaktiviert)
 //   int identifier = 1935033665;
   // o.write((char*)&identifier,sizeof(int));
    //startvon schreiben, zunächst die größe
    int size_start_f_von = articles->size();
    o.write((char*)&size_start_f_von,sizeof(int));
    for(int i = 0;i<size_start_f_von;i++)
    {
        o.write((char*)&start_f_von[i],sizeof(int));
    }
    delete[] start_f_von;
    //nun die eigentliche verlinkungen
    int size=items.size();
    o.write((char*)&size,sizeof(int));
    for(int i = 0;i<size;i++)
    {
        o.write((char*)&arrayed[i],sizeof(int));
    }
    delete[] arrayed;
    o.close();
}
void LinkTree::load(string filename)
{
    ifstream in(filename,ios::in|ios::binary);
    int sizestart_f_von=0;
    in.read((char*)&sizestart_f_von,sizeof(int));
    //Ausgabe alloc für diese daten
    cout<<"Anzahl der Artikel:"<<sizestart_f_von<<endl;
    __start_f_von_List = new int[sizestart_f_von];
    int item=0;
    for(int i= 0;i<sizestart_f_von;i++){
        in.read((char*)&item,sizeof(int));
        __start_f_von_List[i] = item;
    }
     SizevonListe=0;
     in.read((char*)&SizevonListe,sizeof(int));
     //Ausgabe alloc für diese daten
     cout<<"Anzahl der Verbindungen:"<<SizevonListe<<endl;
     __vonList = new int[SizevonListe];
     for(int i= 0;i<SizevonListe;i++){
         in.read((char*)&item,sizeof(int));
         __vonList[i] = item;
     }
}

void LinkTree::add(WikiLink *a)
{
    //
    items.push_back(a);
}
std::pair<int*,int*> LinkTree::toCL()
{
    return pair<int*,int*>(__vonList,__start_f_von_List);
}
size_t LinkTree::size() const
{
    if(SizevonListe==-1) return items.size();
    else return SizevonListe;
}
