#ifndef OPENCLWATERSIMULATOR_H
#define OPENCLWATERSIMULATOR_H
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include "linktree.h"
#include "wikiarticle.h"
#include "treewikiarticle.h"
#include "dijkstraprovider.h"
namespace tools{
//multi dimensional, da es mehrere Möglichkeiten gibt.
//zunächst aber ein dimensional..., wobei wenn wir immer die teilstücke nehmen und zusammenstecken, abhängig von der Position
//durch wechsel des ZU articles...
//Der erste artikel wird !immer hinzugefügt!
template<typename typhus>
std::vector<std::vector<int> > traceBack(int* von, int* start_f_von, typhus* status,int von_article , int zu_article, int size, int size_connections,function<string(int)> converter)
{
    //Backbouncing
    std::vector<std::vector<int> >  result;
    vector<int> ergebnis;
   // cin>>von_article;
    int article=zu_article;

    if(article!=von_article)
    {
      //  cout<<"article"<<article<<endl;
        typhus status_article = status[article];
       // cout<<"status is"<<(int)status_article<<endl;
        int maximum = ((article+1)<size)?start_f_von[article+1]:size_connections;
//        cout<<"M"<<maximum<<endl;
        for(int j=start_f_von[article];j<maximum;j++)//TODO: bound check...
        {
            if(status[von[j]]==status_article-1)
            {
              //  cout<<"Exchangign"<<von[j]<<converter(von[j])<<endl;
                //Rekur
                auto zw = traceBack<typhus>(von,start_f_von,status,von_article,von[j],size,size_connections,converter);
                for(auto i=zw.begin();i!=zw.end();++i)
        {
            result.push_back(*i);
        }
        //article= von[j];//Fixed--> should make translation
                //cin>>j;
            }
        }
    }
    else {
    //ergebnis.push_back(article);
    result.push_back(ergebnis);//empty end recursion
}
    for(auto i = result.begin();i!=result.end();++i)
    {
    (*i).push_back(article);
    }
    return result;
}
}
class OpenCLWaterSimulator:public DijkstraProvider//TODO: Baseclass einführen
{
public:
    OpenCLWaterSimulator(LinkTree& verbindungen,TreeWikiArticle& artics);
    ~OpenCLWaterSimulator();
    virtual std::vector<std::vector<int> > suche(int von, int zu);
     virtual void parameterisierteAusfuehrung(function<bool(char*,unsigned long,unsigned long)> weiter_laufen,
                                     function<void(char*)> initialisierung,
                                     function<void(char*)> endbewertung);
private:
    cl::CommandQueue queue;
     cl::Kernel kernel;
     cl::Context context;
    size_t preferredSize = 32;
    LinkTree& verbindungen;
    TreeWikiArticle& articles;//Referenz, nicht kopieren

    //daten
    pair<int*,int*> daten;
    cl::Buffer von_cl,start_f_von_cl;
};

#endif // OPENCLWATERSIMULATOR_H
