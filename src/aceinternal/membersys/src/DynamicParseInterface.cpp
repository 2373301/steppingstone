#include "DynamicParseInterface.h"
#include "DynamicParse.h"


void getJsonFormat(void* msg, std::string & json_format)
{
	DynamicParse::instance()->getJsonFormat((google::protobuf::Message *)msg, json_format);
}
