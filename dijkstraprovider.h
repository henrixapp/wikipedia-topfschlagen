#ifndef DIJKSTRAPROVIDER_H
#define DIJKSTRAPROVIDER_H
#include <vector>
#include <functional>

using namespace std;
class DijkstraProvider
{
public:
    virtual ~DijkstraProvider(){}
    virtual std::vector<std::vector<int> > suche(int von, int zu)=0;
    virtual void parameterisierteAusfuehrung(function<bool(char*,unsigned long,unsigned long)> weiter_laufen,
                                     function<void(char*)> initialisierung,
                                     function<void(char*)> endbewertung)=0;
};

#endif // DIJKSTRAPROVIDER_H
