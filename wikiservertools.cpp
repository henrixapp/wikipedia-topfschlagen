#include "wikiservertools.h"

WikiServerTools::WikiServerTools(LinkTree *l, TreeWikiArticle *t, OpenCLWaterSimulator* sim, uri url):Artikel(t),verbindungen(l),m_listener(http_listener(url)),simulator(sim)

{
    m_listener.support(methods::GET,
                       bind(&WikiServerTools::handle_get,
                            this,
                            placeholders::_1));
    m_listener.support(methods::POST,
                       bind(&WikiServerTools::handle_post,
                            this,
                            placeholders::_1));
    m_listener.open();
}
void WikiServerTools::handle_get(http_request request)
{
    try
    {
    auto anfrageURI = uri::split_path(request.relative_uri().path());
    if(anfrageURI[0]=="distance")
    {
        int von = boost::lexical_cast<int>(anfrageURI[1]);//von ist die erste
        int zu = boost::lexical_cast<int>(anfrageURI[2]);//von ist die erste
        cout<<von<<" nach "<<zu<<endl;
        json::value response;
        response["status"]=json::value::string("OK");
        request.reply(200,response);
        return;
    }
    json::value response;
    response["status"]=json::value::string("NOT FOUND");
    request.reply(404,response);
    }
    catch(...)
    {
        json::value response;
        response["status"]=json::value::string("ERROR");
        request.reply(500,response);
    }
}
void WikiServerTools::handle_post(http_request request)
{
    try
    {
    auto anfrageURI = uri::split_path(request.relative_uri().path());
    if(anfrageURI[0]=="distance")
    {
        request.extract_json().then([=](json::value req)
        {
            auto von = Artikel->find(req["von"].as_string());
            if(von==NULL)
            {
                json::value response;
                response["case"]=json::value::string("VON");
                response["status"]=json::value::string("NOT FOUND");
                request.reply(404,response);
                return;
            }
            auto zu = Artikel->find(req["zu"].as_string());
            if(zu==NULL)
            {
                json::value response;
                response["case"]=json::value::string("ZU");
                response["status"]=json::value::string("NOT FOUND");
                request.reply(404,response);
                return;
            }
            cout<<"[HTTP] searching:"<<von->Titel()<<" nach "<<zu->Titel()<<endl;
            auto result = simulator->suche(von->ID(),zu->ID());
            //Welche nodes brauchen wir alles?
            vector<int> umwelt;
            vector<int> ebenen;
            for(auto i=result.begin();i!=result.end();++i)
            {
                int ebene =0;
                for(auto j=(*i).begin();j!=(*i).end();++j)
                {
                    if(std::find(umwelt.begin(),umwelt.end(),(*j))==umwelt.end())
                    {
                        umwelt.push_back(*j);
                        ebenen.push_back(ebene);
                    }
                    ebene++;
                }
            }
           // cout<<"Hier durch"<<umwelt.size()<<endl;
            //schreiben der nodes in nodes
            json::value nodes;
            int c=0;
            int x=0;
            int y=0;
            for(auto i= umwelt.begin();i!=umwelt.end();i++)
            {
                try
                {
               // cout<<i-umwelt.begin()<<"-->"<<*i<<endl;
               auto eintrag = Artikel->find(*i);
               if(eintrag==NULL)
               {
                   cout<<"Fehler"<<*i<<endl;
                   continue;
               }
               //cerr<<"b"<<eintrag->ID()<<"-->"<<eintrag->Titel()<<endl;
               json::value entry;
               stringstream sstreamer;
               sstreamer<<"n"<<eintrag->ID();
               entry["id"] = json::value::string(sstreamer.str());
               entry["label"]= json::value::string(eintrag->Titel());
               cout<<"a"<<endl;
               int size=3;
               if((*i)==von->ID()) size=5;
               if((*i)==zu->ID()) size=5;
               entry["size"]=json::value::number(size);
               entry["x"]=json::value::number(x++%((int)result.size()));
               entry["y"]=json::value::number(ebenen.at(i-umwelt.begin()));//no
               if((*i)==zu->ID()) entry["type"]= json::value::string("star");
               nodes[c++] = entry;
               //cout<<c<<endl;
                }
                catch(exception &e)
                {
                    cout<<"Fehler:"<<e.what()<<endl;
                }
            }
           // cout<<"Hier durch"<<endl;
            json::value edges;
            int id=0;
            //TODO: Zusätzliche Edges...
            for(auto i = result.begin();i!= result.end();++i)
            {
                int source=-1;
                int target=-1;
                for(auto j = (*i).begin();j!=(*i).end();++j)
                {
                    source=target;//umdrehen
                    target=*j;
                    if(source!=-1)
                    {
                       json::value entry;
                       stringstream istreamer;
                       istreamer<<"e"<<id;
                       entry["id"]= json::value::string(istreamer.str());
                       stringstream sstreamer;
                       sstreamer<<"n"<<source;
                       stringstream tstreamer;
                       tstreamer<<"n"<<target;
                       entry["source"]= json::value::string(sstreamer.str());
                       entry["target"]= json::value::string(tstreamer.str());
                       entry["type"]=json::value::string("arrow");
                       edges[id++]=entry;
                    }
                }
            }
            json::value response;
            response["nodes"]=nodes;
            response["edges"]=edges;
            //response["status"]=json::value::string("OK");
           // cout<<"Hier durch"<<response<<endl;
            request.reply(200,response);
            return;
        });
        return;
    }
    json::value response;
    response["status"]=json::value::string("NOT FOUND");
    request.reply(404,response);
    }
    catch(...)
    {
        json::value response;
        response["status"]=json::value::string("ERROR");
        request.reply(500,response);
    }
}
