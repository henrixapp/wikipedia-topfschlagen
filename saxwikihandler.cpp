/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Id: SAXWikiHandler.cpp 557282 2007-07-18 14:54:15Z amassari $
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "saxwikihandler.h"
#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <ios>
#include <string>
#include <iostream>
#include "saxwikiparser.hpp"
#include <locale>
#include <iostream>       // std::cout, std::hex
#include <string>         // std::string, std::u32string
#include <locale>         // std::wstring_convert
#include <cstdint>        // std::uint_least32_t
#include "wikiarticle.h"
#include "wikilink.h"
#include "database/databasevector.h"
// ---------------------------------------------------------------------------
//  SAXWikiHandler: Constructors and Destructor
// ---------------------------------------------------------------------------
SAXWikiHandler::SAXWikiHandler(SQLConnection *a,bool article_adding) :

    fAttrCount(0)
    , fCharacterCount(0)
    , fElementCount(0)
    , fSpaceCount(0)
    , fSawErrors(false),
    db(a),
    liste(a),
    article_adding(article_adding),
    zustand(Parsing_bullshit),
    start_element(-1)//TODO: Change!
{

    //DatabaseLayout anlegen
    WikiArticle d(db,-1,"");
    d.updateTableFormat();
    WikiLink l(db,-1,0,0);
    l.updateTableFormat();
    current = NULL;
}

SAXWikiHandler::~SAXWikiHandler()
{
       if(article_adding) db->executeQueue("INSERT INTO WikiArticle(Titel) VALUES ");
    else db->executeQueue("INSERT INTO WikiLink(von,zu) VALUES ");
       db->close();
    delete db;
}

using namespace std;
// ---------------------------------------------------------------------------
//  SAXWikiHandler: Implementation of the SAX DocumentHandler interface
// ---------------------------------------------------------------------------
#include <boost/algorithm/string.hpp>
using namespace std;
using namespace boost::algorithm;

std::string conv(const XMLCh* str, size_t len)
{
  StrX s(str);
  string ss(s.localForm());
  trim(ss);//Todo leerzeichen durch _ ersetzen, um in Datei zu speichern
  replace_all(ss,"'","APX");//Coole standard ersetzung, kommt sonnst nichtfor
  return ss;
}
void SAXWikiHandler::startElement(const   XMLCh* const     name
                                    ,       AttributeList&  attributes)
{
    //char* message = XMLString::transcode(name);

  // size_t groesse =strlen(message);
//    XMLString::release(&message);
    //cout<<"groesse"<<groesse<<endl;
    //if(groesse<3) return;
    if((name[0]=='t')&&(name[1]=='i')&&(name[2]=='t')&&(name[3]=='l'))
       {
        ++fElementCount;
        if(article_adding)
        {
        if((fElementCount)%100==0)
        {
                db->executeQueue("INSERT INTO WikiArticle(Titel) VALUES ");
            cout<<fElementCount<<". Titel:"<<title.str()<<std::endl;

        }
        {
        WikiArticle* d = new WikiArticle(db,-1,title.str());
        d->dequeSave();//Speichern und direkt wieder vergessen
        delete d;
        }

        }
        else //jeden dritten link einfangen
        {
            db->executeQueue("INSERT INTO WikiLink(von,zu) VALUES ");
        }
        title.str("");
        fAttrCount += attributes.getLength();
        zustand= Parsing_title;

    }else if(article_adding)
    {
         zustand=Parsing_bullshit;
        //nichtstuen
    }
    else if((name[0]=='t')&&(name[1]=='e')&&(name[2]=='x')&&(name[3]=='t'))
    {
        zustand=Parsing_for_link;
        cout<<"Parsing:"<<title.str()<<endl;
       /* DatabaseVector<WikiArticle> alle(db,WIKIARTICLE_TABLE_NAME);
        stringstream term;
        term<<"Titel LIKE '%"<<title.str()<<"%'";
        alle.loadAll(term.str());*/
        article_link.str("");
        current = liste.find(title.str());
        if(current==NULL)
        {

            zustand=Parsing_bullshit;
            cout<<"Bullshit bei "<<title.str();
            int a;
            cin>>a;
            return;
        }
        if(current->id()<start_element)
        {
            zustand=Parsing_bullshit;

        }
          cout<<"Mit ID:"<<current->id()<<endl;

    }
    else
    {
        zustand=Parsing_bullshit;
    }

}

