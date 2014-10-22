#include "wikilink.h"
#include <sstream>
/*WikiLink::WikiLink(SQLConnection *conn, int ID, WikiArticle *von, WikiArticle *zu):BasisItem(conn,WIKILINK_TABLE_NAME,ID)
{
    reserve();
    if(ID>-1)
    {
        load();
        return;
    }
    count_link++;
    if(von!=NULL)
    {
      //  cout<<von->Titel()<<"--->";
        setvon(von);
    }
    if(zu!=NULL)
    {
        //cout<<zu->Titel()<<endl;
        setzu(zu);
    }
}
WikiLink::WikiLink(SQLConnection * conn, int ID, int von, int zu):BasisItem(conn,WIKILINK_TABLE_NAME,ID)
{
    reserve();
    if(ID>-1)
    {
        load();
        return;
    }
    count_link++;
    setint("von",von);
    setint("zu",zu);
}

void WikiLink::reserve()
{
    registerColumn(types::ID,"von");
    registerColumn(types::ID,"zu");
}
int WikiLink::ziel()
{
    return getint("zu");
}
*/
WikiLink::WikiLink(int von, int zu)
{
    __von= von;
    __zu= zu;
}

int WikiLink::zu() const
{
    return __zu;
}

string WikiLink::toFile()
{
    std::stringstream a;
    a<<__von<<endl;
    a<<__zu;
    return a.str();
}
int WikiLink::von() const
{
    return __von;
}
