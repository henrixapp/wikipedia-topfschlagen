#include "wikilink.h"

WikiLink::WikiLink(SQLConnection *conn, int ID, WikiArticle *von, WikiArticle *zu):BasisItem(conn,WIKILINK_TABLE_NAME,ID)
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
