
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/lexical_cast.hpp>
#include "SimpleXML.h"

bool getAttrValue(Element * element, const string & attr_name, string & attr_value)
{
	Attribute * attr = element->get_attribute(attr_name);
	if (NULL != attr)
	{
		attr_value = attr->as_cstr();
		return true;
	}

	return false;
}

bool getAttrValue(Element * element, const string & attr_name, int32 & attr_value)
{
	string attr_str;
	if (getAttrValue(element, attr_name, attr_str))
	{
		return convert_from_to(attr_str, attr_value);
	}

	return false;
}

bool getAttrValue(Element * element, const string & attr_name, uint32 & attr_value)
{
	string attr_str;
	if (getAttrValue(element, attr_name, attr_str))
	{
		return convert_from_to(attr_str, attr_value);
	}

	return false;
}

bool getAttrValue(Element * element, const string & attr_name, uint64 & attr_value)
{
	string attr_str;
	if (getAttrValue(element, attr_name, attr_str))
	{
		return convert_from_to(attr_str, attr_value);
	}

	return false;
}

bool getAttrValue(Element * element, const string & attr_name, Uint32Vec_t & attr_value, char * split_token)
{
	string attr_str;
	if (!getAttrValue(element, attr_name, attr_str))
	{
		return false;
	}

	uint32 v = 0;
	vector<string> str_vec;
	if (!attr_str.empty())
	{
		boost::split(str_vec, attr_str, boost::is_any_of(split_token));
	}

	for (vector<string>::iterator it = str_vec.begin(); it != str_vec.end(); ++it)
	{
		v = 0;
		if (!convert_from_to(*it, v))
		{
			return false;
		}

		attr_value.push_back(v);
	}

	return true;
}

bool getAttrValue(Element * element, const string & attr_name, Int32Vec_t & attr_value, char * split_token)
{
	string attr_str;
	if (!getAttrValue(element, attr_name, attr_str))
	{
		return false;
	}

	int32 v = 0;
	vector<string> str_vec;
	if (!attr_str.empty())
	{
		boost::split(str_vec, attr_str, boost::is_any_of(split_token));
	}

	for (vector<string>::iterator it = str_vec.begin(); it != str_vec.end(); ++it)
	{
		v = 0;
		if (!convert_from_to(*it, v))
		{
			return false;
		}

		attr_value.push_back(v);
	}

	return true;
}

bool getAttrValue(Element * element, const string & attr_name, Uint32Set_t & attr_value, char * split_token)
{
	string attr_str;
	if (!getAttrValue(element, attr_name, attr_str))
	{
		return false;
	}

	uint32 v = 0;
	vector<string> str_vec;
	if (!attr_str.empty())
	{
		boost::split(str_vec, attr_str, boost::is_any_of(split_token));
	}

	for (vector<string>::iterator it = str_vec.begin(); it != str_vec.end(); ++it)
	{
		v = 0;
		if (!convert_from_to(*it, v))
		{
			return false;
		}

		attr_value.insert(v);
	}

	return true;
}

bool getAttrValue(Element * element, const string & attr_name, Uint64Vec_t & attr_value, char * split_token)
{
	string attr_str;
	if (!getAttrValue(element, attr_name, attr_str))
	{
		return false;
	}

	uint64 v = 0;
	vector<string> str_vec;
	if (!attr_str.empty())
	{
		boost::split(str_vec, attr_str, boost::is_any_of(split_token));
	}

	for (vector<string>::iterator it = str_vec.begin(); it != str_vec.end(); ++it)
	{
		v = 0;
		if (!convert_from_to(*it, v))
		{
			return false;
		}

		attr_value.push_back(v);
	}

	return true;
}

Attribute::Attribute(const string & name, const string & value)
: m_name(name)
, m_value(value)
{
}

Attribute::Attribute(const string & name, bool value)
: m_name(name)
{
	set_bool(value);
}

Attribute::Attribute(const string & name, int value)
{
	set_bool(value ? true : false);
}

Attribute::Attribute(const string & name, double value)
{
	set_bool(value ? true : false);
}

const string & Attribute::get_name() const
{
	return m_name;
}

const string & Attribute::get_value() const
{
	return m_value;
}

// query operator
const char * Attribute::as_cstr() const
{
	return m_value.c_str();
}

int Attribute::as_int(int value) const
{
	convert_from_to((char *)m_value.c_str(), value);
	return value;
}

double Attribute::as_double(double value) const
{
	convert_from_to(m_value, value);
	return value;
}

bool Attribute::as_bool(bool value) const
{
	convert_from_to(m_value, value);
	return value;
}

// set operator
void Attribute::set_string(const string & value)
{
	m_value = value;
}

void Attribute::set_bool(bool value)
{
	convert_from_to(value, m_value);
}

void Attribute::set_int(int value)
{
	convert_from_to(value, m_value);
}

void Attribute::set_double(double value)
{
	convert_from_to(value, m_value);
}