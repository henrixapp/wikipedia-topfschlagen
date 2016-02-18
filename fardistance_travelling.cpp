//(c) 2015, Henrik Reinstädtler
/**
  * Diese Datei dient der Abschätzung von Entfernungen in Wikipedia-> wie weit sind die Sachen weg?
  * Einführung von Priviligen
  *
*/
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
       LinkTree links(&wiki);
       links.load(links_base);
       cout<<"Starte OpenCL..."<<endl;
       OpenCLWaterSimulator searchAgent(links,wiki);
       bool running=true;
       while(running)
       {
             WikiArticle* start =NULL;
       while(start==NULL)
       {
           cout<<"Gib bitte den Titel des Startartikels ein:";
           string titel;
           cin>>titel;
            start = wiki.find(titel);
       }
       cout<<"Startartikel:"<<start->Titel()<<" mit ID:"<<start->ID()<<endl;
        unsigned int intinput;
        cout<<"Wie weit sollen wir uns entfernen?";
        cin>> intinput;
        char Entfernung = intinput;
       cout<<"Starte Suche...";
       searchAgent.parameterisierteAusfuehrung([Entfernung](char* d,unsigned long run, unsigned long time)
       {
           //int max=0;
           //for(int i=0; i<wiki.size();i++) if(d[i]!=-1) if(d[i]>max) max=d[i];
           //cout<<max<<" Treffer beim "<<run<<"Lauf"<<endl;
           //Tester run gibt null passiert die Ausgabe an.
           return run<(Entfernung+1);//true solange kleiner oder gleich, mit +1 klappt es aufjeden fall
       },[&](char* array)
       {
           for(int i=0; i<wiki.size();i++) array[i]=-1;
           array[start->ID()]=0;
       },[&](char* array)
       {
           //durchgehen und dann Raussuchen bei -1
           int count=0;
           for(int i=0; i<wiki.size();i++)
           {
               if(array[i]==Entfernung)
               {
                   cout<<++count<<wiki.find(i)->Titel()<<"-->"<<(int)array[i]<<endl;
               }
           }
           cout<<count<<" Treffer"<<endl;
       });
       cout<<"Beenden [Y/n]?";
       string answer;cin>>answer;
       if(answer=="Y") running=false;
       }
       return 0;
   }
