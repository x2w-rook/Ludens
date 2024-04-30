#pragma once

#include <doctest.h>
#include "Core/Serialize/Include/XML.h"

using namespace LD;

static bool Equal(const XMLString& xml, const char* str)
{
    size_t len = strlen(str);
    if (xml.Size() != len)
        return false;

    const char* data = xml.Data();
    for (size_t i = 0; i < len; i++)
    {
        if (data[i] != str[i])
            return false;
    }

    return true;
}

TEST_CASE("XML Header")
{
    XMLParserConfig config{};
    XMLParser parser(config);

    {
        auto fuzz = [](XMLParser& parser, const char* xml) {
            Ref<XMLDocument> doc = parser.ParseString(xml);
            CHECK(doc);

            XMLElement* header = doc->GetHeader();
            CHECK(header);
            CHECK(Equal(header->GetName(), "xml"));

            XMLAttribute* attr = header->GetFirstAttribute();
            CHECK(!attr);
        };

        fuzz(parser, "<?xml?>");
        fuzz(parser, "<?xml  ?>");
    }

    {
        auto fuzz = [](XMLParser& parser, const char* xml) {
            Ref<XMLDocument> doc = parser.ParseString(xml);
            CHECK(doc);

            XMLElement* header = doc->GetHeader();
            CHECK(header);
            CHECK(Equal(header->GetName(), "xml"));

            XMLAttribute* attr = header->GetFirstAttribute();
            CHECK(attr);
            CHECK(Equal(attr->GetName(), "version"));
            CHECK(Equal(attr->GetValue(), "1.0"));

            attr = attr->GetNext();
            CHECK(attr);
            CHECK(Equal(attr->GetName(), "encoding"));
            CHECK(Equal(attr->GetValue(), "UTF-8"));

            attr = attr->GetNext();
            CHECK(attr);
            CHECK(Equal(attr->GetName(), "standalone"));
            CHECK(Equal(attr->GetValue(), "no"));

            attr = attr->GetNext();
            CHECK(!attr);
        };

        fuzz(parser, "<?xml version='1.0' encoding='UTF-8' standalone='no'?>");
        fuzz(parser, "<?xml version = '1.0' encoding = 'UTF-8' standalone = 'no' ?>");
    }
}

TEST_CASE("XML Single Element")
{
    XMLParserConfig config{};
    config.SkipHeader = true;

    XMLParser parser(config);

    // single element, no content
    {
        auto fuzz = [](XMLParser& parser, const char* xml) {
            Ref<XMLDocument> doc = parser.ParseString(xml);

            CHECK(doc);

            XMLNode* node = doc->GetChild();
            CHECK(node);
            CHECK(node->GetType() == XMLType::Element);
            XMLElement* element = node->ToElement();
            XMLString name = element->GetName();
            CHECK(Equal(name, "h1"));

            CHECK(!element->GetChild());

            doc = nullptr;
        };

        fuzz(parser, "<h1></h1>");
        fuzz(parser, "< h1 >< /h1 >");
        fuzz(parser, "<h1>  </ h1>");
        fuzz(parser, "<h1/>");
        fuzz(parser, "<h1/ >");
        fuzz(parser, "<h1 />");
    }

    // single element, pure text content
    {
        auto fuzz = [](XMLParser& parser, const char* xml) {
            Ref<XMLDocument> doc = parser.ParseString(xml);

            CHECK(doc);

            XMLNode* node = doc->GetChild();
            CHECK(node);
            CHECK(node->GetType() == XMLType::Element);

            XMLElement* element = node->ToElement();
            XMLString name = element->GetName();
            CHECK(Equal(name, "tag"));

            node = element->GetChild();
            CHECK(node);
            CHECK(node->GetType() == XMLType::Text);

            XMLText* text = node->ToText();
            CHECK(Equal(text->GetText(), "content"));

            doc = nullptr;
        };

        fuzz(parser, "<tag>content</tag>");
        fuzz(parser, "<tag >content< /tag>");
        fuzz(parser, "< tag>content</tag >");
    }

    // single element, attributes
    {
        auto fuzz = [](XMLParser& parser, const char* xml) {
            Ref<XMLDocument> doc = parser.ParseString(xml);

            CHECK(doc);

            XMLNode* node = doc->GetChild();
            CHECK(node);
            CHECK(node->GetType() == XMLType::Element);

            XMLElement* element = node->ToElement();
            CHECK(Equal(element->GetName(), "h1"));

            XMLAttribute* attr = element->GetFirstAttribute();
            CHECK(attr);
            CHECK(Equal(attr->GetName(), "name"));
            CHECK(Equal(attr->GetValue(), "value"));

            doc = nullptr;
        };

        fuzz(parser, "<h1 name='value'></h1>");
        fuzz(parser, "<h1 name=\"value\"></h1>");
        fuzz(parser, "<h1 name = 'value'></h1>");
        fuzz(parser, "<h1 name ='value' ></h1>");
        fuzz(parser, "<h1 name='value'/>");
    }
}

