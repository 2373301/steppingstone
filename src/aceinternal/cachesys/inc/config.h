#ifndef __CACHE_SERVICE_CONFIG__FFFF
#define __CACHE_SERVICE_CONFIG__FFFF

#include "cached.h"
#include "Singleton.h"
#include "SingletonInit.h"
#include "dynamicparse.h"

#define CONTAINER cached::service::CacheConfiguration::instance() 

/// 配置XPath常量
const ::std::string CONFIG_PATH          = "./cache_config.xml";            /// 默认配置文件路径
const ::std::string TERMINAL_LISTEN_IP   = "/cached/terminal:listen_ip";    /// 终端监听地址 
const ::std::string TERMINAL_LISTEN_PORT = "/cached/terminal:listen_port";  /// 终端监听端口
const ::std::string DATABASE_IP          = "/cached/database:host";         /// 数据库服务标识
const ::std::string DATABASE_PORT        = "/cached/database:port";         /// 数据库服务端口
const ::std::string DATABASE_DB_NAME     = "/cached/database:name";         /// 数据库名
const ::std::string DATABASE_USER        = "/cached/database:user";         /// 数据库用户名
const ::std::string DATABASE_PASSWORD    = "/cached/database:password";     /// 数据库用户名
const ::std::string SERIALIZE_TYPE       = "/cached/serialize:type";        /// 
const ::std::string SERIALIZE_INTERVAL   = "/cached/serialize:interval";    /// 
const ::std::string SERIALIZE_LISTEN_ADDR   = "/cached/serialize:listen_addr";    /// 

const ::std::string CACHED_LOG_LEVEL			= "/cached/log:level";    /// 
const ::std::string CACHED_LOG_PRINT			= "/cached/log:print";    /// 
const ::std::string LOG_ENABLE_REMOTE	= "/cached/log:enable_remote";    /// 
const ::std::string REMOTE_LOG_ADDR		= "/cached/remotlog:addr";    /// 
const ::std::string REMOTE_LOG_LEVEL	= "/cached/remotlog:LL_DEBUG";    /// 

const ::std::string ENTITY_PATH = "/cached/entitypath:path";

const ::std::string CONFIG_XML_ROOT_NAME        = "cached";
const ::std::string CONFIG_XML_VERIFY_PROPERTY  = "code";
const ::std::string CONFIG_XML_VERIFY_CODE      = "{FBD56DA7-0B8C-4eb6-B69F-382F3A7EF8BB}";

namespace cached {
namespace service {

/// 配置类，负责加载配置文件，提供配置属性值的查询
class CacheConfiguration : public SingletonInit
{
public:
	static CacheConfiguration * instance()
	{
		return Singleton<CacheConfiguration>::instance();
	}

	virtual int init();

	virtual int stop();

	virtual int finit();

    /// 初始化函数
    /// @param filepath 配置文件路径
    /// @return 成功返回True，失败返回False
    bool open(const ::std::string& filepath = "");

    /// xml属性值查询函数
    /// @param path 指定xml属性值的XPath
    /// @return 成功,返回指定路径对应的字符串值;失败,返回空串
    ::std::string lookup(const ::std::string& path);

	IDynamicParser* getParser() { return parser_; }

private:
    // 初始化表<XPath, Value>
    void parse(const ::std::string& path, Element* e);
    //  验证xml有效性
    bool check_config_file(Element *root);
    // 表<XPath,Value>
    std::map<std::string, std::string> config_map_;
	IDynamicParser * parser_ = NULL;
};

}; // namespace service
}; // namespace cached

#endif
