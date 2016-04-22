#include "classicaldijkstraprovider.h"
#include "openclwatersimulator.h"
#include <xercesc/parsers/SAXParser.hpp>
   #include <xercesc/sax/HandlerBase.hpp>
   #include <xercesc/util/XMLString.hpp>
ClassicalDijkstraProvider::ClassicalDijkstraProvider(LinkTree &tree,TreeWikiArticle &wikia):links(tree),wiki(wikia)
{

}
//OPENCL quasi impl.
 void ocl_water_sim( int *von_array,  int * start_f_von, char* status_array,
                     int elements, int target,char current_round, int connection_count,int id)
{
if(id<elements){
if(status_array[id]==-1)
{
    int start_id=start_f_von[id];//later on:     (id>=(elements-1))?elements-1:id;
    int maximum_start= 0;
    if(id<(elements-1))
    {
        maximum_start = start_f_von[id+1];
    } else maximum_start= connection_count;//Rest auslesen...

    for(int i=start_id;i<maximum_start;++i)//hier ischt der Hund begraben--> -1
    {
       if(status_array[von_array[i]] == current_round)//vielleicht statt -1 0 nehmen
       {
          //erhöhen
           status_array[id]=status_array[von_array[i]]+1;
           return;//Early exiting...
       }
    }
}
}
else {
    cout<<"Out of bound,none important error occured"<<endl;
}
//#schon da
}
 long getCurrentMillis(){

    return xercesc::XMLPlatformUtils::getCurrentMillis();
 }

std::vector<std::vector<int> > ClassicalDijkstraProvider::suche(int von, int zu){
    //Später;
    //parameterisierteAusfuehrung([&](char * bett,unsigned long ));
    char * feld= new char[wiki.size()];
    std::fill_n(feld,wiki.size()*sizeof(char),-1);
    feld[von]=0;
    char tries=-1;
    //Laden aus links.toCL
    auto daten =links.toCL();
    int * von_array= links.toCL().first;
    int * start_f_von= links.toCL().second;
#define TRIES_MAX 25
    //simulieren
    cout<<"[ClassicalDijkstra] daten erstellt"<<wiki.size()<<endl;
    while((feld[zu]==-1)&&((tries++)<TRIES_MAX)){
        cout<<"[ClassicalDijkstra]"<<tries<<endl;
        for(int i=0;i<wiki.size();i++)
        {
            ocl_water_sim(von_array,start_f_von,feld,wiki.size(),zu,tries,links.size(), i);
        }
    }
    //ergebnisse zurückholen
    std::vector<std::vector<int> > result = tools::traceBack<char>(daten.first, daten.second,feld,von,zu,wiki.size(),links.size(),[&](int id){
        return wiki.find(id)->Titel();
    });
    //speicher freigeben
    delete[] feld;
    //ergebnis zurückgeben
    cout<<"[ClassicalProvider]/size:"<<result.size()<<endl;
    return result;
}

void ClassicalDijkstraProvider::parameterisierteAusfuehrung(function<bool (char *, unsigned long, unsigned long)> weiter_laufen, function<void (char *)> initialisierung, function<void (char *)> endbewertung)
{
    char * feld= new char[wiki.size()];
    initialisierung(feld);
    char tries=-1;
    //Laden aus links.toCL
    auto daten =links.toCL();
    int * von_array= links.toCL().first;
    int * start_f_von= links.toCL().second;
#define TRIES_MAX 25
    //simulieren
    cout<<"[ClassicalDijkstra] daten erstellt"<<wiki.size()<<endl;
    long time = getCurrentMillis();
    while(weiter_laufen(feld,tries++,getCurrentMillis()-time)){
        cout<<"[ClassicalDijkstra]"<<tries<<endl;
        for(int i=0;i<wiki.size();i++)
        {
            ocl_water_sim(von_array,start_f_von,feld,wiki.size(),0,tries,links.size(), i);
        }
    }
    //endbewertung
    endbewertung(feld);
    delete[] feld;
}
