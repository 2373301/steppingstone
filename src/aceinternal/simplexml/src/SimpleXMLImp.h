
#ifndef SIMPLE_XML_IMP_HPP
#define SIMPLE_XML_IMP_HPP

#include "SimpleXML.h"
#include "tinyxml.h"

class TiElement : public Element
{
public:
	TiElement();

	virtual ~TiElement();

public:
	// element query operator
	virtual Element * get_element(const string & name);

	virtual Elements get_elements(const string & name);

	virtual Elements & get_elements();

	// element modify operator
	virtual bool add_element(Element * element);

	virtual Element * add_element(const string & name, const string & text = "");

	virtual bool remove_element(Element * element);

	virtual bool remove_elements(const string & name);

	virtual void set_name(const string & name);

	virtual const string & get_name() const;

	virtual void set_text(const string & text);

	virtual const string & get_text() const;

	// attribute query operator
	virtual Attribute * get_attribute(const string & name);

	virtual Attributes get_attributes () const;

	// attribute modify operator
	virtual void set_attribute (const string &name, const string &value);

	virtual void set_attribute (const string &name, const char * value);

	virtual void set_attribute (const string &name, bool value);

	virtual void set_attribute (const string &name, int value);

	virtual void set_attribute (const string &name, double value);

	virtual bool remove_attribute(const string & name);

public:
	void save(TiXmlNode * tixml_node);

protected:
	TiXmlElement * convertToTiXMLElement();

private:
	string m_name;

	string m_text;

	Attributes m_attributes;

	Elements m_child_elements;
};

class TiDocument : public Document
{
public:
	TiDocument();
	
	virtual ~TiDocument();
public:
	// load and save
	virtual bool load_file(const string & file_name);

	virtual bool load_string(const string & xml_str);

	virtual bool save_file(const string & file_name);

	virtual string get_xml_string();

	// element operator
	virtual Element * get_root();

	virtual void set_root(Element * root_element);

	virtual Element * make_element(const string & name, Element * parent = NULL);

	virtual void clear();
protected:
	bool loadElement(TiXmlElement * xml_element, TiElement * parent);

	bool loadElementAttribute(TiXmlElement * xml_element, TiElement * ti_element);

	TiXmlDocument * convertToTiXML();

	void saveDeclaration(TiXmlDocument * tixml_doc);

protected:
private:
	TiElement * m_root;
};

#endif