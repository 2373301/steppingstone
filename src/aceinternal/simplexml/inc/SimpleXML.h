
#ifndef SIMPLE_XML_HPP
#define SIMPLE_XML_HPP

#include "typedef.h"
#include <boost/lexical_cast.hpp>

class Element;
class Attribute;

template<class from_t, class to_t>
bool convert_from_to(from_t from_v, to_t & to_v)
{
	try
	{
		to_v = boost::lexical_cast<to_t>(from_v);
		return true;
	}
	catch (boost::bad_lexical_cast & e )
	{	
		e;
//		DEF_LOG_ERROR(_T("bad convert, msg is <%s>\n"), bc->what());
	}

	return false;
}

bool getAttrValue(Element * element, const string & attr_name, string & attr_value);

bool getAttrValue(Element * element, const string & attr_name, int32 & attr_value);

bool getAttrValue(Element * element, const string & attr_name, uint32 & attr_value);

bool getAttrValue(Element * element, const string & attr_name, uint64 & attr_value);

bool getAttrValue(Element * element, const string & attr_name, Uint32Vec_t & attr_value, char * split_token = ";");

bool getAttrValue(Element * element, const string & attr_name, Int32Vec_t & attr_value, char * split_token = ";");

bool getAttrValue(Element * element, const string & attr_name, Uint32Set_t & attr_value, char * split_token = ";");

bool getAttrValue(Element * element, const string & attr_name, Uint64Vec_t & attr_value, char * split_token = ";");

class Document
{
public:
	virtual ~Document()
	{}

	// load and save
	virtual bool load_file(const string & file_name) = 0;

	virtual bool load_string(const string & xml_str) = 0;

	virtual bool save_file(const string & file_name) = 0;

	virtual string get_xml_string() = 0;

	// element operator
	virtual Element * get_root() = 0;

	virtual void set_root(Element * root_element) = 0;

	virtual Element * make_element(const string & name, Element * parent = NULL) = 0;

	virtual void clear() = 0;

protected:
private:
};

typedef list<Element *> Elements;
typedef list<Attribute *> Attributes;

class Element
{
public:
	virtual ~Element()
	{}

	// element query operator
	virtual Element * get_element(const string & name) = 0;

	virtual Elements get_elements(const string & name) = 0;

	virtual Elements & get_elements() = 0;

	// element modify operator
	virtual bool add_element(Element * element) = 0;

	virtual Element * add_element(const string & name, const string & text = "") = 0;

	virtual bool remove_element(Element * element) = 0;

	virtual bool remove_elements(const string & name) = 0;

	virtual void set_name(const string & name) = 0;

	virtual const string & get_name() const = 0;

	virtual void set_text(const string & text) = 0;

	virtual const string & get_text() const = 0;

	// attribute query operator
	virtual Attribute * get_attribute(const string & name) = 0;

	virtual Attributes get_attributes () const = 0;

	// attribute modify operator
	virtual void set_attribute (const string &name, const string &value)  = 0;

	virtual void set_attribute (const string &name, const char * value)  = 0;

	virtual void set_attribute (const string &name, bool vlaue) = 0;

	virtual void set_attribute (const string &name, int value) = 0;

	virtual void set_attribute (const string &name, double value) = 0;

	virtual bool remove_attribute(const string & name) = 0;

protected:
private:
};

class Attribute
{
public:
	Attribute(const string & name, const string & value);

	Attribute(const string & name, bool value);

	Attribute(const string & name, int value);

	Attribute(const string & name, double value);
public:
	const string & get_name() const;

	const string & get_value() const;

	// query operator
	const char * as_cstr() const;

	int as_int(int value = -1) const;

	double as_double(double value = 0) const;

	bool as_bool(bool value = false) const;

	// set operator
	void set_string(const string & value);

	void set_bool(bool value);

	void set_int(int value);

	void set_double(double value);

protected:
	string m_name;
	string m_value;
};

class XMLFactory
{
public:
	static Document * create_document();
};

#endif