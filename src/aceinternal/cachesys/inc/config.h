#ifndef __CACHE_SERVICE_CONFIG__FFFF
#define __CACHE_SERVICE_CONFIG__FFFF

#include "cached.h"
#include "Singleton.h"
#include "SingletonInit.h"
#include "dynamicparse.h"

#define CONTAINER cached::service::CacheConfiguration::instance() 

/// ����XPath����
const ::std::string CONFIG_PATH          = "./cache_config.xml";            /// Ĭ�������ļ�·��
const ::std::string TERMINAL_LISTEN_IP   = "/cached/terminal:listen_ip";    /// �ն˼�����ַ 
const ::std::string TERMINAL_LISTEN_PORT = "/cached/terminal:listen_port";  /// �ն˼����˿�
const ::std::string DATABASE_IP          = "/cached/database:host";         /// ���ݿ�����ʶ
const ::std::string DATABASE_PORT        = "/cached/database:port";         /// ���ݿ����˿�
const ::std::string DATABASE_DB_NAME     = "/cached/database:name";         /// ���ݿ���
const ::std::string DATABASE_USER        = "/cached/database:user";         /// ���ݿ��û���
const ::std::string DATABASE_PASSWORD    = "/cached/database:password";     /// ���ݿ��û���
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

/// �����࣬������������ļ����ṩ��������ֵ�Ĳ�ѯ
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

    /// ��ʼ������
    /// @param filepath �����ļ�·��
    /// @return �ɹ�����True��ʧ�ܷ���False
    bool open(const ::std::string& filepath = "");

    /// xml����ֵ��ѯ����
    /// @param path ָ��xml����ֵ��XPath
    /// @return �ɹ�,����ָ��·����Ӧ���ַ���ֵ;ʧ��,���ؿմ�
    ::std::string lookup(const ::std::string& path);

	IDynamicParser* getParser() { return parser_; }

private:
    // ��ʼ����<XPath, Value>
    void parse(const ::std::string& path, Element* e);
    //  ��֤xml��Ч��
    bool check_config_file(Element *root);
    // ��<XPath,Value>
    std::map<std::string, std::string> config_map_;
	IDynamicParser * parser_ = NULL;
};

}; // namespace service
}; // namespace cached

#endif
