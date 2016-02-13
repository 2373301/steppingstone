
#include "SerializeFactory.h"
#include "SQLiteSerialize.h"

namespace cached {
namespace service {

SerializeFactory::SerializeFactory()
{}

SerializeFactory::~SerializeFactory()
{}

Serialize * SerializeFactory::makeSerialize(const string & serialize_type)
{
	Serialize * result = NULL;
	if (serialize_type == SQLITE_TYPE)
	{
		result = new SQLiteSerialize();
	}
	else if (serialize_type == SQLITE_TYPE)
	{
	}
	else
	{
	}

	if (NULL == result)
	{
		//DEF_LOG_ERROR("Cannot make the serialize object : %s", serialize_type.c_str());
	}
	return result;
}

};
};