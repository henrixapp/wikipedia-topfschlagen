#ifndef SAXWIKIHANDLER_H
#define SAXWIKIHANDLER_H

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
 * $Id: SAXCountHandlers.hpp 679377 2008-07-24 11:56:42Z borisk $
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/sax/HandlerBase.hpp>

XERCES_CPP_NAMESPACE_USE

XERCES_CPP_NAMESPACE_BEGIN
class AttributeList;
XERCES_CPP_NAMESPACE_END
#include <sstream>
//#include "database/sqliteconnection.h"
#include "wikiarticle.h"
#include "treewikiarticle.h"
#include "linktree.h"
enum state_parser
{
    Parsing_title,
    Parsing_for_link,
            Parsing_for_link_a,//einfall
    Parsing_link,
    Parsing_bullshit
};
extern int count_error_links;
class SAXWikiHandler : public HandlerBase
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    SAXWikiHandler(bool article_adding,string wiki_base,string link_base);
    ~SAXWikiHandler();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    XMLSize_t getElementCount() const
    {
        return fElementCount;
    }

    XMLSize_t getAttrCount() const
    {
        return fAttrCount;
    }

    XMLSize_t getCharacterCount() const
    {
        return fCharacterCount;
    }

    bool getSawErrors() const
    {
        return fSawErrors;
    }

    XMLSize_t getSpaceCount() const
    {
        return fSpaceCount;
    }


    // -----------------------------------------------------------------------
    //  Handlers for the SAX DocumentHandler interface
    // -----------------------------------------------------------------------
    void startElement(const XMLCh* const name, AttributeList& attributes);
    void characters(const XMLCh* const chars, const XMLSize_t length);
    void ignorableWhitespace(const XMLCh* const chars, const XMLSize_t length);
    void resetDocument();


    // -----------------------------------------------------------------------
    //  Handlers for the SAX ErrorHandler interface
    // -----------------------------------------------------------------------
    void warning(const SAXParseException& exc);
    void error(const SAXParseException& exc);
    void fatalError(const SAXParseException& exc);
    void resetErrors();


private:
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fAttrCount
    //  fCharacterCount
    //  fElementCount
    //  fSpaceCount
    //      These are just counters that are run upwards based on the input
    //      from the document handlers.
    //
    //  fSawErrors
    //      This is set by the error handlers, and is queryable later to
    //      see if any errors occured.
    // -----------------------------------------------------------------------
    XMLSize_t       fAttrCount;
    XMLSize_t       fCharacterCount;
    XMLSize_t       fElementCount;
    XMLSize_t       fSpaceCount;
    bool            fSawErrors;
    bool article_adding;
    std::stringstream title;
    std::stringstream article_link;
    state_parser zustand;
    WikiArticle* current;//Der aktuelle artikel
    TreeWikiArticle liste;
    int start_element;//mit welchen element fangen wir an?
    string wiki_filename;
    string link_filename;
    LinkTree links;
};

#endif // SAXWIKIHANDLER_H
