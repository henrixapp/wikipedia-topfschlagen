#ifndef CLASSICALDIJKSTRAPROVIDER_H
#define CLASSICALDIJKSTRAPROVIDER_H

#include "dijkstraprovider.h"
#include "linktree.h"

#include <functional>
#include <vector>
using namespace std;
class ClassicalDijkstraProvider : public DijkstraProvider
{
public:
    ClassicalDijkstraProvider(LinkTree &links,TreeWikiArticle &wiki);
    ~ClassicalDijkstraProvider(){}
    virtual std::vector<std::vector<int> > suche(int von, int zu);
     virtual void parameterisierteAusfuehrung(function<bool(char*,unsigned long,unsigned long)> weiter_laufen,
                                     function<void(char*)> initialisierung,
                                     function<void(char*)> endbewertung);
private:
    LinkTree &links;
    TreeWikiArticle wiki;
};

#endif // CLASSICALDIJKSTRAPROVIDER_H
