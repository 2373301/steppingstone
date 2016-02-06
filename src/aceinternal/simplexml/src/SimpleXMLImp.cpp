
#include "SimpleXMLImp.h"
#include "tinyxml.h"

class find_ele_by_name
{
public:
	find_ele_by_name(const string & name, bool equal)
	: m_name (name)
	, m_equal(equal)
	{}

	bool operator ()(Element * element)
	{
		if (m_equal)
		{
			return strcmp(m_name.c_str(), element->get_name().c_str()) == 0;
		}
		else
		{
			return strcmp(m_name.c_str(), element->get_name().c_str()) != 0;
		}
	}
private:
	bool m_equal;
	string m_name;
};

class find_attr_by_name
{
public:
	find_attr_by_name(const string & name)
	: m_name(name)
	{}

	bool operator ()(Attribute * attribute)
	{
		return strcmp(m_name.c_str(), attribute->get_name().c_str()) == 0;
	}
protected:
private:
	string m_name;
};

Document * XMLFactory::create_document()
{
	return new TiDocument();
}

TiElement::TiElement()
{
}

TiElement::~TiElement()
{
	for (Attributes::iterator it = m_attributes.begin(); it != m_attributes.end(); ++it)
	{
		delete *it;
	}

	for (Elements::iterator it = m_child_elements.begin(); it != m_child_elements.end(); ++it)
	{
		delete *it;
	}
}

// element query operator
Element * TiElement::get_element(const string & name)
{
	Elements::iterator it = std::find_if(m_child_elements.begin(), m_child_elements.end(), find_ele_by_name(name, true));
	if (it != m_child_elements.end())
	{
		return *it;
	}
	else
	{
		return NULL;
	}
}

Elements TiElement::get_elements(const string & name)
{
	Elements result;
	remove_copy_if(m_child_elements.begin(), m_child_elements.end(), back_insert_iterator<Elements>(result), find_ele_by_name(name, false));
	return result;
}

Elements & TiElement::get_elements()
{
	return m_child_elements;
}

// element modify operator
bool TiElement::add_element(Element * element)
{
	m_child_elements.push_back(element);

	return true;
}

Element * TiElement::add_element(const string & name, const string & text)
{
	TiElement * e = new TiElement();
	e->set_name(name);
	e->set_text(text);

	m_child_elements.push_back(e);

	return e;
}

bool TiElement::remove_element(Element * element)
{
	Elements::iterator it = std::find(m_child_elements.begin(), m_child_elements.end(), element);
	if (it != m_child_elements.end())
	{
		m_child_elements.erase(it);
		return true;
	}

	return false;
}

bool TiElement::remove_elements(const string & name)
{
	bool result = false;
	Elements remove_ele;
	std::remove_copy_if(m_child_elements.begin(), m_child_elements.end(), back_insert_iterator<Elements>(remove_ele), find_ele_by_name(name, false));

	result = remove_ele.size() > 0 ? true : false;

	for (Elements::iterator it = remove_ele.begin(); it != remove_ele.end(); ++it)
	{
		delete *it;
	}

	std::remove_if(m_child_elements.begin(), m_child_elements.end(), find_ele_by_name(name, true));

	return result;
}

void TiElement::set_name(const string & name)
{
	m_name = name;
}

const string & TiElement::get_name() const
{
	return m_name;
}

void TiElement::set_text(const string & text)
{
	m_text = text;
}

const string & TiElement::get_text() const
{
	return m_text;
}

// attribute query operator
Attribute * TiElement::get_attribute(const string & name)
{
	Attributes::iterator it = std::find_if(m_attributes.begin(), m_attributes.end(), find_attr_by_name(name));
	if (it != m_attributes.end())
	{
		return *it;
	}
	else
	{
		return NULL;
	}
}

Attributes TiElement::get_attributes () const
{
	return m_attributes;
}

// attribute modify operator
void TiElement::set_attribute (const string &name, const string &value) 
{
	Attributes::iterator it = std::find_if(m_attributes.begin(), m_attributes.end(), find_attr_by_name(name));
	if (it != m_attributes.end())
	{
		(*it)->set_string(value);
	}
	else
	{
		m_attributes.push_back(new Attribute(name, value));
	}
}

void TiElement::set_attribute (const string &name, const char * value)
{
	string v = value;
	set_attribute(name, v);
}

void TiElement::set_attribute (const string &name, bool value)
{
	string v;
	convert_from_to(value, v);
	set_attribute(name, v);
}

void TiElement::set_attribute (const string &name, int value)
{
	string v;
	convert_from_to(value, v);
	set_attribute(name, v);
}

void TiElement::set_attribute (const string &name, double value)
{
	string v;
	convert_from_to(value, v);
	set_attribute(name, v);
}

bool TiElement::remove_attribute(const string & name)
{
	bool result = false;
	Attributes::iterator it = std::find_if(m_attributes.begin(), m_attributes.end(), find_attr_by_name(name));
	if (it != m_attributes.end())
	{
		m_attributes.erase(it);
		result = true;
	}

	return result;
}

void TiElement::save(TiXmlNode * tixml_node)
{
	TiXmlElement * tixml_ele = convertToTiXMLElement();
	if (NULL == tixml_ele)
	{
		return;
	}

	tixml_node->LinkEndChild(tixml_ele);

	for (Elements::iterator it = m_child_elements.begin(); it != m_child_elements.end(); ++it)
	{
		TiElement * ele = (TiElement *)*it;
		ele->save(tixml_ele);
	}
}