void SAXWikiHandler::characters(  const   XMLCh* const     text
                                    , const XMLSize_t       length)
{
    string chars =conv(text,length-1);
    switch(zustand)
    {
    case Parsing_title:
    title<<chars;
    fCharacterCount += length;
    break;
    case Parsing_for_link:
        //cout<<"Parse:"<<chars<<endl;
        //int w;
        //cin>>w;
        for(int i=0;i<(chars.length());i++)
        {
            switch(zustand)
            {
            case Parsing_for_link:
                if((chars[i]=='['))
                {
                    zustand=Parsing_for_link_a;
                }
                break;
            case Parsing_for_link_a:
               if((chars[i]=='['))
               {
                   //cout<<"fall a"<<endl;
                   zustand=Parsing_link;
               }
               else zustand=Parsing_for_link;
                break;//spring weiter
            case Parsing_link:

                if((chars[i]=='#')||(chars[i]=='|')||((chars[i]==']')&&(chars[i+1]==']')))
                {
                    //cout<<"fall b";
                    //article_link<<"\n";
                    WikiArticle * ziel=NULL;
                    ziel=liste.find(article_link.str());
                    article_link.str("");
                    if(ziel!=NULL)//falls entrag existiert.
                    {
                        WikiLink *w = new WikiLink(db,-1,current,ziel);
                        w->dequeSave();
                        delete w;
                        if(count_link%200==0)db->executeQueue("INSERT INTO WikiLink(von,zu) VALUES ");//alle zweihundert backup
                    }
                    else
                    {
                        ++count_error_links;
                        if(count_error_links%1000==0)cout<<count_error_links<<"/"<<count_link<<" fehlerhafte Links"<<endl;
                    }
                    zustand=Parsing_for_link;
                }
                else
                {
                    article_link<<chars[i];
                  //  cout<<endl<<article_link.str()<<endl<<endl;
                }
            }
        }
        break;
    case Parsing_bullshit:
        break;
    }
   // std::cout<<std::endl;

}

void SAXWikiHandler::ignorableWhitespace( const   XMLCh* const /* chars */
                                            , const XMLSize_t    length)
{
    fSpaceCount += length;
}

void SAXWikiHandler::resetDocument()
{
    fAttrCount = 0;
    fCharacterCount = 0;
    fElementCount = 0;
    fSpaceCount = 0;
}


// ---------------------------------------------------------------------------
//  SAXWikiHandler: Overrides of the SAX ErrorHandler interface
// ---------------------------------------------------------------------------
void SAXWikiHandler::error(const SAXParseException& e)
{
    fSawErrors = true;
    XERCES_STD_QUALIFIER cerr << "\nError at file " << StrX(e.getSystemId())
         << ", line " << e.getLineNumber()
         << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
}

void SAXWikiHandler::fatalError(const SAXParseException& e)
{
    fSawErrors = true;
    XERCES_STD_QUALIFIER cerr << "\nFatal Error at file " << StrX(e.getSystemId())
         << ", line " << e.getLineNumber()
         << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
}

void SAXWikiHandler::warning(const SAXParseException& e)
{
    XERCES_STD_QUALIFIER cerr << "\nWarning at file " << StrX(e.getSystemId())
         << ", line " << e.getLineNumber()
         << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
}

void SAXWikiHandler::resetErrors()
{
    fSawErrors = false;
}