TEST_CASE("XML Top Level Elements")
{
    XMLParserConfig config{};
    config.SkipHeader = true;

    XMLParser parser(config);

    // multiple top level elements
    {
        auto fuzz = [](XMLParser& parser, const char* xml) {
            Ref<XMLDocument> doc = parser.ParseString(xml);
            CHECK(doc);

            XMLNode* node = doc->GetChild();
            CHECK(node);
            CHECK(node->GetType() == XMLType::Element);

            XMLElement* memberdef = node->ToElement();
            CHECK(Equal(memberdef->GetName(), "memberdef"));

            {
                XMLAttribute* kind = memberdef->GetFirstAttribute();
                CHECK(kind);
                CHECK(Equal(kind->GetName(), "kind"));
                CHECK(Equal(kind->GetValue(), "function"));

                XMLAttribute* id = kind->GetNext();
                CHECK(id);
                CHECK(Equal(id->GetName(), "id"));
                CHECK(Equal(id->GetValue(), "1"));

                XMLAttribute* prot = id->GetNext();
                CHECK(prot);
                CHECK(Equal(prot->GetName(), "prot"));
                CHECK(Equal(prot->GetValue(), "public"));
            }

            memberdef = memberdef->GetNext()->ToElement();
            CHECK(Equal(memberdef->GetName(), "memberdef"));

            {
                XMLAttribute* kind = memberdef->GetFirstAttribute();
                CHECK(kind);
                CHECK(Equal(kind->GetName(), "kind"));
                CHECK(Equal(kind->GetValue(), "function"));

                XMLAttribute* id = kind->GetNext();
                CHECK(id);
                CHECK(Equal(id->GetName(), "id"));
                CHECK(Equal(id->GetValue(), "2"));

                XMLAttribute* prot = id->GetNext();
                CHECK(prot);
                CHECK(Equal(prot->GetName(), "prot"));
                CHECK(Equal(prot->GetValue(), "private"));
            }

            memberdef = memberdef->GetNext()->ToElement();
            CHECK(Equal(memberdef->GetName(), "memberdef"));

            {
                XMLAttribute* kind = memberdef->GetFirstAttribute();
                CHECK(kind);
                CHECK(Equal(kind->GetName(), "kind"));
                CHECK(Equal(kind->GetValue(), "function"));

                XMLAttribute* id = kind->GetNext();
                CHECK(id);
                CHECK(Equal(id->GetName(), "id"));
                CHECK(Equal(id->GetValue(), "3"));

                XMLAttribute* prot = id->GetNext();
                CHECK(prot);
                CHECK(Equal(prot->GetName(), "prot"));
                CHECK(Equal(prot->GetValue(), "private"));

                XMLAttribute* inlin = prot->GetNext();
                CHECK(inlin);
                CHECK(Equal(inlin->GetName(), "inline"));
                CHECK(Equal(inlin->GetValue(), "yes"));
            }
            doc = nullptr;
        };

        fuzz(parser, "<memberdef kind='function' id='1' prot='public'></memberdef>\n"
                     "<memberdef kind='function' id='2' prot='private'></memberdef>\n"
                     "<memberdef kind='function' id='3' prot='private' inline='yes'></memberdef>");

        fuzz(parser, "<memberdef kind='function' id='1' prot='public'/>\n"
                     "<memberdef kind='function' id='2' prot='private'/>\n"
                     "< memberdef kind = 'function' id = '3' prot = 'private' inline = 'yes' > < / memberdef >");
    }
}