TiXmlElement * TiElement::convertToTiXMLElement()
{
	if (m_name.empty())
	{
		return NULL;
	}

	TiXmlElement * tixml_ele = new TiXmlElement(m_name.c_str());
	if (!m_text.empty())
	{
		TiXmlText * tixml_text = new TiXmlText(m_text.c_str());
		tixml_ele->LinkEndChild(tixml_text);
	}

	for (Attributes::iterator it = m_attributes.begin(); it != m_attributes.end(); ++it)
	{
		Attribute * attr = *it;
		tixml_ele->SetAttribute(attr->get_name().c_str(), attr->get_value().c_str());
	}

	return tixml_ele;
}

// end tielement

TiDocument::TiDocument()
{
	m_root = new TiElement();
}

TiDocument::~TiDocument()
{
	delete m_root;
	m_root = NULL;
}

bool TiDocument::load_file(const string & file_name)
{
	bool result = false;
	auto_ptr<TiXmlDocument> doc(new TiXmlDocument());
	if (!doc->LoadFile(file_name.c_str()))
	{
		//DEF_LOG_ERROR("failed to load xml file : <%s>, xml error : <%s>\n", file_name.c_str(), doc->ErrorDesc());
		return result;
	}

	result = loadElement(doc->RootElement(), NULL);
	return result;
}

bool TiDocument::load_string(const string & xml_str)
{
	bool result = false;

	auto_ptr<TiXmlDocument> doc(new TiXmlDocument());
	if (doc->Parse(xml_str.c_str()) == 0)
	{
		//DEF_LOG_ERROR("failed to load xml string : <%s>, xml error : <%s>\n", xml_str.c_str(), doc->ErrorDesc());
		return result;
	}

	result = loadElement(doc->RootElement(), NULL);

	return result;
}

bool TiDocument::save_file(const string & file_name)
{
	std::auto_ptr<TiXmlDocument> tixml_doc(convertToTiXML());

	return tixml_doc->SaveFile(file_name.c_str());
}

string TiDocument::get_xml_string()
{
	std::auto_ptr<TiXmlDocument> tixml_doc(convertToTiXML());
	TiXmlPrinter printer;
	tixml_doc->Accept(&printer);
	return printer.CStr();
}

// element operator
Element * TiDocument::get_root()
{
	return m_root;
}

void TiDocument::set_root(Element * root_element)
{
	if (NULL == root_element)
	{
		return ;
	}

	if (NULL != m_root)
	{
		delete m_root;
		m_root = NULL;
	}

	m_root = (TiElement *)root_element;
}

Element * TiDocument::make_element(const string & name, Element * parent)
{
	TiElement * ti_ele = new TiElement();
	ti_ele->set_name(name);
	if (NULL != parent)
	{
		parent->add_element(ti_ele);
	}
	return ti_ele;
}

void TiDocument::clear()
{
	delete m_root;

	m_root = new TiElement();
}

bool TiDocument::loadElement(TiXmlElement * xml_element, TiElement * parent)
{
	bool result = false;
	if (NULL == parent)
	{
		parent = dynamic_cast<TiElement *>(m_root);
	}

	if (NULL == parent)
	{
		return false;
	}

	const char * str_name = xml_element->Value();
	if (NULL != str_name)
	{
		parent->set_name(str_name);
	}

	const char * str_text = xml_element->GetText();
	if (NULL != str_text)
	{
		parent->set_text(str_text);
	}

	loadElementAttribute(xml_element, parent);

	result = true;
	TiXmlElement * curr_element = xml_element->FirstChildElement();
	while (NULL != curr_element)
	{
		TiElement * ti_ele = new TiElement();
		parent->add_element(ti_ele);

		if (!loadElement(curr_element, ti_ele))
		{
			result = false;
			break;
		}

		if (curr_element != xml_element->LastChild())
		{
			curr_element = curr_element->NextSiblingElement();
		}
		else
		{
			// over
			break;
		}
	}

	return result;
}

bool TiDocument::loadElementAttribute(TiXmlElement * xml_element, TiElement * ti_element)
{
	string attr_name;
	string attr_value;

	TiXmlAttribute * curr_attr = xml_element->FirstAttribute();
	while (NULL != curr_attr)
	{
		attr_name = curr_attr->Name();
		attr_value = curr_attr->Value();
		ti_element->set_attribute(attr_name, attr_value);
		if (xml_element->LastAttribute() != curr_attr)
		{
			curr_attr = curr_attr->Next();
		}
		else 
		{
			break;
		}
	}

	return true;
}

TiXmlDocument * TiDocument::convertToTiXML()
{
	std::auto_ptr<TiXmlDocument> tixml_doc (new TiXmlDocument ());

	saveDeclaration(tixml_doc.get());

	if (NULL != m_root)
	{
		m_root->save(tixml_doc.get());
	}

	return tixml_doc.release();
}

void TiDocument::saveDeclaration(TiXmlDocument * tixml_doc)
{
	TiXmlDeclaration * dec = new TiXmlDeclaration("1.0", "utf-8", "");

	tixml_doc->LinkEndChild(dec);
}
