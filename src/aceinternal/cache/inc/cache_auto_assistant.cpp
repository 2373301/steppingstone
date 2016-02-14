/// cache_auto_assistant.cpp
/// Generate by DB CACHE tools, PLEASE DO NOT EDIT IT!

#include "cache_auto_assistant.h"

namespace dbass {

bool AssPlayer::insert(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "INSERT INTO Player SET ";
        query << "guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        query << ",";
        query << "name=" << ::mysqlpp::quote << this->obj_.name();
        query << ",";
        query << "level=" << ::boost::lexical_cast< ::std::string >(this->obj_.level());
        query << ",";
        query << "exp=" << ::boost::lexical_cast< ::std::string >(this->obj_.exp());
        query << ",";
        query << "sex=" << ::boost::lexical_cast< ::std::string >(this->obj_.sex());
        query << ",";
        query << "profession=" << ::boost::lexical_cast< ::std::string >(this->obj_.profession());
        query << ",";
        query << "money=" << ::boost::lexical_cast< ::std::string >(this->obj_.money());
        query << ",";
        query << "coin=" << ::boost::lexical_cast< ::std::string >(this->obj_.coin());
        query << ",";
        query << "src_x=" << ::boost::lexical_cast< ::std::string >(this->obj_.src_x());
        query << ",";
        query << "src_y=" << ::boost::lexical_cast< ::std::string >(this->obj_.src_y());
        query << ",";
        query << "dst_x=" << ::boost::lexical_cast< ::std::string >(this->obj_.dst_x());
        query << ",";
        query << "dst_y=" << ::boost::lexical_cast< ::std::string >(this->obj_.dst_y());
        query << ",";
        query << "seniority=" << ::boost::lexical_cast< ::std::string >(this->obj_.seniority());
        query << ",";
        query << "vip_level=" << ::boost::lexical_cast< ::std::string >(this->obj_.vip_level());
        query << ",";
        query << "coin_exp=" << ::boost::lexical_cast< ::std::string >(this->obj_.coin_exp());
        query << ",";
        query << "prestige=" << ::boost::lexical_cast< ::std::string >(this->obj_.prestige());
        query << ",";
        query << "energy=" << ::boost::lexical_cast< ::std::string >(this->obj_.energy());
        query << ",";
        query << "power=" << ::boost::lexical_cast< ::std::string >(this->obj_.power());
        query << ",";
        query << "training_slot=" << ::boost::lexical_cast< ::std::string >(this->obj_.training_slot());
        query << ",";
        query << "member_slot=" << ::boost::lexical_cast< ::std::string >(this->obj_.member_slot());
        query << ",";
        query << "map_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.map_id());
        query << ",";
        query << "line_no=" << ::boost::lexical_cast< ::std::string >(this->obj_.line_no());
        query << ",";
        query << "stage=" << ::boost::lexical_cast< ::std::string >(this->obj_.stage());
        query << ",";
        query << "direction=" << ::boost::lexical_cast< ::std::string >(this->obj_.direction());
        query << ",";
        query << "form=" << ::boost::lexical_cast< ::std::string >(this->obj_.form());
        query << ",";
        query << "rank=" << ::boost::lexical_cast< ::std::string >(this->obj_.rank());
        query << ",";
        query << "guild=" << ::boost::lexical_cast< ::std::string >(this->obj_.guild());
        query << ",";
        query << "guild_name=" << ::mysqlpp::quote << this->obj_.guild_name();
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.wonder_skill_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.wonder_skill(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "wonder_skill=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.buffers_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.buffers(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "buffers=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.roles_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.roles(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "roles=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.bag_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.bag(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "bag=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.storehouse_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.storehouse(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "storehouse=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.hunt_master_state_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.hunt_master_state(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "hunt_master_state=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.can_recruit_roles_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.can_recruit_roles(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "can_recruit_roles=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.leave_team_roles_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.leave_team_roles(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "leave_team_roles=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        query << "sect=" << ::boost::lexical_cast< ::std::string >(this->obj_.sect());
        query << ",";
        query << "law=" << ::boost::lexical_cast< ::std::string >(this->obj_.law());
        query << ",";
        query << "sthen_will_succ=" << ::boost::lexical_cast< ::std::string >(this->obj_.sthen_will_succ());
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.sthen_queue_stamp_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.sthen_queue_stamp(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "sthen_queue_stamp=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.sthen_queue_times_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.sthen_queue_times(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "sthen_queue_times=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.sthen_queue_flags_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.sthen_queue_flags(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "sthen_queue_flags=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        query << "stheno_hire_checksum=" << ::boost::lexical_cast< ::std::string >(this->obj_.stheno_hire_checksum());
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.stheno_hire_list_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.stheno_hire_list(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "stheno_hire_list=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.stheno_idle_list_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.stheno_idle_list(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "stheno_idle_list=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.stheno_equip_list_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.stheno_equip_list(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "stheno_equip_list=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        query << "announce=" << ::boost::lexical_cast< ::std::string >(this->obj_.announce());
        query << ",";
        query << "task_brief=" << ::boost::lexical_cast< ::std::string >(this->obj_.task_brief());
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.attributes_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.attributes(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "attributes=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        query << "level_limit=" << ::boost::lexical_cast< ::std::string >(this->obj_.level_limit());
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.town_stage_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.town_stage(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "town_stage=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        query << "educate=" << ::boost::lexical_cast< ::std::string >(this->obj_.educate());
        query << ",";
        query << "attach=" << ::boost::lexical_cast< ::std::string >(this->obj_.attach());
        query << ",";
        query << "specshop=" << ::boost::lexical_cast< ::std::string >(this->obj_.specshop());
        query << ",";
        query << "chakra=" << ::boost::lexical_cast< ::std::string >(this->obj_.chakra());
        query << ",";
        query << "last_clean_day_relational_time=" << ::boost::lexical_cast< ::std::string >(this->obj_.last_clean_day_relational_time());
        query << ",";
        query << "last_login_time=" << ::boost::lexical_cast< ::std::string >(this->obj_.last_login_time());
        query << ",";
        query << "last_logoff_time=" << ::boost::lexical_cast< ::std::string >(this->obj_.last_logoff_time());
        query << ",";
        query << "worship=" << ::boost::lexical_cast< ::std::string >(this->obj_.worship());
        query << ",";
        query << "role_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.role_id());
        query << ",";
        query << "first_role=" << ::boost::lexical_cast< ::std::string >(this->obj_.first_role());
        query << ",";
        query << "last_clean_week_relational_time=" << ::boost::lexical_cast< ::std::string >(this->obj_.last_clean_week_relational_time());
        query << ",";
        query << "eggs=" << ::boost::lexical_cast< ::std::string >(this->obj_.eggs());
        query << ",";
        query << "condor=" << ::boost::lexical_cast< ::std::string >(this->obj_.condor());
        query << ",";
        query << "reverence=" << ::boost::lexical_cast< ::std::string >(this->obj_.reverence());
        query << ",";
        query << "vip_exp=" << ::boost::lexical_cast< ::std::string >(this->obj_.vip_exp());
        query << ",";
        query << "boss=" << ::boost::lexical_cast< ::std::string >(this->obj_.boss());
        query << ",";
        query << "answer=" << ::boost::lexical_cast< ::std::string >(this->obj_.answer());
        query << ",";
        query << "addiction=" << ::boost::lexical_cast< ::std::string >(this->obj_.addiction());
        query << ",";
        query << "is_addiction=" << ::boost::lexical_cast< ::std::string >(this->obj_.is_addiction());
        query << ",";
        query << "addiction_status=" << ::boost::lexical_cast< ::std::string >(this->obj_.addiction_status());
        query << ",";
        query << "gift=" << ::boost::lexical_cast< ::std::string >(this->obj_.gift());
        query << ",";
        query << "vip=" << ::boost::lexical_cast< ::std::string >(this->obj_.vip());
        query << ",";
        query << "sthen_flags=" << ::boost::lexical_cast< ::std::string >(this->obj_.sthen_flags());
        query << ",";
        query << "account=" << ::mysqlpp::quote << this->obj_.account();
        query << ",";
        query << "super_gm=" << ::boost::lexical_cast< ::std::string >(this->obj_.super_gm());
        query << ",";
        query << "achievement=" << ::boost::lexical_cast< ::std::string >(this->obj_.achievement());
        query << ",";
        query << "practice=" << ::boost::lexical_cast< ::std::string >(this->obj_.practice());
        query << ",";
        query << "flower_rank=" << ::boost::lexical_cast< ::std::string >(this->obj_.flower_rank());
        query << ",";
        query << "daily_points=" << ::boost::lexical_cast< ::std::string >(this->obj_.daily_points());
        query << ",";
        query << "tower=" << ::boost::lexical_cast< ::std::string >(this->obj_.tower());
        query << ",";
        query << "start_stheno=" << ::boost::lexical_cast< ::std::string >(this->obj_.start_stheno());
        query << ",";
        query << "tavern=" << ::boost::lexical_cast< ::std::string >(this->obj_.tavern());
        query << ",";
        query << "fish=" << ::boost::lexical_cast< ::std::string >(this->obj_.fish());
        query << ",";
        query << "dodge=" << ::boost::lexical_cast< ::std::string >(this->obj_.dodge());
        query << ",";
        query << "guild_icon=" << ::boost::lexical_cast< ::std::string >(this->obj_.guild_icon());
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.wifes_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.wifes(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "wifes=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        query << "player_wife=" << ::boost::lexical_cast< ::std::string >(this->obj_.player_wife());
        query << ",";
        query << "player_activies=" << ::boost::lexical_cast< ::std::string >(this->obj_.player_activies());
        query << ",";
        query << "last_energy_reward_time=" << ::boost::lexical_cast< ::std::string >(this->obj_.last_energy_reward_time());
        query << ",";
        query << "left_energy_reward=" << ::boost::lexical_cast< ::std::string >(this->obj_.left_energy_reward());
        query << ",";
        query << "maze=" << ::boost::lexical_cast< ::std::string >(this->obj_.maze());
        query << ",";
        query << "open_activity=" << ::boost::lexical_cast< ::std::string >(this->obj_.open_activity());
        query << ",";
        query << "open_act=" << ::boost::lexical_cast< ::std::string >(this->obj_.open_act());
        query << ",";
        query << "coin_guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.coin_guid());
        query << ",";
        query << "last_logoff_time_str=" << ::mysqlpp::quote << this->obj_.last_logoff_time_str();
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.give_equip_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.give_equip(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "give_equip=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.equip_reward_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.equip_reward(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "equip_reward=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        query << "follow_role_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.follow_role_id());
        query << ",";
        query << "max_power=" << ::boost::lexical_cast< ::std::string >(this->obj_.max_power());
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.item_id_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.item_id(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "item_id=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.item_num_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.item_num(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "item_num=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.current_title_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.current_title(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "current_title=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        query << "enable_title=" << ::boost::lexical_cast< ::std::string >(this->obj_.enable_title());
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.push_id_list_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.push_id_list(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "push_id_list=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.push_status_list_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.push_status_list(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "push_status_list=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        query << "jifen=" << ::boost::lexical_cast< ::std::string >(this->obj_.jifen());
        query << ",";
        query << "total_money_cost=" << ::boost::lexical_cast< ::std::string >(this->obj_.total_money_cost());
        query << ",";
        query << "total_coin_cost=" << ::boost::lexical_cast< ::std::string >(this->obj_.total_coin_cost());
        query << ",";
        query << "total_seni_cost=" << ::boost::lexical_cast< ::std::string >(this->obj_.total_seni_cost());
        query << ",";
        query << "charge_quest=" << ::boost::lexical_cast< ::std::string >(this->obj_.charge_quest());
        query << ",";
        query << "trial=" << ::boost::lexical_cast< ::std::string >(this->obj_.trial());
        query << ",";
        query << "runes=" << ::boost::lexical_cast< ::std::string >(this->obj_.runes());
        query << ",";
        query << "shl_switch=" << ::boost::lexical_cast< ::std::string >(this->obj_.shl_switch());
        query << ",";
        query << "doubleexp=" << ::boost::lexical_cast< ::std::string >(this->obj_.doubleexp());
        query << ",";
        query << "soul_pve_guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.soul_pve_guid());
        query << ",";
        query << "soul=" << ::boost::lexical_cast< ::std::string >(this->obj_.soul());
        query << ",";
        query << "soul_frag=" << ::boost::lexical_cast< ::std::string >(this->obj_.soul_frag());
        query << ",";
        query << "login_gift=" << ::boost::lexical_cast< ::std::string >(this->obj_.login_gift());
        query << ",";
        query << "super_power=" << ::boost::lexical_cast< ::std::string >(this->obj_.super_power());
        query << ",";
        query << "platform=" << ::mysqlpp::quote << this->obj_.platform();
        query << ",";
        query << "goldcard=" << ::boost::lexical_cast< ::std::string >(this->obj_.goldcard());
        query << ",";
        query << "special_right=" << ::boost::lexical_cast< ::std::string >(this->obj_.special_right());
        query << ",";
        query << "talent=" << ::boost::lexical_cast< ::std::string >(this->obj_.talent());
        query << ",";
        query << "turntable_accrun=" << ::boost::lexical_cast< ::std::string >(this->obj_.turntable_accrun());
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.touch_times_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.touch_times(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "touch_times=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        query << "validate_str=" << ::mysqlpp::quote << this->obj_.validate_str();
        query << ",";
        query << "vip_level_init=" << ::boost::lexical_cast< ::std::string >(this->obj_.vip_level_init());
        query << ",";
        query << "year_card=" << ::boost::lexical_cast< ::std::string >(this->obj_.year_card());
        query << ",";
        query << "time_stamp=" << ::boost::lexical_cast< ::std::string >(this->obj_.time_stamp());
        query << ",";
        query << "horse=" << ::boost::lexical_cast< ::std::string >(this->obj_.horse());
        query << ",";
        query << "robot_validate=" << ::mysqlpp::quote << this->obj_.robot_validate();
        query << ",";
        query << "robot_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.robot_id());
        query << ",";
        query << "horse_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.horse_id());
        query << ",";
        query << "wife_follow=" << ::boost::lexical_cast< ::std::string >(this->obj_.wife_follow());
        query << ",";
        query << "is_yellow_vip=" << ::boost::lexical_cast< ::std::string >(this->obj_.is_yellow_vip());
        query << ",";
        query << "is_yelow_year_vip=" << ::boost::lexical_cast< ::std::string >(this->obj_.is_yelow_year_vip());
        query << ",";
        query << "yellow_vip_level=" << ::boost::lexical_cast< ::std::string >(this->obj_.yellow_vip_level());
        query << ",";
        query << "is_yellow_high_vip=" << ::boost::lexical_cast< ::std::string >(this->obj_.is_yellow_high_vip());
        query << ",";
        query << "open_id_qq=" << ::mysqlpp::quote << this->obj_.open_id_qq();
        query << ",";
        query << "open_id_referrer=" << ::mysqlpp::quote << this->obj_.open_id_referrer();
        query << ",";
        query << "qqspreadsen=" << ::boost::lexical_cast< ::std::string >(this->obj_.qqspreadsen());
        query << ",";
        query << "wf_fatepoint=" << ::boost::lexical_cast< ::std::string >(this->obj_.wf_fatepoint());
        return query.exec();
    }
    catch (const ::mysqlpp::BadQuery& er) {
        if (ER_DUP_ENTRY == er.errnum())
            er_code_ = CAE_DUP_KEY;
        else
            er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// AssPlayer::insert

bool AssPlayer::load(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "SELECT guid,name,level,exp,sex,profession,money,coin,src_x,src_y,dst_x,dst_y,seniority,vip_level,coin_exp,prestige,energy,power,training_slot,member_slot,map_id,line_no,stage,direction,form,rank,guild,guild_name,wonder_skill,buffers,roles,bag,storehouse,hunt_master_state,can_recruit_roles,leave_team_roles,sect,law,sthen_will_succ,sthen_queue_stamp,sthen_queue_times,sthen_queue_flags,stheno_hire_checksum,stheno_hire_list,stheno_idle_list,stheno_equip_list,announce,task_brief,attributes,level_limit,town_stage,educate,attach,specshop,chakra,last_clean_day_relational_time,last_login_time,last_logoff_time,worship,role_id,first_role,last_clean_week_relational_time,eggs,condor,reverence,vip_exp,boss,answer,addiction,is_addiction,addiction_status,gift,vip,sthen_flags,account,super_gm,achievement,practice,flower_rank,daily_points,tower,start_stheno,tavern,fish,dodge,guild_icon,wifes,player_wife,player_activies,last_energy_reward_time,left_energy_reward,maze,open_activity,open_act,coin_guid,last_logoff_time_str,give_equip,equip_reward,follow_role_id,max_power,item_id,item_num,current_title,enable_title,push_id_list,push_status_list,jifen,total_money_cost,total_coin_cost,total_seni_cost,charge_quest,trial,runes,shl_switch,doubleexp,soul_pve_guid,soul,soul_frag,login_gift,super_power,platform,goldcard,special_right,talent,turntable_accrun,touch_times,validate_str,vip_level_init,year_card,time_stamp,horse,robot_validate,robot_id,horse_id,wife_follow,is_yellow_vip,is_yelow_year_vip,yellow_vip_level,is_yellow_high_vip,open_id_qq,open_id_referrer,qqspreadsen,wf_fatepoint FROM Player WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        const ::mysqlpp::StoreQueryResult& res = query.store();
        if (res.size() == 0) {
             er_code_ = CAE_NOT_FOUND;
             er_str_  = "Cannot find Player<<" + ::boost::lexical_cast< ::std::string >(this->obj_.guid()) + ">>";
             return false;
        }
        obj_.set_guid(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(0)));
        obj_.set_name(res.at(0).at(1));
        obj_.set_level(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(2)));
        obj_.set_exp(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(3)));
        obj_.set_sex(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(4)));
        obj_.set_profession(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(5)));
        obj_.set_money(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(6)));
        obj_.set_coin(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(7)));
        obj_.set_src_x(::boost::lexical_cast< ::boost::int32_t >(res.at(0).at(8)));
        obj_.set_src_y(::boost::lexical_cast< ::boost::int32_t >(res.at(0).at(9)));
        obj_.set_dst_x(::boost::lexical_cast< ::boost::int32_t >(res.at(0).at(10)));
        obj_.set_dst_y(::boost::lexical_cast< ::boost::int32_t >(res.at(0).at(11)));
        obj_.set_seniority(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(12)));
        obj_.set_vip_level(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(13)));
        obj_.set_coin_exp(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(14)));
        obj_.set_prestige(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(15)));
        obj_.set_energy(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(16)));
        obj_.set_power(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(17)));
        obj_.set_training_slot(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(18)));
        obj_.set_member_slot(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(19)));
        obj_.set_map_id(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(20)));
        obj_.set_line_no(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(21)));
        obj_.set_stage(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(22)));
        obj_.set_direction(::boost::lexical_cast< ::boost::int32_t >(res.at(0).at(23)));
        obj_.set_form(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(24)));
        obj_.set_rank(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(25)));
        obj_.set_guild(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(26)));
        obj_.set_guild_name(res.at(0).at(27));
        if (!res.at(0).at(28).is_null()) {
            string temp(res.at(0).at(28).data(), res.at(0).at(28).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint32_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
                this->obj_.add_wonder_skill(v);
            }
	    }
        if (!res.at(0).at(29).is_null()) {
            string temp(res.at(0).at(29).data(), res.at(0).at(29).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint64_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
                this->obj_.add_buffers(v);
            }
	    }
        if (!res.at(0).at(30).is_null()) {
            string temp(res.at(0).at(30).data(), res.at(0).at(30).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint64_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
                this->obj_.add_roles(v);
            }
	    }
        if (!res.at(0).at(31).is_null()) {
            string temp(res.at(0).at(31).data(), res.at(0).at(31).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint64_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
                this->obj_.add_bag(v);
            }
	    }
        if (!res.at(0).at(32).is_null()) {
            string temp(res.at(0).at(32).data(), res.at(0).at(32).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint64_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
                this->obj_.add_storehouse(v);
            }
	    }
        if (!res.at(0).at(33).is_null()) {
            string temp(res.at(0).at(33).data(), res.at(0).at(33).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint32_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
                this->obj_.add_hunt_master_state(v);
            }
	    }
        if (!res.at(0).at(34).is_null()) {
            string temp(res.at(0).at(34).data(), res.at(0).at(34).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint32_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
                this->obj_.add_can_recruit_roles(v);
            }
	    }
        if (!res.at(0).at(35).is_null()) {
            string temp(res.at(0).at(35).data(), res.at(0).at(35).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint64_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
                this->obj_.add_leave_team_roles(v);
            }
	    }
        obj_.set_sect(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(36)));
        obj_.set_law(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(37)));
        obj_.set_sthen_will_succ(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(38)));
        if (!res.at(0).at(39).is_null()) {
            string temp(res.at(0).at(39).data(), res.at(0).at(39).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint64_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
                this->obj_.add_sthen_queue_stamp(v);
            }
	    }
        if (!res.at(0).at(40).is_null()) {
            string temp(res.at(0).at(40).data(), res.at(0).at(40).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint32_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
                this->obj_.add_sthen_queue_times(v);
            }
	    }
        if (!res.at(0).at(41).is_null()) {
            string temp(res.at(0).at(41).data(), res.at(0).at(41).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint32_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
                this->obj_.add_sthen_queue_flags(v);
            }
	    }
        obj_.set_stheno_hire_checksum(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(42)));
        if (!res.at(0).at(43).is_null()) {
            string temp(res.at(0).at(43).data(), res.at(0).at(43).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint32_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
                this->obj_.add_stheno_hire_list(v);
            }
	    }
        if (!res.at(0).at(44).is_null()) {
            string temp(res.at(0).at(44).data(), res.at(0).at(44).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint64_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
                this->obj_.add_stheno_idle_list(v);
            }
	    }
        if (!res.at(0).at(45).is_null()) {
            string temp(res.at(0).at(45).data(), res.at(0).at(45).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint64_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
                this->obj_.add_stheno_equip_list(v);
            }
	    }
        obj_.set_announce(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(46)));
        obj_.set_task_brief(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(47)));
        if (!res.at(0).at(48).is_null()) {
            string temp(res.at(0).at(48).data(), res.at(0).at(48).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint32_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
                this->obj_.add_attributes(v);
            }
	    }
        obj_.set_level_limit(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(49)));
        if (!res.at(0).at(50).is_null()) {
            string temp(res.at(0).at(50).data(), res.at(0).at(50).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint64_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
                this->obj_.add_town_stage(v);
            }
	    }
        obj_.set_educate(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(51)));
        obj_.set_attach(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(52)));
        obj_.set_specshop(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(53)));
        obj_.set_chakra(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(54)));
        obj_.set_last_clean_day_relational_time(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(55)));
        obj_.set_last_login_time(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(56)));
        obj_.set_last_logoff_time(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(57)));
        obj_.set_worship(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(58)));
        obj_.set_role_id(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(59)));
        obj_.set_first_role(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(60)));
        obj_.set_last_clean_week_relational_time(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(61)));
        obj_.set_eggs(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(62)));
        obj_.set_condor(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(63)));
        obj_.set_reverence(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(64)));
        obj_.set_vip_exp(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(65)));
        obj_.set_boss(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(66)));
        obj_.set_answer(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(67)));
        obj_.set_addiction(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(68)));
        obj_.set_is_addiction(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(69)));
        obj_.set_addiction_status(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(70)));
        obj_.set_gift(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(71)));
        obj_.set_vip(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(72)));
        obj_.set_sthen_flags(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(73)));
        obj_.set_account(res.at(0).at(74));
        obj_.set_super_gm(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(75)));
        obj_.set_achievement(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(76)));
        obj_.set_practice(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(77)));
        obj_.set_flower_rank(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(78)));
        obj_.set_daily_points(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(79)));
        obj_.set_tower(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(80)));
        obj_.set_start_stheno(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(81)));
        obj_.set_tavern(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(82)));
        obj_.set_fish(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(83)));
        obj_.set_dodge(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(84)));
        obj_.set_guild_icon(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(85)));
        if (!res.at(0).at(86).is_null()) {
            string temp(res.at(0).at(86).data(), res.at(0).at(86).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint64_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
                this->obj_.add_wifes(v);
            }
	    }
        obj_.set_player_wife(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(87)));
        obj_.set_player_activies(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(88)));
        obj_.set_last_energy_reward_time(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(89)));
        obj_.set_left_energy_reward(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(90)));
        obj_.set_maze(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(91)));
        obj_.set_open_activity(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(92)));
        obj_.set_open_act(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(93)));
        obj_.set_coin_guid(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(94)));
        obj_.set_last_logoff_time_str(res.at(0).at(95));
        if (!res.at(0).at(96).is_null()) {
            string temp(res.at(0).at(96).data(), res.at(0).at(96).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint32_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
                this->obj_.add_give_equip(v);
            }
	    }
        if (!res.at(0).at(97).is_null()) {
            string temp(res.at(0).at(97).data(), res.at(0).at(97).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint32_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
                this->obj_.add_equip_reward(v);
            }
	    }
        obj_.set_follow_role_id(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(98)));
        obj_.set_max_power(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(99)));
        if (!res.at(0).at(100).is_null()) {
            string temp(res.at(0).at(100).data(), res.at(0).at(100).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint32_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
                this->obj_.add_item_id(v);
            }
	    }
        if (!res.at(0).at(101).is_null()) {
            string temp(res.at(0).at(101).data(), res.at(0).at(101).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint32_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
                this->obj_.add_item_num(v);
            }
	    }
        if (!res.at(0).at(102).is_null()) {
            string temp(res.at(0).at(102).data(), res.at(0).at(102).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint32_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
                this->obj_.add_current_title(v);
            }
	    }
        obj_.set_enable_title(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(103)));
        if (!res.at(0).at(104).is_null()) {
            string temp(res.at(0).at(104).data(), res.at(0).at(104).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint32_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
                this->obj_.add_push_id_list(v);
            }
	    }
        if (!res.at(0).at(105).is_null()) {
            string temp(res.at(0).at(105).data(), res.at(0).at(105).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint32_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
                this->obj_.add_push_status_list(v);
            }
	    }
        obj_.set_jifen(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(106)));
        obj_.set_total_money_cost(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(107)));
        obj_.set_total_coin_cost(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(108)));
        obj_.set_total_seni_cost(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(109)));
        obj_.set_charge_quest(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(110)));
        obj_.set_trial(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(111)));
        obj_.set_runes(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(112)));
        obj_.set_shl_switch(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(113)));
        obj_.set_doubleexp(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(114)));
        obj_.set_soul_pve_guid(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(115)));
        obj_.set_soul(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(116)));
        obj_.set_soul_frag(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(117)));
        obj_.set_login_gift(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(118)));
        obj_.set_super_power(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(119)));
        obj_.set_platform(res.at(0).at(120));
        obj_.set_goldcard(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(121)));
        obj_.set_special_right(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(122)));
        obj_.set_talent(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(123)));
        obj_.set_turntable_accrun(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(124)));
        if (!res.at(0).at(125).is_null()) {
            string temp(res.at(0).at(125).data(), res.at(0).at(125).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint64_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
                this->obj_.add_touch_times(v);
            }
	    }
        obj_.set_validate_str(res.at(0).at(126));
        obj_.set_vip_level_init(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(127)));
        obj_.set_year_card(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(128)));
        obj_.set_time_stamp(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(129)));
        obj_.set_horse(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(130)));
        obj_.set_robot_validate(res.at(0).at(131));
        obj_.set_robot_id(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(132)));
        obj_.set_horse_id(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(133)));
        obj_.set_wife_follow(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(134)));
        obj_.set_is_yellow_vip(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(135)));
        obj_.set_is_yelow_year_vip(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(136)));
        obj_.set_yellow_vip_level(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(137)));
        obj_.set_is_yellow_high_vip(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(138)));
        obj_.set_open_id_qq(res.at(0).at(139));
        obj_.set_open_id_referrer(res.at(0).at(140));
        obj_.set_qqspreadsen(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(141)));
        obj_.set_wf_fatepoint(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(142)));
        return true;
    }
    catch (const ::mysqlpp::BadQuery& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// AssPlayer::load

bool AssPlayer::update(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "UPDATE Player SET ";
        query << "guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        query << ",";
        query << "name=" << ::mysqlpp::quote << this->obj_.name();
        query << ",";
        query << "level=" << ::boost::lexical_cast< ::std::string >(this->obj_.level());
        query << ",";
        query << "exp=" << ::boost::lexical_cast< ::std::string >(this->obj_.exp());
        query << ",";
        query << "sex=" << ::boost::lexical_cast< ::std::string >(this->obj_.sex());
        query << ",";
        query << "profession=" << ::boost::lexical_cast< ::std::string >(this->obj_.profession());
        query << ",";
        query << "money=" << ::boost::lexical_cast< ::std::string >(this->obj_.money());
        query << ",";
        query << "coin=" << ::boost::lexical_cast< ::std::string >(this->obj_.coin());
        query << ",";
        query << "src_x=" << ::boost::lexical_cast< ::std::string >(this->obj_.src_x());
        query << ",";
        query << "src_y=" << ::boost::lexical_cast< ::std::string >(this->obj_.src_y());
        query << ",";
        query << "dst_x=" << ::boost::lexical_cast< ::std::string >(this->obj_.dst_x());
        query << ",";
        query << "dst_y=" << ::boost::lexical_cast< ::std::string >(this->obj_.dst_y());
        query << ",";
        query << "seniority=" << ::boost::lexical_cast< ::std::string >(this->obj_.seniority());
        query << ",";
        query << "vip_level=" << ::boost::lexical_cast< ::std::string >(this->obj_.vip_level());
        query << ",";
        query << "coin_exp=" << ::boost::lexical_cast< ::std::string >(this->obj_.coin_exp());
        query << ",";
        query << "prestige=" << ::boost::lexical_cast< ::std::string >(this->obj_.prestige());
        query << ",";
        query << "energy=" << ::boost::lexical_cast< ::std::string >(this->obj_.energy());
        query << ",";
        query << "power=" << ::boost::lexical_cast< ::std::string >(this->obj_.power());
        query << ",";
        query << "training_slot=" << ::boost::lexical_cast< ::std::string >(this->obj_.training_slot());
        query << ",";
        query << "member_slot=" << ::boost::lexical_cast< ::std::string >(this->obj_.member_slot());
        query << ",";
        query << "map_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.map_id());
        query << ",";
        query << "line_no=" << ::boost::lexical_cast< ::std::string >(this->obj_.line_no());
        query << ",";
        query << "stage=" << ::boost::lexical_cast< ::std::string >(this->obj_.stage());
        query << ",";
        query << "direction=" << ::boost::lexical_cast< ::std::string >(this->obj_.direction());
        query << ",";
        query << "form=" << ::boost::lexical_cast< ::std::string >(this->obj_.form());
        query << ",";
        query << "rank=" << ::boost::lexical_cast< ::std::string >(this->obj_.rank());
        query << ",";
        query << "guild=" << ::boost::lexical_cast< ::std::string >(this->obj_.guild());
        query << ",";
        query << "guild_name=" << ::mysqlpp::quote << this->obj_.guild_name();
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.wonder_skill_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.wonder_skill(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "wonder_skill=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.buffers_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.buffers(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "buffers=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.roles_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.roles(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "roles=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.bag_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.bag(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "bag=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.storehouse_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.storehouse(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "storehouse=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.hunt_master_state_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.hunt_master_state(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "hunt_master_state=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.can_recruit_roles_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.can_recruit_roles(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "can_recruit_roles=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.leave_team_roles_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.leave_team_roles(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "leave_team_roles=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        query << "sect=" << ::boost::lexical_cast< ::std::string >(this->obj_.sect());
        query << ",";
        query << "law=" << ::boost::lexical_cast< ::std::string >(this->obj_.law());
        query << ",";
        query << "sthen_will_succ=" << ::boost::lexical_cast< ::std::string >(this->obj_.sthen_will_succ());
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.sthen_queue_stamp_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.sthen_queue_stamp(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "sthen_queue_stamp=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.sthen_queue_times_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.sthen_queue_times(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "sthen_queue_times=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.sthen_queue_flags_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.sthen_queue_flags(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "sthen_queue_flags=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        query << "stheno_hire_checksum=" << ::boost::lexical_cast< ::std::string >(this->obj_.stheno_hire_checksum());
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.stheno_hire_list_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.stheno_hire_list(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "stheno_hire_list=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.stheno_idle_list_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.stheno_idle_list(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "stheno_idle_list=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.stheno_equip_list_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.stheno_equip_list(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "stheno_equip_list=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        query << "announce=" << ::boost::lexical_cast< ::std::string >(this->obj_.announce());
        query << ",";
        query << "task_brief=" << ::boost::lexical_cast< ::std::string >(this->obj_.task_brief());
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.attributes_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.attributes(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "attributes=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        query << "level_limit=" << ::boost::lexical_cast< ::std::string >(this->obj_.level_limit());
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.town_stage_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.town_stage(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "town_stage=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        query << "educate=" << ::boost::lexical_cast< ::std::string >(this->obj_.educate());
        query << ",";
        query << "attach=" << ::boost::lexical_cast< ::std::string >(this->obj_.attach());
        query << ",";
        query << "specshop=" << ::boost::lexical_cast< ::std::string >(this->obj_.specshop());
        query << ",";
        query << "chakra=" << ::boost::lexical_cast< ::std::string >(this->obj_.chakra());
        query << ",";
        query << "last_clean_day_relational_time=" << ::boost::lexical_cast< ::std::string >(this->obj_.last_clean_day_relational_time());
        query << ",";
        query << "last_login_time=" << ::boost::lexical_cast< ::std::string >(this->obj_.last_login_time());
        query << ",";
        query << "last_logoff_time=" << ::boost::lexical_cast< ::std::string >(this->obj_.last_logoff_time());
        query << ",";
        query << "worship=" << ::boost::lexical_cast< ::std::string >(this->obj_.worship());
        query << ",";
        query << "role_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.role_id());
        query << ",";
        query << "first_role=" << ::boost::lexical_cast< ::std::string >(this->obj_.first_role());
        query << ",";
        query << "last_clean_week_relational_time=" << ::boost::lexical_cast< ::std::string >(this->obj_.last_clean_week_relational_time());
        query << ",";
        query << "eggs=" << ::boost::lexical_cast< ::std::string >(this->obj_.eggs());
        query << ",";
        query << "condor=" << ::boost::lexical_cast< ::std::string >(this->obj_.condor());
        query << ",";
        query << "reverence=" << ::boost::lexical_cast< ::std::string >(this->obj_.reverence());
        query << ",";
        query << "vip_exp=" << ::boost::lexical_cast< ::std::string >(this->obj_.vip_exp());
        query << ",";
        query << "boss=" << ::boost::lexical_cast< ::std::string >(this->obj_.boss());
        query << ",";
        query << "answer=" << ::boost::lexical_cast< ::std::string >(this->obj_.answer());
        query << ",";
        query << "addiction=" << ::boost::lexical_cast< ::std::string >(this->obj_.addiction());
        query << ",";
        query << "is_addiction=" << ::boost::lexical_cast< ::std::string >(this->obj_.is_addiction());
        query << ",";
        query << "addiction_status=" << ::boost::lexical_cast< ::std::string >(this->obj_.addiction_status());
        query << ",";
        query << "gift=" << ::boost::lexical_cast< ::std::string >(this->obj_.gift());
        query << ",";
        query << "vip=" << ::boost::lexical_cast< ::std::string >(this->obj_.vip());
        query << ",";
        query << "sthen_flags=" << ::boost::lexical_cast< ::std::string >(this->obj_.sthen_flags());
        query << ",";
        query << "account=" << ::mysqlpp::quote << this->obj_.account();
        query << ",";
        query << "super_gm=" << ::boost::lexical_cast< ::std::string >(this->obj_.super_gm());
        query << ",";
        query << "achievement=" << ::boost::lexical_cast< ::std::string >(this->obj_.achievement());
        query << ",";
        query << "practice=" << ::boost::lexical_cast< ::std::string >(this->obj_.practice());
        query << ",";
        query << "flower_rank=" << ::boost::lexical_cast< ::std::string >(this->obj_.flower_rank());
        query << ",";
        query << "daily_points=" << ::boost::lexical_cast< ::std::string >(this->obj_.daily_points());
        query << ",";
        query << "tower=" << ::boost::lexical_cast< ::std::string >(this->obj_.tower());
        query << ",";
        query << "start_stheno=" << ::boost::lexical_cast< ::std::string >(this->obj_.start_stheno());
        query << ",";
        query << "tavern=" << ::boost::lexical_cast< ::std::string >(this->obj_.tavern());
        query << ",";
        query << "fish=" << ::boost::lexical_cast< ::std::string >(this->obj_.fish());
        query << ",";
        query << "dodge=" << ::boost::lexical_cast< ::std::string >(this->obj_.dodge());
        query << ",";
        query << "guild_icon=" << ::boost::lexical_cast< ::std::string >(this->obj_.guild_icon());
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.wifes_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.wifes(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "wifes=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        query << "player_wife=" << ::boost::lexical_cast< ::std::string >(this->obj_.player_wife());
        query << ",";
        query << "player_activies=" << ::boost::lexical_cast< ::std::string >(this->obj_.player_activies());
        query << ",";
        query << "last_energy_reward_time=" << ::boost::lexical_cast< ::std::string >(this->obj_.last_energy_reward_time());
        query << ",";
        query << "left_energy_reward=" << ::boost::lexical_cast< ::std::string >(this->obj_.left_energy_reward());
        query << ",";
        query << "maze=" << ::boost::lexical_cast< ::std::string >(this->obj_.maze());
        query << ",";
        query << "open_activity=" << ::boost::lexical_cast< ::std::string >(this->obj_.open_activity());
        query << ",";
        query << "open_act=" << ::boost::lexical_cast< ::std::string >(this->obj_.open_act());
        query << ",";
        query << "coin_guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.coin_guid());
        query << ",";
        query << "last_logoff_time_str=" << ::mysqlpp::quote << this->obj_.last_logoff_time_str();
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.give_equip_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.give_equip(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "give_equip=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.equip_reward_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.equip_reward(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "equip_reward=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        query << "follow_role_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.follow_role_id());
        query << ",";
        query << "max_power=" << ::boost::lexical_cast< ::std::string >(this->obj_.max_power());
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.item_id_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.item_id(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "item_id=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.item_num_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.item_num(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "item_num=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.current_title_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.current_title(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "current_title=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        query << "enable_title=" << ::boost::lexical_cast< ::std::string >(this->obj_.enable_title());
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.push_id_list_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.push_id_list(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "push_id_list=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.push_status_list_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.push_status_list(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "push_status_list=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        query << "jifen=" << ::boost::lexical_cast< ::std::string >(this->obj_.jifen());
        query << ",";
        query << "total_money_cost=" << ::boost::lexical_cast< ::std::string >(this->obj_.total_money_cost());
        query << ",";
        query << "total_coin_cost=" << ::boost::lexical_cast< ::std::string >(this->obj_.total_coin_cost());
        query << ",";
        query << "total_seni_cost=" << ::boost::lexical_cast< ::std::string >(this->obj_.total_seni_cost());
        query << ",";
        query << "charge_quest=" << ::boost::lexical_cast< ::std::string >(this->obj_.charge_quest());
        query << ",";
        query << "trial=" << ::boost::lexical_cast< ::std::string >(this->obj_.trial());
        query << ",";
        query << "runes=" << ::boost::lexical_cast< ::std::string >(this->obj_.runes());
        query << ",";
        query << "shl_switch=" << ::boost::lexical_cast< ::std::string >(this->obj_.shl_switch());
        query << ",";
        query << "doubleexp=" << ::boost::lexical_cast< ::std::string >(this->obj_.doubleexp());
        query << ",";
        query << "soul_pve_guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.soul_pve_guid());
        query << ",";
        query << "soul=" << ::boost::lexical_cast< ::std::string >(this->obj_.soul());
        query << ",";
        query << "soul_frag=" << ::boost::lexical_cast< ::std::string >(this->obj_.soul_frag());
        query << ",";
        query << "login_gift=" << ::boost::lexical_cast< ::std::string >(this->obj_.login_gift());
        query << ",";
        query << "super_power=" << ::boost::lexical_cast< ::std::string >(this->obj_.super_power());
        query << ",";
        query << "platform=" << ::mysqlpp::quote << this->obj_.platform();
        query << ",";
        query << "goldcard=" << ::boost::lexical_cast< ::std::string >(this->obj_.goldcard());
        query << ",";
        query << "special_right=" << ::boost::lexical_cast< ::std::string >(this->obj_.special_right());
        query << ",";
        query << "talent=" << ::boost::lexical_cast< ::std::string >(this->obj_.talent());
        query << ",";
        query << "turntable_accrun=" << ::boost::lexical_cast< ::std::string >(this->obj_.turntable_accrun());
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.touch_times_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.touch_times(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "touch_times=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        query << "validate_str=" << ::mysqlpp::quote << this->obj_.validate_str();
        query << ",";
        query << "vip_level_init=" << ::boost::lexical_cast< ::std::string >(this->obj_.vip_level_init());
        query << ",";
        query << "year_card=" << ::boost::lexical_cast< ::std::string >(this->obj_.year_card());
        query << ",";
        query << "time_stamp=" << ::boost::lexical_cast< ::std::string >(this->obj_.time_stamp());
        query << ",";
        query << "horse=" << ::boost::lexical_cast< ::std::string >(this->obj_.horse());
        query << ",";
        query << "robot_validate=" << ::mysqlpp::quote << this->obj_.robot_validate();
        query << ",";
        query << "robot_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.robot_id());
        query << ",";
        query << "horse_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.horse_id());
        query << ",";
        query << "wife_follow=" << ::boost::lexical_cast< ::std::string >(this->obj_.wife_follow());
        query << ",";
        query << "is_yellow_vip=" << ::boost::lexical_cast< ::std::string >(this->obj_.is_yellow_vip());
        query << ",";
        query << "is_yelow_year_vip=" << ::boost::lexical_cast< ::std::string >(this->obj_.is_yelow_year_vip());
        query << ",";
        query << "yellow_vip_level=" << ::boost::lexical_cast< ::std::string >(this->obj_.yellow_vip_level());
        query << ",";
        query << "is_yellow_high_vip=" << ::boost::lexical_cast< ::std::string >(this->obj_.is_yellow_high_vip());
        query << ",";
        query << "open_id_qq=" << ::mysqlpp::quote << this->obj_.open_id_qq();
        query << ",";
        query << "open_id_referrer=" << ::mysqlpp::quote << this->obj_.open_id_referrer();
        query << ",";
        query << "qqspreadsen=" << ::boost::lexical_cast< ::std::string >(this->obj_.qqspreadsen());
        query << ",";
        query << "wf_fatepoint=" << ::boost::lexical_cast< ::std::string >(this->obj_.wf_fatepoint());
        query << " WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        return query.exec();
    }
    catch (const ::mysqlpp::BadQuery& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// AssPlayer::update

bool AssPlayer::remove(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query << "DELETE FROM Player WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        return query.exec();
    }
    catch (const ::mysqlpp::BadQuery& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// AssPlayer::remove

bool AssRole::insert(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "INSERT INTO Role SET ";
        query << "guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        query << ",";
        query << "player=" << ::boost::lexical_cast< ::std::string >(this->obj_.player());
        query << ",";
        query << "role_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.role_id());
        query << ",";
        query << "name=" << ::mysqlpp::quote << this->obj_.name();
        query << ",";
        query << "sex=" << ::boost::lexical_cast< ::std::string >(this->obj_.sex());
        query << ",";
        query << "job=" << ::boost::lexical_cast< ::std::string >(this->obj_.job());
        query << ",";
        query << "faction=" << ::boost::lexical_cast< ::std::string >(this->obj_.faction());
        query << ",";
        query << "level=" << ::boost::lexical_cast< ::std::string >(this->obj_.level());
        query << ",";
        query << "exp=" << ::boost::lexical_cast< ::std::string >(this->obj_.exp());
        query << ",";
        query << "postion=" << ::boost::lexical_cast< ::std::string >(this->obj_.postion());
        query << ",";
        query << "normal_skill=" << ::boost::lexical_cast< ::std::string >(this->obj_.normal_skill());
        query << ",";
        query << "extremely_skill=" << ::boost::lexical_cast< ::std::string >(this->obj_.extremely_skill());
        query << ",";
        query << "char_ability=" << ::boost::lexical_cast< ::std::string >(this->obj_.char_ability());
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.culture_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.culture(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "culture=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.pellet1_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.pellet1(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "pellet1=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.pellet_no_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.pellet_no(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "pellet_no=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.attributes_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::int32_t v = this->obj_.attributes(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::int32_t));
            }
            query << "attributes=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.buffers_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.buffers(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "buffers=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.equipments_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.equipments(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "equipments=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.hunt_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.hunt(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "hunt=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        query << "hire_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.hire_id());
        query << ",";
        query << "avatar_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.avatar_id());
        query << ",";
        query << "educate=" << ::boost::lexical_cast< ::std::string >(this->obj_.educate());
        query << ",";
        query << "stomach_start_time=" << ::boost::lexical_cast< ::std::string >(this->obj_.stomach_start_time());
        query << ",";
        query << "quality=" << ::boost::lexical_cast< ::std::string >(this->obj_.quality());
        query << ",";
        query << "training_level=" << ::boost::lexical_cast< ::std::string >(this->obj_.training_level());
        query << ",";
        query << "dodge_skill_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.dodge_skill_id());
        query << ",";
        query << "wife=" << ::boost::lexical_cast< ::std::string >(this->obj_.wife());
        query << ",";
        query << "pellet=" << ::boost::lexical_cast< ::std::string >(this->obj_.pellet());
        query << ",";
        query << "inherit_to=" << ::boost::lexical_cast< ::std::string >(this->obj_.inherit_to());
        query << ",";
        query << "inherit_from=" << ::boost::lexical_cast< ::std::string >(this->obj_.inherit_from());
        query << ",";
        query << "shl_owner=" << ::boost::lexical_cast< ::std::string >(this->obj_.shl_owner());
        query << ",";
        query << "update_fight_power=" << ::boost::lexical_cast< ::std::string >(this->obj_.update_fight_power());
        return query.exec();
    }
    catch (const ::mysqlpp::BadQuery& er) {
        if (ER_DUP_ENTRY == er.errnum())
            er_code_ = CAE_DUP_KEY;
        else
            er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// AssRole::insert

bool AssRole::load(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "SELECT guid,player,role_id,name,sex,job,faction,level,exp,postion,normal_skill,extremely_skill,char_ability,culture,pellet1,pellet_no,attributes,buffers,equipments,hunt,hire_id,avatar_id,educate,stomach_start_time,quality,training_level,dodge_skill_id,wife,pellet,inherit_to,inherit_from,shl_owner,update_fight_power FROM Role WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        const ::mysqlpp::StoreQueryResult& res = query.store();
        if (res.size() == 0) {
             er_code_ = CAE_NOT_FOUND;
             er_str_  = "Cannot find Role<<" + ::boost::lexical_cast< ::std::string >(this->obj_.guid()) + ">>";
             return false;
        }
        obj_.set_guid(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(0)));
        obj_.set_player(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(1)));
        obj_.set_role_id(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(2)));
        obj_.set_name(res.at(0).at(3));
        obj_.set_sex(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(4)));
        obj_.set_job(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(5)));
        obj_.set_faction(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(6)));
        obj_.set_level(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(7)));
        obj_.set_exp(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(8)));
        obj_.set_postion(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(9)));
        obj_.set_normal_skill(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(10)));
        obj_.set_extremely_skill(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(11)));
        obj_.set_char_ability(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(12)));
        if (!res.at(0).at(13).is_null()) {
            string temp(res.at(0).at(13).data(), res.at(0).at(13).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint32_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
                this->obj_.add_culture(v);
            }
	    }
        if (!res.at(0).at(14).is_null()) {
            string temp(res.at(0).at(14).data(), res.at(0).at(14).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint32_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
                this->obj_.add_pellet1(v);
            }
	    }
        if (!res.at(0).at(15).is_null()) {
            string temp(res.at(0).at(15).data(), res.at(0).at(15).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint32_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
                this->obj_.add_pellet_no(v);
            }
	    }
        if (!res.at(0).at(16).is_null()) {
            string temp(res.at(0).at(16).data(), res.at(0).at(16).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::int32_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::int32_t));
                this->obj_.add_attributes(v);
            }
	    }
        if (!res.at(0).at(17).is_null()) {
            string temp(res.at(0).at(17).data(), res.at(0).at(17).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint64_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
                this->obj_.add_buffers(v);
            }
	    }
        if (!res.at(0).at(18).is_null()) {
            string temp(res.at(0).at(18).data(), res.at(0).at(18).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint64_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
                this->obj_.add_equipments(v);
            }
	    }
        if (!res.at(0).at(19).is_null()) {
            string temp(res.at(0).at(19).data(), res.at(0).at(19).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint64_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
                this->obj_.add_hunt(v);
            }
	    }
        obj_.set_hire_id(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(20)));
        obj_.set_avatar_id(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(21)));
        obj_.set_educate(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(22)));
        obj_.set_stomach_start_time(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(23)));
        obj_.set_quality(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(24)));
        obj_.set_training_level(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(25)));
        obj_.set_dodge_skill_id(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(26)));
        obj_.set_wife(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(27)));
        obj_.set_pellet(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(28)));
        obj_.set_inherit_to(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(29)));
        obj_.set_inherit_from(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(30)));
        obj_.set_shl_owner(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(31)));
        obj_.set_update_fight_power(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(32)));
        return true;
    }
    catch (const ::mysqlpp::BadQuery& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// AssRole::load

bool AssRole::update(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "UPDATE Role SET ";
        query << "guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        query << ",";
        query << "player=" << ::boost::lexical_cast< ::std::string >(this->obj_.player());
        query << ",";
        query << "role_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.role_id());
        query << ",";
        query << "name=" << ::mysqlpp::quote << this->obj_.name();
        query << ",";
        query << "sex=" << ::boost::lexical_cast< ::std::string >(this->obj_.sex());
        query << ",";
        query << "job=" << ::boost::lexical_cast< ::std::string >(this->obj_.job());
        query << ",";
        query << "faction=" << ::boost::lexical_cast< ::std::string >(this->obj_.faction());
        query << ",";
        query << "level=" << ::boost::lexical_cast< ::std::string >(this->obj_.level());
        query << ",";
        query << "exp=" << ::boost::lexical_cast< ::std::string >(this->obj_.exp());
        query << ",";
        query << "postion=" << ::boost::lexical_cast< ::std::string >(this->obj_.postion());
        query << ",";
        query << "normal_skill=" << ::boost::lexical_cast< ::std::string >(this->obj_.normal_skill());
        query << ",";
        query << "extremely_skill=" << ::boost::lexical_cast< ::std::string >(this->obj_.extremely_skill());
        query << ",";
        query << "char_ability=" << ::boost::lexical_cast< ::std::string >(this->obj_.char_ability());
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.culture_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.culture(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "culture=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.pellet1_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.pellet1(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "pellet1=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.pellet_no_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.pellet_no(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "pellet_no=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.attributes_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::int32_t v = this->obj_.attributes(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::int32_t));
            }
            query << "attributes=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.buffers_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.buffers(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "buffers=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.equipments_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.equipments(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "equipments=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.hunt_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.hunt(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "hunt=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        query << "hire_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.hire_id());
        query << ",";
        query << "avatar_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.avatar_id());
        query << ",";
        query << "educate=" << ::boost::lexical_cast< ::std::string >(this->obj_.educate());
        query << ",";
        query << "stomach_start_time=" << ::boost::lexical_cast< ::std::string >(this->obj_.stomach_start_time());
        query << ",";
        query << "quality=" << ::boost::lexical_cast< ::std::string >(this->obj_.quality());
        query << ",";
        query << "training_level=" << ::boost::lexical_cast< ::std::string >(this->obj_.training_level());
        query << ",";
        query << "dodge_skill_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.dodge_skill_id());
        query << ",";
        query << "wife=" << ::boost::lexical_cast< ::std::string >(this->obj_.wife());
        query << ",";
        query << "pellet=" << ::boost::lexical_cast< ::std::string >(this->obj_.pellet());
        query << ",";
        query << "inherit_to=" << ::boost::lexical_cast< ::std::string >(this->obj_.inherit_to());
        query << ",";
        query << "inherit_from=" << ::boost::lexical_cast< ::std::string >(this->obj_.inherit_from());
        query << ",";
        query << "shl_owner=" << ::boost::lexical_cast< ::std::string >(this->obj_.shl_owner());
        query << ",";
        query << "update_fight_power=" << ::boost::lexical_cast< ::std::string >(this->obj_.update_fight_power());
        query << " WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        return query.exec();
    }
    catch (const ::mysqlpp::BadQuery& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// AssRole::update

bool AssRole::remove(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query << "DELETE FROM Role WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        return query.exec();
    }
    catch (const ::mysqlpp::BadQuery& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// AssRole::remove

bool AssCoin::insert(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "INSERT INTO Coin SET ";
        query << "guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        query << ",";
        query << "player=" << ::boost::lexical_cast< ::std::string >(this->obj_.player());
        query << ",";
        query << "coin=" << ::boost::lexical_cast< ::std::string >(this->obj_.coin());
        query << ",";
        query << "money=" << ::boost::lexical_cast< ::std::string >(this->obj_.money());
        return query.exec();
    }
    catch (const ::mysqlpp::BadQuery& er) {
        if (ER_DUP_ENTRY == er.errnum())
            er_code_ = CAE_DUP_KEY;
        else
            er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// AssCoin::insert

bool AssCoin::load(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "SELECT guid,player,coin,money FROM Coin WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        const ::mysqlpp::StoreQueryResult& res = query.store();
        if (res.size() == 0) {
             er_code_ = CAE_NOT_FOUND;
             er_str_  = "Cannot find Coin<<" + ::boost::lexical_cast< ::std::string >(this->obj_.guid()) + ">>";
             return false;
        }
        obj_.set_guid(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(0)));
        obj_.set_player(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(1)));
        obj_.set_coin(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(2)));
        obj_.set_money(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(3)));
        return true;
    }
    catch (const ::mysqlpp::BadQuery& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// AssCoin::load

bool AssCoin::update(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "UPDATE Coin SET ";
        query << "guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        query << ",";
        query << "player=" << ::boost::lexical_cast< ::std::string >(this->obj_.player());
        query << ",";
        query << "coin=" << ::boost::lexical_cast< ::std::string >(this->obj_.coin());
        query << ",";
        query << "money=" << ::boost::lexical_cast< ::std::string >(this->obj_.money());
        query << " WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        return query.exec();
    }
    catch (const ::mysqlpp::BadQuery& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// AssCoin::update

bool AssCoin::remove(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query << "DELETE FROM Coin WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        return query.exec();
    }
    catch (const ::mysqlpp::BadQuery& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// AssCoin::remove

bool Assnesttest::insert(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "INSERT INTO nesttest SET ";
        query << "guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        return query.exec();
    }
    catch (const ::mysqlpp::BadQuery& er) {
        if (ER_DUP_ENTRY == er.errnum())
            er_code_ = CAE_DUP_KEY;
        else
            er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// Assnesttest::insert

bool Assnesttest::load(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "SELECT guid FROM nesttest WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        const ::mysqlpp::StoreQueryResult& res = query.store();
        if (res.size() == 0) {
             er_code_ = CAE_NOT_FOUND;
             er_str_  = "Cannot find nesttest<<" + ::boost::lexical_cast< ::std::string >(this->obj_.guid()) + ">>";
             return false;
        }
        obj_.set_guid(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(0)));
        return true;
    }
    catch (const ::mysqlpp::BadQuery& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// Assnesttest::load

bool Assnesttest::update(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "UPDATE nesttest SET ";
        query << "guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        query << " WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        return query.exec();
    }
    catch (const ::mysqlpp::BadQuery& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// Assnesttest::update

bool Assnesttest::remove(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query << "DELETE FROM nesttest WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        return query.exec();
    }
    catch (const ::mysqlpp::BadQuery& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// Assnesttest::remove

bool Asstest::insert(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "INSERT INTO test SET ";
        query << "guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        query << ",";
        query << "t_int32=" << ::boost::lexical_cast< ::std::string >(this->obj_.t_int32());
        query << ",";
        query << "t_uint32=" << ::boost::lexical_cast< ::std::string >(this->obj_.t_uint32());
        query << ",";
        query << "t_string=" << ::mysqlpp::quote << this->obj_.t_string();
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.t_uint32_rep_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.t_uint32_rep(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "t_uint32_rep=" << ::mysqlpp::quote << ssm.str();
        }
        return query.exec();
    }
    catch (const ::mysqlpp::BadQuery& er) {
        if (ER_DUP_ENTRY == er.errnum())
            er_code_ = CAE_DUP_KEY;
        else
            er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// Asstest::insert

bool Asstest::load(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "SELECT guid,t_int32,t_uint32,t_string,t_uint32_rep FROM test WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        const ::mysqlpp::StoreQueryResult& res = query.store();
        if (res.size() == 0) {
             er_code_ = CAE_NOT_FOUND;
             er_str_  = "Cannot find test<<" + ::boost::lexical_cast< ::std::string >(this->obj_.guid()) + ">>";
             return false;
        }
        obj_.set_guid(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(0)));
        obj_.set_t_int32(::boost::lexical_cast< ::boost::int32_t >(res.at(0).at(1)));
        obj_.set_t_uint32(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(2)));
        obj_.set_t_string(res.at(0).at(3));
        if (!res.at(0).at(4).is_null()) {
            string temp(res.at(0).at(4).data(), res.at(0).at(4).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint32_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
                this->obj_.add_t_uint32_rep(v);
            }
	    }
        return true;
    }
    catch (const ::mysqlpp::BadQuery& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// Asstest::load

bool Asstest::update(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "UPDATE test SET ";
        query << "guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        query << ",";
        query << "t_int32=" << ::boost::lexical_cast< ::std::string >(this->obj_.t_int32());
        query << ",";
        query << "t_uint32=" << ::boost::lexical_cast< ::std::string >(this->obj_.t_uint32());
        query << ",";
        query << "t_string=" << ::mysqlpp::quote << this->obj_.t_string();
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.t_uint32_rep_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint32_t v = this->obj_.t_uint32_rep(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint32_t));
            }
            query << "t_uint32_rep=" << ::mysqlpp::quote << ssm.str();
        }
        query << " WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        return query.exec();
    }
    catch (const ::mysqlpp::BadQuery& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// Asstest::update

bool Asstest::remove(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query << "DELETE FROM test WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        return query.exec();
    }
    catch (const ::mysqlpp::BadQuery& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// Asstest::remove

} /// namespace dbass
