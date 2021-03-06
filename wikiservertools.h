#ifndef WIKISERVERTOOLS_H
#define WIKISERVERTOOLS_H
#include "linktree.h"
#include "treewikiarticle.h"
#include "cpprest/http_listener.h"
#include "dijkstraprovider.h"
using namespace web::http::experimental::listener;
using namespace web::http;
using namespace web;
using namespace std;
class WikiServerTools
{
public:
    WikiServerTools(LinkTree *links,TreeWikiArticle *articles, DijkstraProvider *sim,uri url);
     //get is a map
    void handle_get(http_request request);
    //post is a search request
    void handle_post(http_request request);
private:
    LinkTree *verbindungen;
    TreeWikiArticle *Artikel;
    DijkstraProvider *simulator;
    http_listener m_listener;
};

#endif // WIKISERVERTOOLS_H
