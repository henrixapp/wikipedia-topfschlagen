#include <xercesc/parsers/SAXParser.hpp>
   #include <xercesc/sax/HandlerBase.hpp>
   #include <xercesc/util/XMLString.hpp>
#include "saxwikiparser.hpp"
#include "saxwikihandler.h"
   #include <iostream>

   using namespace std;
   using namespace xercesc;
int count_link=0;
int count_error_links=0;
   int main (int argc, char* args[]) {
        cout<<"int size:"<<sizeof(int)<<endl;
       try {
           XMLPlatformUtils::Initialize();
       }
       catch (const XMLException& toCatch) {
           char* message = XMLString::transcode(toCatch.getMessage());
           cout << "Error during initialization! :\n"
                << message << "\n";
           XMLString::release(&message);
           return 1;
       }
       bool article_adding= argc==3;
       char* xmlFile = args[1];
       SAXParser* parser = new SAXParser();
       //parser->setDoValidation(true);
       parser->setDoNamespaces(true);    // optional
        //SQLConnection* db = new SQLITEconnection();
       // db->connect("/media/ramdisk/simple_wiki2.sqlite");
       if(article_adding) cout<<"DOING magick..."<<endl;
       DocumentHandler* docHandler = new SAXWikiHandler(article_adding,string(args[2]),string((argc==4)?args[3]:"*"));
       ErrorHandler* errHandler = (ErrorHandler*) docHandler;
       parser->setDocumentHandler(docHandler);
       parser->setErrorHandler(errHandler);

       try {
           const unsigned long time = XMLPlatformUtils::getCurrentMillis();
           parser->parse(xmlFile);
            const unsigned long timeAfter = XMLPlatformUtils::getCurrentMillis();
            cout<<"Took "<<timeAfter-time<<" ms"<<endl;
       }
       catch (const XMLException& toCatch) {
           char* message = XMLString::transcode(toCatch.getMessage());
           cout << "Exception message is: \n"
                << message << "\n";
           XMLString::release(&message);
           return -1;
       }
       catch (const SAXParseException& toCatch) {
           char* message = XMLString::transcode(toCatch.getMessage());
           cout << "Exception message is: \n"
                << message << "\n";
           XMLString::release(&message);
           return -1;
       }
       catch (...) {
           cout << "Unexpected Exception \n" ;
           return -1;
       }

       delete parser;
       delete docHandler;
       return 0;
   }
