#ifndef OPENCLWATERSIMULATOR_H
#define OPENCLWATERSIMULATOR_H
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include "linktree.h"
#include "wikiarticle.h"
#include "treewikiarticle.h"
class OpenCLWaterSimulator
{
public:
    OpenCLWaterSimulator(LinkTree& verbindungen,TreeWikiArticle& artics);
    ~OpenCLWaterSimulator();
    int suche(int von, int zu);
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