TEST_CASE("XML Nested Elements")
{
    XMLParserConfig config{};
    config.SkipHeader = true;

    XMLParser parser(config);

    {
        Ref<XMLDocument> doc = parser.ParseString("<p><b><i>bold and italic</i></b></p>");
        CHECK(doc);

        XMLElement* e = doc->GetChild()->ToElement();
        CHECK(e);
        CHECK(Equal(e->GetName(), "p"));

        e = e->GetChild()->ToElement();
        CHECK(e);
        CHECK(Equal(e->GetName(), "b"));

        e = e->GetChild()->ToElement();
        CHECK(e);
        CHECK(Equal(e->GetName(), "i"));

        XMLText* text = e->GetChild()->ToText();
        CHECK(text);
        CHECK(Equal(text->GetText(), "bold and italic"));

        doc = nullptr;
    }

    {
        Ref<XMLDocument> doc = parser.ParseString(
            "<table rows='3' border-radius='10px'><tr>row 1</tr><tr>row 2</tr><tr>row 3</tr></table>");
        CHECK(doc);

        XMLElement* table = doc->GetChild()->ToElement();
        CHECK(table);
        CHECK(Equal(table->GetName(), "table"));

        XMLAttribute* attr = table->GetFirstAttribute();
        CHECK(attr);
        CHECK(Equal(attr->GetName(), "rows"));
        CHECK(Equal(attr->GetValue(), "3"));

        attr = attr->GetNext();
        CHECK(attr);
        CHECK(Equal(attr->GetName(), "border-radius"));
        CHECK(Equal(attr->GetValue(), "10px"));

        attr = attr->GetNext();
        CHECK(!attr);

        XMLElement* row = table->GetChild()->ToElement();
        CHECK(row);
        CHECK(Equal(row->GetName(), "tr"));

        XMLText* text = row->GetChild()->ToText();
        CHECK(text);
        CHECK(Equal(text->GetText(), "row 1"));

        row = row->GetNext()->ToElement();
        CHECK(row);

        text = row->GetChild()->ToText();
        CHECK(text);
        CHECK(Equal(text->GetText(), "row 2"));

        row = row->GetNext()->ToElement();
        CHECK(row);

        text = row->GetChild()->ToText();
        CHECK(text);
        CHECK(Equal(text->GetText(), "row 3"));

        doc = nullptr;
    }
}

TEST_CASE("XML Mixed Content")
{
    XMLParserConfig config{};
    config.SkipHeader = true;

    XMLParser parser(config);

    {
        Ref<XMLDocument> doc = parser.ParseString("<p>xml mixed <b>content</b> test</p>");
        CHECK(doc);

        XMLElement* p = doc->GetChild()->ToElement();
        CHECK(p);
        CHECK(Equal(p->GetName(), "p"));

        XMLNode* content = p->GetChild();
        CHECK(content);
        XMLText* text = content->ToText();
        CHECK(text);
        CHECK(Equal(text->GetText(), "xml mixed "));

        content = content->GetNext();
        CHECK(content);
        XMLElement* b = content->ToElement();
        CHECK(b);
        CHECK(Equal(b->GetName(), "b"));

        text = b->GetChild()->ToText();
        CHECK(text);
        CHECK(Equal(text->GetText(), "content"));

        content = content->GetNext();
        CHECK(content);
        text = content->ToText();
        CHECK(text);
        CHECK(Equal(text->GetText(), " test"));

        doc = nullptr;
    }
}