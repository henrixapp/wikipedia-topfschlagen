#include <xercesc/parsers/SAXParser.hpp>
   #include <xercesc/sax/HandlerBase.hpp>
   #include <xercesc/util/XMLString.hpp>
#include "saxwikiparser.hpp"
#include "saxwikihandler.h"
   #include <iostream>
#include "openclwatersimulator.h"
   using namespace std;
   using namespace xercesc;
//Forwarden
int count_link=0;
int count_error_links=0;
   int main (int argc, char* args[]) {
       if(argc<3)
       {
           cerr<<"Bitte verwende das Program so:"<<endl;
           cerr<<"./suchen [wiki_file] {links}"<<endl;
           return -1;
       }
       string wiki_base(args[1]);
       string links_base(args[2]);
       TreeWikiArticle wiki;
       wiki.load(wiki_base);
       LinkTree links;
       links.load(links_base);
       WikiArticle* start =NULL;
       while(start==NULL)
       {
           cout<<"Gib bitte den Titel des Startartikels ein:";
           string titel;
           cin>>titel;
            start = wiki.find(titel);
       }
       cout<<"Startartikel:"<<start->Titel()<<" mit ID:"<<start->ID()<<endl;
       auto ende_a= (WikiArticle*)NULL;
       while(ende_a==NULL)
       {
           cout<<"Gib bitte den Titel des Zielartikels ein:";
           string ende;
           cin>>ende;
            ende_a = wiki.find(ende);
       }
       cout<<"Zielartikel:"<<ende_a->Titel()<<" mit ID:"<<ende_a->ID()<<endl;
       cout<<"Starte OpenCL..."<<endl;
       OpenCLWaterSimulator searchAgent(links,wiki);
       cout<<"Starte Suche...";
       int result = searchAgent.suche(start->ID(),ende_a->ID());
       cout<<"Ergebnis:"<<result;
       return 0;
   }

