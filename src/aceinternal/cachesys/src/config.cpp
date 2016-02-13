/// config.cpp
/// 
///

#include "config.h"

namespace cached {
namespace service {

int CacheConfiguration::init()
{
	if (open(CONFIG_PATH))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int CacheConfiguration::stop()
{
	return 0;
}

int CacheConfiguration::finit()
{
	return 0;
}

bool CacheConfiguration::open(const ::std::string& filepath)
{
    /// 打开xml文档
	::boost::scoped_ptr<Document> document(XMLFactory::create_document());
    BOOST_ASSERT(document.get() != NULL);
    if (document.get() == NULL) {
		DEF_LOG_ERROR("Cannot create xml document object, le : <%d>\n", ACE_OS::last_error());
        return false;
    }
    if (!document->load_file(filepath)) {
		DEF_LOG_ERROR("Cannot parse the conf file, le : <%d>\n", ACE_OS::last_error());
        return false;
    }
    /// 验证xml文件有效性
    Element *root = document->get_root();
    if (!check_config_file(root)) {
		DEF_LOG_ERROR("Invalid config file, le : <%d>\n", ACE_OS::last_error());
        return false;
    }
    /// 调用 parse() 加载所有属性值
    this->parse("", root);
    return true;
}

void CacheConfiguration::parse(const ::std::string& path, Element* e)
{
    /// 记录当前元素的属性值到《路径-值map》
    ::std::string full_path = path;
    full_path += ::std::string("/");
    full_path += e->get_name();
    const Attributes& as = e->get_attributes();
    BOOST_FOREACH(Attribute* a, as) {
        this->config_map_[full_path + ":" + a->get_name()] = a->get_value();
    }
    /// 递归搜索所有子元素
    Elements es = e->get_elements();
    BOOST_FOREACH(Element* item, es) {
        this->parse(full_path, item);
    }
}

::std::string CacheConfiguration::lookup(const ::std::string& path)
{
    /// 在《路径-值map》中查找属性值
    CacheS2SMap::const_iterator cit = this->config_map_.find(path);
    if (cit == this->config_map_.end()) {
        return ::std::string("");
    }
    else {
        return cit->second;
    }
}

bool CacheConfiguration::check_config_file(Element *root)
{
    /// 验证xml
    /// 方法：验证 root:code 属性是否等于特定的 GUID
    BOOST_ASSERT(root != NULL);
    if (root != NULL){
        if (root->get_name().compare(CONFIG_XML_ROOT_NAME) == 0){
            const Attribute* a = root->get_attribute(CONFIG_XML_VERIFY_PROPERTY);
            BOOST_ASSERT(a);
            if (a == NULL)
                return false;
            else {
                if (a->get_value().compare(CONFIG_XML_VERIFY_CODE) != 0)
                    return false;
                else
                    return true;
            }
        }
        else
            return false; 
    }
    else
        return false;
}

}; // namespace service
}; // namespace cached