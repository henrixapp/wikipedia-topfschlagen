#ifndef OPENCLWATERSIMULATOR_H
#define OPENCLWATERSIMULATOR_H
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include "linktree.h"
#include "wikiarticle.h"
#include "treewikiarticle.h"
class OpenCLWaterSimulator//TODO: Baseclass einführen
{
public:
    OpenCLWaterSimulator(LinkTree& verbindungen,TreeWikiArticle& artics);
    ~OpenCLWaterSimulator();
    std::vector<std::vector<int> > suche(int von, int zu);
     void parameterisierteAusfuehrung(function<bool(char*,unsigned long,unsigned long)> weiter_laufen,
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
