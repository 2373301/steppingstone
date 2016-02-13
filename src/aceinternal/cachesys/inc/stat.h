/// stat.h
/// 
/// 

#ifndef __CACHE_STAT_HEADER__
#define __CACHE_STAT_HEADER__

#include "cached.h"
#include "Singleton.h"

namespace cached {
namespace service {

class StatCollectorImp
{
public:
	static StatCollectorImp * instance()
	{
		return Singleton<StatCollectorImp>::instance();
		//static StatCollectorImp * s = new StatCollectorImp();
		//return s;
	}
public:
    typedef struct _FunctionStatItem
    {
        _FunctionStatItem() : 
          total_spent_time(0), 
          max_spent_time(0), 
          long_spent_count(0), 
          total_count(0)
        {
        }
        ::boost::uint64_t total_spent_time;
        ::boost::uint64_t max_spent_time;
        ::boost::uint32_t long_spent_count;
        ::boost::uint32_t total_count;
    } FunctionStatItem;
    typedef define_unordered_map< ::std::string, FunctionStatItem > FunctionStatMap;

    void function_stat(::std::string& function_name, ACE_Time_Value& interval)
    {
        ACE_GUARD_RETURN(ACE_Thread_Mutex, mon, lock_, );
        function_stat_i(function_name, interval);
    }

    void dump(::std::ostream& ost)
    {
        for (FunctionStatMap::const_iterator it = function_stat_map_.begin();
          it != function_stat_map_.end();
          ++it){
            ost << it->first;
            ost << " " << it->second.total_spent_time;
            ost << " " << it->second.total_count;
            ost << " " << it->second.max_spent_time;
            ost << " " << it->second.long_spent_count;
            ost << ::std::endl;
        }
    }

    const FunctionStatMap& map(void)
    {
        return function_stat_map_;
    }

private:
    void function_stat_i(::std::string& function_name, ACE_Time_Value& interval)
    {
        FunctionStatMap::iterator it = function_stat_map_.find(function_name);
        if (it == function_stat_map_.end()) {
            FunctionStatItem item;
            it = function_stat_map_.insert(::std::make_pair(function_name, item)).first;
            if (it == function_stat_map_.end())
                return;
        }
        ACE_UINT64 temp = 0;
        interval.to_usec(temp);
        it->second.total_spent_time += temp;
        it->second.total_count++;
        it->second.max_spent_time = it->second.max_spent_time < temp ? temp : it->second.max_spent_time;
        it->second.long_spent_count += temp > 3000 ? 1 : 0;
    }

    FunctionStatMap function_stat_map_;
    ACE_Thread_Mutex lock_;
};
//typedef ACE_Singleton<StatCollectorImp, ACE_Null_Mutex> StatCollector;

class FunctionStatStub
{
public:
    FunctionStatStub(const char* func_name) : func_name_(func_name), interval_(ACE_OS::gettimeofday())
    {
        
    }
    ~FunctionStatStub()
    {
       interval_ = ACE_OS::gettimeofday() - interval_; 
       StatCollectorImp::instance()->function_stat(func_name_, interval_);
    }

private:
    ::std::string func_name_;
    ACE_Time_Value interval_;
};

#ifdef CACHE_HAS_STAT
#define FUNC_STAT cached::service::FunctionStatStub stub(__FUNCTION__)
#else
#define FUNC_STAT
#endif


};  // namespace service
};  // namespace cached

#endif
