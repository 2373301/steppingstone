
#include <boost/filesystem.hpp>
#include <fstream>
#include "ManageResourceImp.h"
#include "share_fun.h"

#define REPORT_RES_LOAD_RESULT(log_info, ...)	\
	if (is_first_time)	\
	{	\
		DEF_LOG_INFO(log_info, ##__VA_ARGS__)	\
	}	

ManageResourceImp::ManageResourceImp()
: m_scene_cfg(NULL)
{
}

ManageResourceImp::~ManageResourceImp()
{
	cleanMapSecondContent(m_file_to_xml_map);
}

const SoulStageInfoMap_t & ManageResourceImp::getSoulStageInfoMap()
{
    return m_manage_soul_stage_res.getSoulStageInfo();
}

const QqCfgInfoMap_t & ManageResourceImp::getQqCfgInfoMap()
{
    return m_manage_qq_cfg_res.getQqCfgInfo();
}


int ManageResourceImp::init(SceneCfg * scene_cfg)
{
	if (NULL == scene_cfg)
	{
		return -1;
	}

	m_scene_cfg = scene_cfg;

	if (loadAllRes() == -1)
	{
		return -1;
	}

	//convertSchemePropertyID();
	return 0;
}

const HorseRoomInfoMap_t & ManageResourceImp::getHorseRoomInfoMap()
{
    return m_manage_horse_room_res.getHorseRoomInfo();
}

const AccOnlineInfo & ManageResourceImp::getAccOnlineInfo()
{
    return m_manage_acc_online_res.getAccOnlineInfo();
}

const RankingTalkInfoMap_t & ManageResourceImp::getRankingTalkInfoMap()
{
    return m_manage_ranking_talk_res.getRankingTalkInfo();
}

const BillListInfoMap_t & ManageResourceImp::getBillListInfoMap()
{
    return m_manage_bill_list_res.getBillListInfo();
}

bool ManageResourceImp::loadResource(const string & resource_name, Document * * xml_doc)
{
	FileToXmlMap_t::iterator it = m_file_to_xml_map.find(resource_name);
	if (it != m_file_to_xml_map.end())
	{
		*xml_doc = it->second;
		return true;
	}

	bool result = false;
	boost::filesystem::path file_path = m_scene_cfg->resource_cfg.resource_dir + resource_name;
	if (!boost::filesystem::exists(file_path))
	{
		DEF_LOG_ERROR("the file does not exist <%s>\n", file_path.string().c_str());
		return result;
	}

	std::auto_ptr<Document> doc(XMLFactory::create_document());
	if (doc->load_file(file_path.string()))
	{
		*xml_doc = doc.release();
		m_file_to_xml_map[resource_name] = *xml_doc;
		result = true;
	}

	if (!result)
	{
		DEF_LOG_ERROR("failed to load the file <%s>\n", file_path.string().c_str());
	}

	return result;
}

const MapResInfoMap_t & ManageResourceImp::getMapResInfo()
{
	return m_manage_map_res.getMapInfoMap();
}

const BattlegroundRewardInfoMap_t & ManageResourceImp::getBattlegroundRewardInfoMap()
{
	return m_manage_battleground_reward_res.getBattlegroundRewardInfo();
}

const BattlegroundDatetimeInfoMap_t & ManageResourceImp::getBattlegroundDatetimeInfoMap()
{
	return m_manage_battleground_datetime_res.getBattlegroundDatetimeInfo();
}

const EnemyResInfoMap_t & ManageResourceImp::getEnemyResInfo()
{
	return m_manage_enemy_res.getEnemyInfo();
}

const SkillResInfoMap_t & ManageResourceImp::getSkillResInfo()
{
	return m_manage_skill_res.getSkillInfoMap();
}

const NpcResInfoMap_t & ManageResourceImp::getNpcResInfo()
{
	return m_manage_npc_res.getNpcInfoMap();
}

const FormResInfoMap_t & ManageResourceImp::getFormResInfo()
{
	return m_manage_form_res.getFormInfoMap();
}

const MonsterResInfoMap_t & ManageResourceImp::getMonsterResInfo()
{
	return m_manage_monster_res.getMonsterInfoMap();
}

const InitializationCfg & ManageResourceImp::getInitializationCfg()
{
	return m_manage_initialization_cfg.getPlayerInitCfg();
}

const WarJobEnrageResInfoMap_t & ManageResourceImp::getWarJobEnrageResInfo()
{
	return m_manage_warjob_res.getWarJobEnrageResInfo();
}

const WarInitResInfo & ManageResourceImp::getWarInitResInfo()
{
	return m_manage_war_init_res.getWarInitResInfo();
}

const PropertyResInfoMap_t & ManageResourceImp::getPropertyResInfo()
{
	return m_manage_property_res.getPropertyInfoMap();
}

const RoleResInfoMap_t & ManageResourceImp::getRoleResInfo()
{
	return m_manage_role_res.getRoleInfoMap();
}

const BufferResInfoMap_t & ManageResourceImp::getBufferResInfo()
{
	return m_manage_buffer_res.getBufferInfo();
}

const HireResInfoMap_t & ManageResourceImp::getHireResInfo()
{
	return m_manage_hire_res.getHireInfoMap();
}

const LevelExpResInfoMap_t & ManageResourceImp::getLevelExpResInfo()
{
	return m_manage_level_exp_res.getLevelExpInfoMap();
}

const ExpAdditionResInfoMap_t & ManageResourceImp::getExpAdditionResInfo()
{
	return m_manage_exp_addition_res.getExpAdditionInfo();
}

const WarScoreResInfoVec_t & ManageResourceImp::getWarScoreResInfo()
{
	return m_manage_war_score_res.getWarScoreInfoVec();
}

const LawResInfoVec_t & ManageResourceImp::getLawInfoVec()
{
	return m_manage_law_res.getLawInfoVec();
}

const ItemResInfoMap_t & ManageResourceImp::getItemInfoMap()
{
	return m_manage_item_res.getItemInfoMap();
}

const BonusResInfoMap_t & ManageResourceImp::getBonusInfoMap()
{
	return m_manage_bonus_res.getBonusInfoMap();
}

const SectResInfoMap_t & ManageResourceImp::getSectInfoMap()
{
	return m_manage_sect_res.getSectInfoMap();
}

const PrandomResInfoMap_t & ManageResourceImp::getPrandomInfoMap()
{
	return m_manage_prandom_res.getPrandomInfoMap();
}

const RandomSkillResInfoMap_t & ManageResourceImp::getRandomSkillInfoMap()
{
	return m_manage_random_skill_res.getRandomSkillInfoMap();
}

const GoldParamResInfoMap_t & ManageResourceImp::getGoldParamResInfoMap()
{
	return m_manage_gold_param_res.getGoldParamInfoMap();
}

const ProhibitWordResVec_t & ManageResourceImp::getProhibitWordResVec()
{
	return m_manage_prohibit_word.getProhibitWordInfoMap();
}

const ProhibitWordResVec_t & ManageResourceImp::getProhibitNameInfoMap()
{
	return m_manage_prohibit_word.getProhibitNameInfoMap();
}

const TalkResInfoMap_t & ManageResourceImp::getTalkResInfoMap()
{
	return m_manage_talk_res.getTalkInfoMap();
}

const ItemStrengResInfoMap_t & ManageResourceImp::getItemStrengResInfoMap()
{
	return m_manage_item_streng_res.getItemStrengInfoMap();
}

const RecruitmentResInfoMap_t & ManageResourceImp::getRecruitmentResInfoMap()
{
	return m_manage_recruitment_res.getRecruitmentInfoMap();
}

const RecruitexpResInfoMap_t & ManageResourceImp::getRecruitexpResInfoMap()
{
	return m_manage_recruitexp_res.getRecruitexpInfoMap();
}

const SystemResInfo & ManageResourceImp::getSystemResInfo()
{
	return m_manage_system_res.getSystemInfo();
}

const Stage1stResInfoMap_t & ManageResourceImp::getStage1stResInfoMap()
{
	return m_manage_stage1st_res.getStage1stInfoMap();
}

const QuestResInfoMap_t & ManageResourceImp::getQuestResInfoMap()
{
	return m_manage_quest_res.getQuestInfoMap();
}

const SynthesisResInfoMap_t & ManageResourceImp::getSynthesisResInfo()
{
	return m_manage_synthesis_res.getSynthesisInfoMap();
}

const RaiseResInfoVec_t & ManageResourceImp::getRaiseResInfo()
{
	return m_manage_raise_res.getRaiseInfoVec();
}

const RaiseRateResInfoVec_t & ManageResourceImp::getRaiseRateResInfo()
{
	return m_manage_raise_rate_res.getRaiseRateInfoVec();
}

const ShopInfoMap & ManageResourceImp::getShopInfoMap()
{
	return m_manage_shop_res.getShopInfoMap();
}

const ArenaResInfoVec_t & ManageResourceImp::getArenaResInfo()
{
	return m_manage_arena_res.getArenaInfoVec();
}

const SpecialShopResInfoMap_t & ManageResourceImp::getSpecialShopInfoMap()
{
	return m_manage_special_shop_res.getSpecialShopInfoMap();
}

ColorInfoMap const&
ManageResourceImp::getColorInfoMap() {
    return m_manage_illusion_res.getColorInfoMap();
}

ChakreInfoMap const&
ManageResourceImp::getChakreInfoMap() {
    return m_manage_chakra_res.getChakreInfoMap();
}

ChakreSelectMap const&
ManageResourceImp::getChakreSelectMap() {
    return m_manage_chakra_res.getChakreSelectMap();
}

ChakroInfoMap const&
ManageResourceImp::getChakroInfoMap() {
    return m_manage_chakra_res.getChakroInfoMap();
}

ChakroSortedVec const&
ManageResourceImp::getChakroSortedInfos() {
    return m_manage_chakra_res.getChakroSortedInfos();
}

ChakrpInfoMap const&
ManageResourceImp::getChakrpInfoMap() {
    return m_manage_chakra_res.getChakrpInfoMap();
}

ChakrpSortedVec const&
ManageResourceImp::getChakrpSortedInfos() {
    return m_manage_chakra_res.getChakrpSortedInfos();
}

const DailyBonusResInfoMap_t & ManageResourceImp::getDailyBonusResInfoMap()
{
	return m_manage_daily_bonus_res.getDailyBonusInfoMap();
}

CondorPreyMap const&
ManageResourceImp::getCondorPreyMap() {
    return m_manage_condor_res.getCondorPreyMap();
}

CondorWeaponMap const&
ManageResourceImp::getCondorWeaponMap() {
    return m_manage_condor_res.getCondorWeaponMap();
}

CondorFormula const&
ManageResourceImp::getCondorFormula() {
    return m_manage_condor_res.getCondorFormula();
}

const EggResInfoVec_t & ManageResourceImp::getEggResInfo()
{
	return m_manage_egg_res.getEggInfo();
}

const BossGuWuResInfoMap_t & ManageResourceImp::getBossGuWuResInfoMap()
{
	return m_manage_boss_guwu_res.getBossGuWuInfo();
}
const BossRewardResInfoMap_t & ManageResourceImp::getBossRewardResInfoMap()
{
	return m_manage_boss_reward_res.getBossRewardInfo();
}
const BossForResInfoMap_t & ManageResourceImp::getBossForResInfoMap()
{
	return m_manage_boss_for_res.getBossForInfo();
}

const BossExpResInfoVec_t & ManageResourceImp::getBossExpResInfo()
{
	return m_manage_boss_exp_res.getBossExpInfo();
}
const HammerResInfoMap_t & ManageResourceImp::getHammerResInfoMap()
{
	return m_manage_hammer_res.getHammerInfoMap();
}

const CardAndJokerResInfoMap_t & ManageResourceImp::getCardAndJokerResInfoMap()
{
	return m_manage_card_and_joker_res.getCardAndJokerInfoMap();
}

const VipexpResInfoMap_t & ManageResourceImp::getVipexpResInfoMap()
{
	return m_manage_vipexp_res.getVipexpInfo();
}

const AnswerResource& ManageResourceImp::getAnswerResource()
{
    return m_manage_answer_res.getAnswerResource();
}

const VipInfoMap_t & ManageResourceImp::getVipInfoMap()
{
	return m_manage_vip_res.getVipInfo();
}

const DailyQuestInfoMap_t & ManageResourceImp::getDailyQuestInfoMap()
{
	return m_manage_daily_quest_res.getDailyQuestInfo();
}

const DailyQuestLibraryInfoVec_t & ManageResourceImp::getDailyQuestLibraryInfo()
{
	return m_manage_daily_quest_library_res.getDailyQuestLibraryInfo();
}

const SystemUnlockRes & ManageResourceImp::getSystemUnlockInfoMap()
{
	return m_manage_system_unlock_res.getSystemUnlockRes();
}

const HangUpRes & ManageResourceImp::getHangUpRes()
{
	return m_manage_hang_up_res.getHangUpRes();
}

const GiftResource & ManageResourceImp::getGiftResource()
{
    return m_manage_gift_res.getResource();
}

const AchievementInfoMap_t & ManageResourceImp::getAchevementInfoMap()
{
	return m_manage_achievement_res.getAchievementInfo();
}

const AchievementRewardMap_t & ManageResourceImp::getAchievementRewardMap()
{
	return m_manage_achievement_res.getAchievementReward();
}

const AchievementUnlockMap_t & ManageResourceImp::getAchievementUnlockMap()
{
	return m_manage_achievement_res.getAchievementUnlock();
}

const DailyPointsEventsMap_t & ManageResourceImp::getDailyPointsEventsMap()
{
	return m_manage_dailypoints_res.getDailyPointsEeventMap();
}

const DailyPointsRewardMap_t & ManageResourceImp::getDailyPointsRewardMap()
{
	return m_manage_dailypoints_res.getDailyPointsRewardMap();
}

const Guild_NityMap_t & ManageResourceImp::getGuildNityMap()
{
	return m_manage_guild_res.getGuildNityMap();
}

const Guild_ActivityVector_t & ManageResourceImp::getGuildActivity()
{
	return m_manage_guild_res.getGuildActivityVector();
}

const SweepStageResInfo & ManageResourceImp::getSweepStageResInfo()
{
	return m_manage_sweep_stage_res.getSweepStageResInfo();
}

const MultiCopyResMap_t & ManageResourceImp::getMultiCopyResMap()
{
	return m_manage_multi_copy_res.getMultiCopyResMap();
}

const FlowerResMap_t & ManageResourceImp::getFlowerResMap()
{
	return m_manage_flower_res_info.getFlowerResInfo();
}

const WineTankResInfoMap_t & ManageResourceImp::getWineTankResInfoMap()
{
	return m_manage_practice_re_info.getWineTankResInfoMap();
}

const GourdResInfo & ManageResourceImp::getGourdResInfo()
{
	return m_manage_practice_re_info.getGourdResInfo();
}

const StomachResInfo & ManageResourceImp::getStomachResInfo()
{
	return m_manage_practice_re_info.getStomachResInfo();
}

const WineIncreaseResInfo & ManageResourceImp::getWineIncreaseResInfo()
{
	return m_manage_practice_re_info.getWineIncreaseResInfo();
}

const WineTankUpgradeResInfo & ManageResourceImp::getWineTankUpgradeResInfo()
{
	return m_manage_practice_re_info.getWineTankUpgradeResInfo();
}

const TrainingPartnersInfoMap_t& 
ManageResourceImp::getTrainingPartnersInfoMap()
{
	return m_manage_training_res.getTrainingPartnersInfoMap();
}

const TrainingQualityMap_t& 
ManageResourceImp::getTrainingQualityMap()
{
	return m_manage_training_res.getTrainingQualityMap();
}

const EscortTerminalInfoMap_t & ManageResourceImp::getEscortTerminalInfoMap()
{
	return m_manage_escort_terminal_res.getEscortTerminalInfo();
}

const EscortCarInfoMap_t & ManageResourceImp::getEscortCarInfoMap()
{
	return m_manage_escort_car_res.getEscortCarInfo();
}
const EscortSystemInfo & ManageResourceImp::getEscortSystemInfo()
{
	return m_manage_escort_system_res.getEscortSystemInfo();
}
const EscortAgencyInfoMap_t & ManageResourceImp::getEscortAgencyInfoMap()
{
	return m_manage_escort_agency_res.getEscortAgencyInfo();
}

const FreeTimeBagVec_t & ManageResourceImp::getFreeTimeBagVec()
{
	return m_manage_free_time_bag.getFreeTimeBagVec();
}

const OfflinehangupInfo & ManageResourceImp::getOfflinehangupInfo()
{
	return m_manage_offline_hangup_res.getOfflinehangupInfo();
}

const TavernInfoMap_t & ManageResourceImp::getTavernInfo()
{
	return m_manage_tavern_res.getTavernInfo();
}

const TavernhireInfoMap_t & ManageResourceImp::getTavernhireInfo()
{
	return m_manage_tavernhire_res.getTavernhireInfo();
}

const TavernprobabilityInfoMap_t & ManageResourceImp::getTavernprobabilityInfo()
{
	return m_manage_tavernprobability_res.getTavernprobabilityInfo();
}

const TavernshopInfoVector_t & ManageResourceImp::getTavernshopInfo()
{
	return m_manage_tavernshop_res.getTavernshopInfo();
}

const GvgInfo & ManageResourceImp::getGvgInfo()
{
	return m_manage_gvg__res.getGvgInfo();
}

const GvgrewardsInfoMap_t & ManageResourceImp::getGvgrewardsInfoMap()
{
	return m_manage_gvgrewards__res.getGvgrewardsInfo();
}

const FishingInfoMap_t & ManageResourceImp::getFishingInfoMap()
{
	return m_manage_fishing__res.getFishingInfo();
}

const RingQuestInfoVec_t & ManageResourceImp::getRingQuestInfoVec()
{
	return m_manage_ring_quest_res.getRingQuestInfo();
}

const RingRandomInfoVec_t & ManageResourceImp::getRingRandomInfoVec()
{
	return m_manage_ring_random_res.getRingRandomInfo();
}

const RingGuildInfoMap_t & ManageResourceImp::getRingGuildInfoMap()
{
	return m_manage_ring_guild_res.getRingGuildInfo();
}

const RingRewardInfoMap_t & ManageResourceImp::getRingRewardInfoMap()
{
	return m_manage_ring_reward_res.getRingRewardInfo();
}

const DodgeInfoMap_t & ManageResourceImp::getDodgeInfoMap()
{
	return m_manage_dodge__res.getDodgeInfo();
}

const PlumInfoMap_t & ManageResourceImp::getPlumInfoMap()
{
	return m_manage_plum__res.getPlumInfo();
}

const DodgeMiniInfoMap_t & ManageResourceImp::getDodgeMiniInfoMap()
{
	return m_manage_dodge_mini_res.getDodgeMiniInfo();
}

const PlumSystemInfo & ManageResourceImp::getPlumSystemInfo()
{
	return m_manage_plum_system_res.getPlumSystemInfo();
}

const PlumRewardInfoVec_t & ManageResourceImp::getPlumRewardInfoVec()
{
	return m_manage_plum_reward_res.getPlumRewardInfo();
}

const PlumJarInfoMap_t & ManageResourceImp::getPlumJarInfoMap()
{
	return m_manage_plum_jar_res.getPlumJarInfo();
}

const TavernspebilityInfoMap_t & ManageResourceImp::getTavernspebilityInfoMap()
{
	return m_manage_tavernspebility__res.getTavernspebilityInfo();
}

const SuitInfoMap_t & ManageResourceImp::getSuitInfoMap()
{
	return m_manage_suit__res.getSuitInfo();
}

const MateupInfoMap_t & ManageResourceImp::getMateupInfoMap()
{
	return m_manage_mateup__res.getMateupInfo();
}

const MateinheritInfoMap_t & ManageResourceImp::getMateinheritInfoMap()
{
	return m_manage_mateinherit__res.getMateinheritInfo();
}

const MatecolorInfoMap_t & ManageResourceImp::getMatecolorInfoMap()
{
	return m_manage_matecolor__res.getMatecolorInfo();
}

const MategateInfoMap_t & ManageResourceImp::getMategateInfoMap()
{
	return m_manage_mategate__res.getMategateInfo();
}

const MateInfoMap_t & ManageResourceImp::getMateInfoMap()
{
	return m_manage_mate__res.getMateInfo();
}

const MailInfoMap_t & ManageResourceImp::getMailInfoMap()
{
	return m_manage_mail__res.getMailInfo();
}

const MateskillInfoMap_t & ManageResourceImp::getMateskillInfoMap()
{
	return m_manage_mateskill__res.getMateskillInfo();
}

const PvpInfo & ManageResourceImp::getPvpInfo()
{
	return m_manage_pvp__res.getPvpInfo();
}

const GamblingInfo & ManageResourceImp::getGamblingInfo()
{
	return m_manage_gambling__res.getGamblingInfo();
}

const PvprewardsInfoVec_t & ManageResourceImp::getPvprewardsInfoVec()
{
	return m_manage_pvprewards__res.getPvprewardsInfo();
}

const Mail_formatInfoMap_t & ManageResourceImp::getMail_formatInfoMap()
{
	return m_manage_mail_format__res.getMail_formatInfo();
}

const Tavern_groupInfoMap_t & ManageResourceImp::getTavern_groupInfoMap()
{
	return m_manage_tavern_group__res.getTavern_groupInfo();
}

const GemgroupInfoVec_t & ManageResourceImp::getGemgroupInfoVec()
{
	return m_manage_gemgroup__res.getGemgroupInfo();
}

const PelletInfoResMap_t & ManageResourceImp::getPelletInfoResMap()
{
	return m_manage_pellet_info_res.getPelletInfoResMap();
}

const MazeInfoMap_t & ManageResourceImp::getMazeInfoMap()
{
	return m_manage_maze__res.getMazeInfo();
}

const MazeexplorationInfo & ManageResourceImp::getMazeexplorationInfo()
{
	return m_manage_mazeexploration__res.getMazeexplorationInfo();
}

const MazerewardsInfoVec_t & ManageResourceImp::getMazerewardsInfoMap()
{
	return m_manage_mazerewards__res.getMazerewardsInfo();
}

const MazeroomInfoVec_t & ManageResourceImp::getMazeroomInfoMap()
{
	return m_manage_mazeroom__res.getMazeroomInfo();
}

const TalklinkInfoMap_t & ManageResourceImp::getTalklinkInfoMap()
{
	return m_manage_talklink__res.getTalklinkInfo();
}

const InheritInfoRes & ManageResourceImp::getInheritInfoRes()
{
	return m_manage_inherit_res.getInheritInfoRes();
}

const AmahInfoMap_t & ManageResourceImp::getAmahInfoMap()
{
	return m_manage_amah__res.getAmahInfo();
}

const OpenactInfoMap_t & ManageResourceImp::getOpenactInfoMap()
{
	return m_manage_openact__res.getOpenactInfo();
}

const DodgeRevokeInfo & ManageResourceImp::getDodgeRevokeInfo()
{
	return m_manage_dodge_revoke_res.getDodgeRevokeInfo();
}

const GemsynthesisInfoMap_t & ManageResourceImp::getGemsynthesisInfoMap()
{
	return m_manage_gemsynthesis__res.getGemsynthesisInfo();
}

const Equip_gettingInfoMap_t & ManageResourceImp::getEquip_gettingInfoMap()
{
	return m_manage_equip_getting__res.getEquip_gettingInfo();
}

const TitleInfoMap_t & ManageResourceImp::getTitleInfoMap()
{
	return m_manage_title__res.getTitleInfo();
}

const SplendidVoteTimeMap_t & ManageResourceImp::getSplendidVoteTimeMap()
{
	return m_manage_splendid_res.getSplendidVoteTimeMap();
}

const SplendidRankRewardMap_t & ManageResourceImp::getSplendidRankRewardMap()
{
	return m_manage_splendid_res.getSplendidRankRewardMap();
}

const SplendidWorshipRewardMap_t & ManageResourceImp::getSplendidWorshipRewardMap()
{
	return m_manage_splendid_res.getSplendidWorshipRewardMap();
}

const RaceInfoVec_t & ManageResourceImp::getRaceInfoVec()
{
	return m_manage_race__res.getRaceInfo();
}

const PushInfoMap_t & ManageResourceImp::getPushInfoMap()
{
	return m_manage_push__res.getPushInfo();
}

const ChargequestInfo & ManageResourceImp::getChargequestInfo()
{
	return m_manage_chargequest__res.getChargequestInfo();
}

const Openact1InfoVec_t & ManageResourceImp::getOpenact1InfoVec()
{
	return m_manage_openact1__res.getOpenact1Info();
}

const Openact2InfoVec_t & ManageResourceImp::getOpenact2InfoVec()
{
	return m_manage_openact2__res.getOpenact2Info();
}

const RunesClassInfoMap_t & ManageResourceImp::getRunesClassInfoMap()
{
	return m_manage_runes_class_res.getRunesClassInfo();
}

const RunesExpInfoMap_t & ManageResourceImp::getRunesExpInfoMap()
{
	return m_manage_runes_exp_res.getRunesExpInfo();
}

const RunesLevelInfoMap_t & ManageResourceImp::getRunesLevelInfoMap()
{
	return m_manage_runes_level_res.getRunesLevelInfo();
}

const RunesSystemInfo & ManageResourceImp::getRunesSystemInfo()
{
	return m_manage_runes_system_res.getRunesSystemInfo();
}

const FiresattackInfoMap_t & ManageResourceImp::getFiresattackInfoMap()
{
	return m_manage_firesattack__res.getFiresattackInfo();
}

const FiresInfo & ManageResourceImp::getFiresInfo()
{
	return m_manage_fires__res.getFiresInfo();
}

const SoulSoulInfoMap_t & ManageResourceImp::getSoulSoulInfoMap()
{
	return m_manage_soul_soul_res.getSoulSoulInfo();
}

const SoulSetInfoMap_t & ManageResourceImp::getSoulSetInfoMap()
{
	return m_manage_soul_set_res.getSoulSetInfo();
}

const SoulChangeInfo & ManageResourceImp::getSoulChangeInfo()
{
	return m_manage_soul_change_res.getSoulChangeInfo();
}

const TreasureShopInfoMap_t & ManageResourceImp::getTreasureShopInfoMap()
{
	return m_manage_treasure_shop_res.getTreasureShopInfo();
}

const DanyaoHechengInfoMap_t & ManageResourceImp::getDanyaoHechengInfoMap()
{
	return m_manage_danyao_hecheng_res.getDanyaoHechengInfo();
}

const LogingiftInfo & ManageResourceImp::getLogingiftInfo()
{
	return m_manage_logingift__res.getLogingiftInfo();
}

const SoulSuperInfoMap_t & ManageResourceImp::getSoulSuperInfoMap()
{
	return m_manage_soul_super_res.getSoulSuperInfo();
}

const GetmoneyInfoVec_t & ManageResourceImp::getGetmoneyInfoVec()
{
	return m_manage_getmoney__res.getGetmoneyInfo();
}

const TargetInfoMap_t & ManageResourceImp::getTargetInfoMap()
{
	return m_manage_target__res.getTargetInfo();
}

const TargetquestInfoVec_t & ManageResourceImp::getTargetquestInfoVec()
{
	return m_manage_targetquest__res.getTargetquestInfo();
}

const GoldCardInfoMap_t & ManageResourceImp::getGoldCardInfoMap()
{
	return m_manage_gold_card_res.getGoldCardInfo();
}

const TalentInfoMap_t & ManageResourceImp::getTalentInfoMap()
{
	return m_manage_talent__res.getTalentInfo();
}

const TalentgroupInfoMap_t & ManageResourceImp::getTalentgroupInfoMap()
{
	return m_manage_talentgroup__res.getTalentgroupInfo();
}

const TalentsystemInfo & ManageResourceImp::getTalentsystemInfo()
{
	return m_manage_talentsystem__res.getTalentsystemInfo();
}

const Pvp2Info & ManageResourceImp::getPvp2Info()
{
	return m_manage_pvp_2_res.getPvp2Info();
}

const QqinviteInfoMap_t & ManageResourceImp::getQqinviteInfoMap()
{
	return m_manage_qqinvite__res.getQqinviteInfo();
}

const QqshareInfo & ManageResourceImp::getQqshareInfo()
{
	return m_manage_qqshare__res.getQqshareInfo();
}

const QqbragInfoMap_t & ManageResourceImp::getQqbragInfoMap()
{
	return m_manage_qqbrag__res.getQqbragInfo();
}

//get_end
int ManageResourceImp::loadAllRes()
{
	bool result = true;

	static bool is_first_time = true;

	REPORT_RES_LOAD_RESULT("start to load enemy.xml, current result is <%d>\n", result);
	Document * xml_doc = NULL;
	if (!loadResource("enemy.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_enemy_res.loadEnemyContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load form.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("form.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_form_res.loadFormContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load map.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("map.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_map_res.loadMapContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load npc.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("npc.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_npc_res.loadNpcContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load skill.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("skill.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_skill_res.loadSkillContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load monster.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("monster.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_monster_res.loadMonsterContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load initialization.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("initialization.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_initialization_cfg.loadPlayerContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load war_job.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("war_job.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_warjob_res.loadWarJobEnrageContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load war_initialization.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("war_initialization.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_war_init_res.loadWarInitContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load property.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("property.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_property_res.loadPropertyContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load role.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("role.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_role_res.loadRoleContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load buffer.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("buff.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_buffer_res.loadBufferContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load hire.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("hire.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_hire_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load roleexp.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("roleexp.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_level_exp_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load expbonus.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("expbonus.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_exp_addition_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load score.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("score.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_war_score_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load law.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("law.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_law_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load item.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("item.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_item_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load bonus.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("bonus.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_bonus_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load menpai.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("menpai.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_sect_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load prandom.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("prandom.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_prandom_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load randomskill.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("randomskill.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_random_skill_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load goldblessme.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("goldblessme.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_gold_param_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load prohibitword.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("prohibitword.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_prohibit_word.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load talk.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("talk.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_talk_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load itemStreng.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("itemStreng.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_item_streng_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load Recruitexp.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("Recruitexp.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_recruitexp_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load Recruitment.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("Recruitment.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_recruitment_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load system.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("system.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_system_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load stage1st.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("stage1st.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_stage1st_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load quest.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("quest.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_quest_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load Synthesis.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("Synthesis.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_synthesis_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load raise.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("raise.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_raise_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load raiserate.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("raiserate.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_raise_rate_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load shop.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("shop.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_shop_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load arena.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("arena.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_arena_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load specialshop.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("specialshop.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_special_shop_res.loadContent(xml_doc) && result;

    REPORT_RES_LOAD_RESULT("start to load %s current result is <%d>\n",
        "illusion.xml", result);
    result = loadIllusionRes() && result;

    REPORT_RES_LOAD_RESULT("start to load %s/%s/%s current result is <%d>\n",
        "chakraself.xml", "chakrabody.xml", "chakraitem.xml", result);
    result = loadChakraRes() && result;

	REPORT_RES_LOAD_RESULT("start to load dailybonus.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("dailybonus.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_daily_bonus_res.loadContent(xml_doc) && result;

    REPORT_RES_LOAD_RESULT("start to load %s/%s/%s current result is <%d>\n",
        "condor.xml", "clearning.xml", "preyawards.xml", result);
    result = loadCondorRes() && result;

	REPORT_RES_LOAD_RESULT("start to load egg.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("egg.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_egg_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load hammer.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("hammer.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_hammer_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load cardandjoker.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("cardandjoker.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_card_and_joker_res.loadContent(xml_doc) && result;


	REPORT_RES_LOAD_RESULT("start to load vipexp.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("vipexp.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_vipexp_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load bossguwu.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("bossguwu.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_boss_guwu_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load bossreward.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("bossreward.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_boss_reward_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load bossfor.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("bossfor.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_boss_for_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load bossexp.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("bossexp.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_boss_exp_res.loadContent(xml_doc) && result;

    REPORT_RES_LOAD_RESULT("start to load %s/%s current result is <%d>\n",
        "world.xml", "worldevent.xml", result);
    result = loadAnswerRes() && result;

	REPORT_RES_LOAD_RESULT("start to load vip.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("vip.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_vip_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load systemunlock.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("systemunlock.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_system_unlock_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load hangup.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("hangup.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_hang_up_res.loadContent(xml_doc) && result;

    REPORT_RES_LOAD_RESULT("start to load gifts.xml current result is <%d>\n", result);
    result = loadGiftRes() && result;

	REPORT_RES_LOAD_RESULT("start to load achieve_fun.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("achieve_fun.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_achievement_res.loadContentAchieveFun(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load raid.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("raid.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_sweep_stage_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load achivevment.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("achivevment.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_achievement_res.loadContentAchievement(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load achieve_score.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("achieve_score.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_achievement_res.loadContentAchieveScore(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load multicopy.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("multicopy.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_multi_copy_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load flower.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("flower.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_flower_res_info.loadContent(xml_doc) && result;
	
	REPORT_RES_LOAD_RESULT("start to load training.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("training.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_practice_re_info.loadWineTank(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load trainingup.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("trainingup.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_practice_re_info.loadWineIncrease(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load trainingup.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("trainingup.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_practice_re_info.loadTankUpgrade(xml_doc) && result;


	REPORT_RES_LOAD_RESULT("start to load traingourd.xml  first time,current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("traingourd.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_practice_re_info.loadGourd(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load trainingbelly.xml second time,current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("trainingbelly.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_practice_re_info.loadStomach(xml_doc) && result;



	REPORT_RES_LOAD_RESULT("start to load partners_hire.xml,current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("partners_hire.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_training_res.loadTrainingPartnersInfo(xml_doc) && result;


	REPORT_RES_LOAD_RESULT("start to load partners.xml,current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("partners.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_training_res.loadTrainingLevelTable(xml_doc) && result;


	REPORT_RES_LOAD_RESULT("start to dailyquest.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("dailyquest.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_daily_quest_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to dailyquestlibrary.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("dailyquestlibrary.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_daily_quest_library_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load dailypoints_event.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("dailypoints_event.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_dailypoints_res.loadDailyPointsEevent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load dailypoints_reward.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("dailypoints_reward.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_dailypoints_res.loadDailyPointsReward(xml_doc) && result;


	REPORT_RES_LOAD_RESULT("start to load confraternity.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("confraternity.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_guild_res.loadGuildNity(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load confrateractivity.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if (!loadResource("confrateractivity.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_guild_res.loadGuildActivity(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load checkpoints.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("checkpoints.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_tower_stage_res.loadContent(xml_doc) && result;
	

	REPORT_RES_LOAD_RESULT("start to load checkpointsguwu.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("checkpointsguwu.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_tower_buff_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load checkpointsrewards.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("checkpointsrewards.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_tower_rewards_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load yabiaotermini.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("yabiaotermini.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_escort_terminal_res.loadContent(xml_doc) && result;



	REPORT_RES_LOAD_RESULT("start to load yabiaosystem.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("yabiaosystem.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_escort_system_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load yabiaoescort.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("yabiaoescort.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_escort_agency_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load yabiaocar.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("yabiaocar.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_escort_car_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load bagopentime.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("bagopentime.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_free_time_bag.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load tavern.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("tavern.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_tavern_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load tavernhire.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("tavernhire.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_tavernhire_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load tavernprobability.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("tavernprobability.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_tavernprobability_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load tavernshop.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("tavernshop.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_tavernshop_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load fishing.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("fishing.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_fishing__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load ringquest.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("ringquest.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_ring_quest_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load ringrandom.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("ringrandom.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_ring_random_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load ringguildreward.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("ringguildreward.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_ring_guild_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load ringreward.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("ringreward.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_ring_reward_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load offlinehangup.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("offlinehangup.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_offline_hangup_res.loadContent(xml_doc) && result;
	
	REPORT_RES_LOAD_RESULT("start to load gvg.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("gvg.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_gvg__res.loadContent(xml_doc) && result;
	
	REPORT_RES_LOAD_RESULT("start to load gvgrewards.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("gvgrewards.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_gvgrewards__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load dodge.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("dodge.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_dodge__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load plum.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("plum.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_plum__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load dodgemini.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("dodgemini.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_dodge_mini_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load dodgerevoke.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("dodgerevoke.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_dodge_revoke_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load plumsystem.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("plumsystem.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_plum_system_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load plumreward.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("plumreward.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_plum_reward_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load plumjar.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("plumjar.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_plum_jar_res.loadContent(xml_doc) && result;

	/****ÕóÓªÕ½****/
	REPORT_RES_LOAD_RESULT("start to load faction.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("faction.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_battleground_datetime_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load factionreward.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("factionreward.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_battleground_reward_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load factionbet.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("factionbet.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_battleground_bet_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load factionbuff.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("factionbuff.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_battleground_buff_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load tavernspebility.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("tavernspebility.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_tavernspebility__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load suit.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("suit.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_suit__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load mateup.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("mateup.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_mateup__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load mateinherit.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("mateinherit.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_mateinherit__res.loadContent(xml_doc) && result;
	
	REPORT_RES_LOAD_RESULT("start to load matecolor.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("matecolor.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_matecolor__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load mateget.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("mateget.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_mategate__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load mate.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("mate.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_mate__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load mail.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("mail.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_mail__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load mateskill.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("mateskill.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_mateskill__res.loadContent(xml_doc) && result;


	REPORT_RES_LOAD_RESULT("start to load pvp.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("pvp.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_pvp__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load gambling.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("gambling.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_gambling__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load pvprewards.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("pvprewards.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_pvprewards__res.loadContent(xml_doc) && result;


	REPORT_RES_LOAD_RESULT("start to load mailformat.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("mailformat.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_mail_format__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load tavern_group.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("tavern_group.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_tavern_group__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load gemgroup.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("gemgroup.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_gemgroup__res.loadContent(xml_doc) && result;


    REPORT_RES_LOAD_RESULT("start to load arrest.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("arrest.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_arrest_list_res.loadContent(xml_doc) && result;

    REPORT_RES_LOAD_RESULT("start to load arrestsystem.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("arrestsystem.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_arrest_controller_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load medicine.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("medicine.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_pellet_info_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load maze.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("maze.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_maze__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load mazeexploration.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("mazeexploration.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_mazeexploration__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load mazerewards.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("mazerewards.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_mazerewards__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load mazeroom.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("mazeroom.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_mazeroom__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load talklink.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("talklink.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_talklink__res.loadContent(xml_doc) && result;

	//REPORT_RES_LOAD_RESULT("start to load heritage.xml current result is <%d>\n", result);
	//xml_doc = NULL;
	//if(!loadResource("heritage.xml", &xml_doc))
	//{
	//	result = false;
	//}
	//result = m_manage_inherit_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load gemsynthesis.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("gemsynthesis.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_gemsynthesis__res.loadContent(xml_doc) && result;


	REPORT_RES_LOAD_RESULT("start to load substitute.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("substitute.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_amah__res.loadContent(xml_doc) && result;

    REPORT_RES_LOAD_RESULT("start to load new_landing.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("new_landing.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_acc_online_res.loadContent(xml_doc) && result;
    

    REPORT_RES_LOAD_RESULT("start to load new_everydaycharge.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("new_everydaycharge.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_section_bonus_res.loadContent(xml_doc) && result;

    REPORT_RES_LOAD_RESULT("start to load new_charge1st.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("new_charge1st.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_first_buy_res.loadContent(xml_doc) && result;

    REPORT_RES_LOAD_RESULT("start to load new_chargereward.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("new_chargereward.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_acc_buy_res.loadContent(xml_doc) && result;
    

    REPORT_RES_LOAD_RESULT("start to load new_chargeinterest.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("new_chargeinterest.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_nextday_interest_res.loadContent(xml_doc) && result;

    REPORT_RES_LOAD_RESULT("start to load new_futures.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("new_futures.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_future_bonus_res.loadContent(xml_doc) && result;

    REPORT_RES_LOAD_RESULT("start to load new_shortcuts.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("new_shortcuts.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_shortcut_bonus_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load friends.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("friends.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_friend_add_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load new_operating.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("new_operating.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_openact__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load fail_score.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("fail_score.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_fail_score_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load equip_getting.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("equip_getting.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_equip_getting__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load chenghao.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("chenghao.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_title__res.loadContent(xml_doc) && result;

    REPORT_RES_LOAD_RESULT("start to load ranking.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("ranking.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_ranking_rule_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load jianzhong.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("jianzhong.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_jian_zhong_res.loadContent(xml_doc) && result;
    
    REPORT_RES_LOAD_RESULT("start to load rankingrewards.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("rankingrewards.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_ranking_reward_res.loadContent(xml_doc) && result;

	
	REPORT_RES_LOAD_RESULT("start to load race.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("race.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_race__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load voting.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("voting.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_splendid_res.loadSplendidVoteTime(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load voting_rewards.xmll current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("voting_rewards.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_splendid_res.loadSplendidRankReward(xml_doc) & result;

	REPORT_RES_LOAD_RESULT("start to load worship_rewards.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("worship_rewards.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_splendid_res.loadSplendidWorshipReward(xml_doc) & result;

	REPORT_RES_LOAD_RESULT("start to load push.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("push.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_push__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load chargequest.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("chargequest.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_chargequest__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load openact1.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("openact1.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_openact1__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load openact2.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("openact2.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_openact2__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load runes_class.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("runes_class.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_runes_class_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load runes_exp.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("runes_exp.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_runes_exp_res.loadContent(xml_doc) && result;
	
	REPORT_RES_LOAD_RESULT("start to load runes_level.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("runes_level.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_runes_level_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load runes_system.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("runes_system.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_runes_system_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load fires.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("fires.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_fires__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load fires_attack.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("fires_attack.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_firesattack__res.loadContent(xml_doc) && result;


    REPORT_RES_LOAD_RESULT("start to load fires_token.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("fires_token.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_shenghuo_ling_res.loadContent(xml_doc) && result;

    REPORT_RES_LOAD_RESULT("start to load fires_unlock.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("fires_unlock.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_shl_unlock_res.loadContent(xml_doc) && result;


    REPORT_RES_LOAD_RESULT("start to load fires_exp.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("fires_exp.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_stage_exp_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load buff_medicine.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("buff_medicine.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_doubleexp_res.loadContent(xml_doc) && result;


    REPORT_RES_LOAD_RESULT("start to load rankingtalk.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("rankingtalk.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_ranking_talk_res.loadContent(xml_doc) && result;
    
    REPORT_RES_LOAD_RESULT("start to load singlecharge.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("singlecharge.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_bill_list_res.loadContent(xml_doc) && result;


	REPORT_RES_LOAD_RESULT("start to load soul.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("soul.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_soul_soul_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load soulset.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("soulset.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_soul_set_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load soulchange.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("soulchange.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_soul_change_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load treasure_shop.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("treasure_shop.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_treasure_shop_res.loadContent(xml_doc) && result;

    REPORT_RES_LOAD_RESULT("start to load soulstage.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("soulstage.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_soul_stage_res.loadContent(xml_doc) && result;

    REPORT_RES_LOAD_RESULT("start to load soulget.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("soulget.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_soul_cfg_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load medicineynthesis.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("medicineynthesis.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_danyao_hecheng_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load 2v2_system.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("2v2_system.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_pair_system_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load 2v2_gambling.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("2v2_gambling.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_pair_gambling_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load 2v2_name.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("2v2_name.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_pair_name_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load 2v2_rewards.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("2v2_rewards.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_pair_rewards_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load new_getgold.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("new_getgold.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_logingift__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load supersoul.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("supersoul.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_soul_super_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load new_getmoney.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("new_getmoney.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_getmoney__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load target.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("target.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_target__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load targetquset.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("targetquset.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_targetquest__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load gold_card.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("gold_card.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_gold_card_res.loadContent(xml_doc) && result;

    REPORT_RES_LOAD_RESULT("start to load lucky_rewards.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("lucky_rewards.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_lucky_rewards_res.loadContent(xml_doc) && result;

    REPORT_RES_LOAD_RESULT("start to load lucky_turntable.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("lucky_turntable.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_lucky_turntable_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load talent.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("talent.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_talent__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load talent_group.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("talent_group.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_talentgroup__res.loadContent(xml_doc) && result;


	REPORT_RES_LOAD_RESULT("start to load talent_systemt.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("talent_systemt.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_talentsystem__res.loadContent(xml_doc) && result;


	REPORT_RES_LOAD_RESULT("start to load webvip.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("webvip.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_web_vip_res.loadContent(xml_doc) && result;


	REPORT_RES_LOAD_RESULT("start to load pvp2.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("pvp2.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_pvp_2_res.loadContent(xml_doc) && result;


    REPORT_RES_LOAD_RESULT("start to load horse.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("horse.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_horse_room_res.loadContent(xml_doc) && result;


    REPORT_RES_LOAD_RESULT("start to load horseatt.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("horseatt.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_horse_attr_res.loadContent(xml_doc) && result;

    REPORT_RES_LOAD_RESULT("start to load ranch.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("ranch.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_ranch_info_res.loadContent(xml_doc) && result;

    REPORT_RES_LOAD_RESULT("start to load ranchset.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("ranchset.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_ranch_cfg_res.loadContent(xml_doc) && result;

    REPORT_RES_LOAD_RESULT("start to load riding.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("riding.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_horseman_levelup_res.loadContent(xml_doc) && result;


    REPORT_RES_LOAD_RESULT("start to load ridinghelp.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("ridinghelp.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_helper_info_res.loadContent(xml_doc) && result;


    REPORT_RES_LOAD_RESULT("start to load qq.xml current result is <%d>\n", result);
    xml_doc = NULL;
    if(!loadResource("qq.xml", &xml_doc))
    {
        result = false;
    }
    result = m_manage_qq_cfg_res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load qq_invite.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("qq_invite.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_qqinvite__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load qq_share.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("qq_share.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_qqshare__res.loadContent(xml_doc) && result;

	REPORT_RES_LOAD_RESULT("start to load qq_sport.xml current result is <%d>\n", result);
	xml_doc = NULL;
	if(!loadResource("qq_sport.xml", &xml_doc))
	{
		result = false;
	}
	result = m_manage_qqbrag__res.loadContent(xml_doc) && result;


	//end
	is_first_time = false;

	return result ? 0 : -1;
}

const HelperInfoInfoVec_t & ManageResourceImp::getHelperInfoInfoVec()
{
    return m_manage_helper_info_res.getHelperInfoInfo();
}

const HorsemanLevelupInfoVec_t & ManageResourceImp::getHorsemanLevelupInfoVec()
{
    return m_manage_horseman_levelup_res.getHorsemanLevelupInfo();
}

const RanchCfgInfoVec_t & ManageResourceImp::getRanchCfgInfoVec()
{
    return m_manage_ranch_cfg_res.getRanchCfgInfo();
}

const RanchInfoInfoVec_t & ManageResourceImp::getRanchInfoInfoVec()
{
    return m_manage_ranch_info_res.getRanchInfoInfo();
}

const HorseAttrInfoVec_t & ManageResourceImp::getHorseAttrInfoVec()
{
    return m_manage_horse_attr_res.getHorseAttrInfo();
}

const SoulCfgInfo & ManageResourceImp::getSoulCfgInfo()
{
    return m_manage_soul_cfg_res.getSoulCfgInfo();
}

const LuckyTurntableInfo & ManageResourceImp::getLuckyTurntableInfo()
{
    return m_manage_lucky_turntable_res.getLuckyTurntableInfo();
}

const StageExpInfoMap_t & ManageResourceImp::getStageExpInfoMap()
{
    return m_manage_stage_exp_res.getStageExpInfo();
}

const DoubleexpResMap_t & ManageResourceImp::getDoubleexpResMap()
{
	return m_manage_doubleexp_res.getDoubleexpMap();
}

const ShlUnlockInfoVec_t & ManageResourceImp::getShlUnlockInfoVec()
{
    return m_manage_shl_unlock_res.getShlUnlockInfo();
}

const ShortcutBonusInfo & ManageResourceImp::getShortcutBonusInfo()
{
    return m_manage_shortcut_bonus_res.getShortcutBonusInfo();
}

const FutureBonusInfoVec_t & ManageResourceImp::getFutureBonusInfoVec()
{
    return m_manage_future_bonus_res.getFutureBonusInfo();
}

const NextdayInterestInfoVec_t & ManageResourceImp::getNextdayInterestInfoVec()
{
    return m_manage_nextday_interest_res.getNextdayInterestInfo();
}

const AccBuyInfoVec_t & ManageResourceImp::getAccBuyInfoVec()
{
    return m_manage_acc_buy_res.getAccBuyInfo();
}

const FirstBuyInfoVec_t & ManageResourceImp::getFirstBuyInfoVec()
{
    return m_manage_first_buy_res.getFirstBuyInfo();
}

const SectionBonusInfoVec_t & ManageResourceImp::getSectionBonusInfoVec()
{
    return m_manage_section_bonus_res.getSectionBonusInfo();
}

const BattlegroundBuffInfoMap_t & ManageResourceImp::getBattlegroundBuffInfoMap()
{
	return m_manage_battleground_buff_res.getBattlegroundBuffInfo();
}

const BattlegroundBetInfoMap_t & ManageResourceImp::getBattlegroundBetInfoMap()
{
	return m_manage_battleground_bet_res.getBattlegroundBetInfo();
}

const FriendAddInfoRes & ManageResourceImp::getFriendAddInfoRes()
{
	return m_manage_friend_add_res.getFriendAddInfoRes();
}

bool
ManageResourceImp::loadGiftRes() {
    static bool is_first_time = true;
    bool succ = false;
    do {
        const char *filename = "gifts.xml";
        Document *xml = NULL;
        if (!loadResource(filename, &xml)) {
            REPORT_RES_LOAD_RESULT("loading %s failed\n", filename);
            break;
        }
        if (!m_manage_gift_res.Load(xml)) {
            REPORT_RES_LOAD_RESULT("parsing %s failed\n", filename);
            break;
        }

        succ = true;
    } while (0);
    return succ;
}

bool
ManageResourceImp::loadIllusionRes() {
    static bool is_first_time = true;
    bool succ = true;
    do {
        const char *filename = "illusion.xml";
        //REPORT_RES_LOAD_RESULT("Start file[%s] loading\n", filename);
        Document *xml = NULL;
        if (!loadResource(filename, &xml)) {
            REPORT_RES_LOAD_RESULT("loading %s failed\n", filename);
            succ = false;
            break;
        }
        if (!m_manage_illusion_res.Load(xml)) {
            REPORT_RES_LOAD_RESULT("parsing %s failed\n", filename);
            succ = false;
            break;
        }
        //REPORT_RES_LOAD_RESULT("Done file[%s] parsing\n", filename);
    } while(0);
    return succ;
}

bool
ManageResourceImp::loadChakraRes() {
    static bool is_first_time = true;
    bool succ = true;
    do {
        const char *filename = "chakraself.xml";
        Document *xml = NULL;
        if (!loadResource(filename, &xml)) {
            REPORT_RES_LOAD_RESULT("loading %s failed\n", filename);
            succ = false;
            break;
        }
        if (!m_manage_chakra_res.LoadChakrp(xml)) {
            REPORT_RES_LOAD_RESULT("parsing %s failed\n", filename);
            succ = false;
            break;
        }
    } while(0);

    do {
        const char *filename = "chakrabody.xml";
        Document *xml = NULL;
        if (!loadResource(filename, &xml)) {
            REPORT_RES_LOAD_RESULT("loading %s failed\n", filename);
            succ = false;
            break;
        }
        if (!m_manage_chakra_res.LoadChakro(xml)) {
            REPORT_RES_LOAD_RESULT("parsing %s failed\n", filename);
            succ = false;
            break;
        }
    } while(0);

    do {
        const char *filename = "chakraitem.xml";
        Document *xml = NULL;
        if (!loadResource(filename, &xml)) {
            REPORT_RES_LOAD_RESULT("loading %s failed\n", filename);
            succ = false;
            break;
        }
        if (!m_manage_chakra_res.LoadChakre(xml)) {
            REPORT_RES_LOAD_RESULT("parsing %s failed\n", filename);
            succ = false;
            break;
        }
    } while(0);

    if (succ) {
        m_manage_chakra_res.CheckSelfConsistency();
    }

    is_first_time = false;
    return succ;
}

const LuckyRewardsInfoVec_t & ManageResourceImp::getLuckyRewardsInfoVec()
{
    return m_manage_lucky_rewards_res.getLuckyRewardsInfo();
}

bool
ManageResourceImp::loadCondorRes() {
    bool succ = false;
    static bool is_first_time = true;
    do {
        //REPORT_RES_LOAD_RESULT("[MK_CONDOR] Loading resource ... \n");

        const char *prey_file = "preyawards.xml";
        const char *weapon_file = "condor.xml";
        const char *formula_file = "clearning.xml";
        Document *prey, *weapon, *formula;
        prey = weapon = formula = NULL;
        if (!loadResource(prey_file, &prey)) {
            REPORT_RES_LOAD_RESULT("loading %s failed\n", prey_file);
            break;
        }
        if (!loadResource(weapon_file, &weapon)) {
            REPORT_RES_LOAD_RESULT("loading %s failed\n", weapon_file);
            break;
        }
        if (!loadResource(formula_file, &formula)) {
            REPORT_RES_LOAD_RESULT("loading %s failed\n", formula_file);
            break;
        }
        if (!m_manage_condor_res.Load(prey, weapon, formula)) {
            break;
        }

        //REPORT_RES_LOAD_RESULT("[MK_CONDOR] Done resource loading.\n");
        succ = true;
    } while (0);
    return succ;
}

bool
ManageResourceImp::loadAnswerRes() {
    bool succ = false;
    static bool is_first_time = true;
    do {
        const char *question_file = "world.xml";
        const char *event_file = "worldevent.xml";
        const char *config_file = "system.xml";
        const char *coins_file = "goldblessme.xml";
        Document *question, *event, *config, *coins;
        question = event = config = coins = NULL;
        if (!loadResource(question_file, &question)) {
            REPORT_RES_LOAD_RESULT("loading %s failed\n", question_file);
            break;
        }
        if (!loadResource(event_file, &event)) {
            REPORT_RES_LOAD_RESULT("loading %s failed\n", event_file);
            break;
        }
        if (!loadResource(config_file, &config)) {
            REPORT_RES_LOAD_RESULT("loading %s failed\n", config_file);
            break;
        }
        if (!loadResource(coins_file, &coins)) {
            REPORT_RES_LOAD_RESULT("loading %s failed\n", coins_file);
            break;
        }
        if (!m_manage_answer_res.Load(question, event, config, coins)) {
            REPORT_RES_LOAD_RESULT("parsing answer resource files failed!\n");
            break;
        }

        succ = true;
    } while (0);
    return succ;
}

void ManageResourceImp::convertSchemePropertyID()
{
	convertSchemeSkillPropertyID();
	convertSchemeFormPropertyID();
}

void ManageResourceImp::convertSchemeSkillPropertyID()
{
	SkillResInfoMap_t * skill_info_map = (SkillResInfoMap_t *)&m_manage_skill_res.getSkillInfoMap();
	for (SkillResInfoMap_t::iterator it = skill_info_map->begin(); it != skill_info_map->end(); ++it)
	{
		SkillResInfo * skill_info = it->second;
		for (SkillFunResInfoVec_t::iterator fun_it = skill_info->skill_fun_vec.begin(); fun_it != skill_info->skill_fun_vec.end(); ++fun_it)
		{
			SkillFunResInfo & fun_info = *fun_it;

			if (7 == fun_info.fun_id)
			{
				fun_info.a = getAttrIdBySchemeId(fun_info.a);
			}
		}
	}
}

void ManageResourceImp::convertSchemeFormPropertyID()
{
	FormResInfoMap_t * form_info_map = (FormResInfoMap_t *)&m_manage_form_res.getFormInfoMap();
	for (FormResInfoMap_t::iterator it = form_info_map->begin(); it != form_info_map->end(); ++it)
	{
		FormResInfo * form_info = it->second;
		for (vector<std::pair<int, int> >::iterator attr_it = form_info->attr_increase.begin(); attr_it != form_info->attr_increase.end(); ++attr_it)
		{
			attr_it->first = getAttrIdBySchemeId(attr_it->first);
		}
	}
}

int ManageResourceImp::getAttrIdBySchemeId(int scheme_id)
{
	PropertyResInfoMap_t::const_iterator it = m_manage_property_res.getPropertyInfoMap().find(scheme_id);
	if (it != m_manage_property_res.getPropertyInfoMap().end())
	{
		return it->second->program_id;
	}

	return 0;
}

const TowerStageInfoMap_t & ManageResourceImp::getTowerStageInfoMap()
{
	return m_manage_tower_stage_res.getTowerStageInfo();
}

const TowerBuffInfoMap_t & ManageResourceImp::getTowerBuffInfoMap()
{
	return m_manage_tower_buff_res.getTowerBuffInfo();
}

const TowerRewardsInfoMap_t & ManageResourceImp::getTowerRewardsInfoMap()
{
	return m_manage_tower_rewards_res.getTowerRewardsInfo();
}

const ArrestListInfoMap_t & ManageResourceImp::getArrestListInfoMap()
{
    return m_manage_arrest_list_res.getArrestListInfo();
}

const ArrestControllerInfoVec_t & ManageResourceImp::getArrestControllerInfoVec()
{
    return m_manage_arrest_controller_res.getArrestControllerInfo();
}

const FailScoreInfoMap_t & ManageResourceImp::getFailScoreInfoMap()
{
	return m_manage_fail_score_res.getFailScoreInfo();
}

const JianZhongInfoMap_t & ManageResourceImp::getJianZhongInfoMap()
{
	return m_manage_jian_zhong_res.getJianZhongInfo();
}

const RankingRuleInfoMap_t & ManageResourceImp::getRankingRuleInfoMap()
{
    return m_manage_ranking_rule_res.getRankingRuleInfo();
}

const RankingRewardInfoMap_t & ManageResourceImp::getRankingRewardInfoMap()
{
    return m_manage_ranking_reward_res.getRankingRewardInfo();
}

const ShenghuoLingInfoVec_t & ManageResourceImp::getShenghuoLingInfoVec()
{
    return m_manage_shenghuo_ling_res.getShenghuoLingInfo();
}

const PairSystemInfo & ManageResourceImp::getPairSystemInfo()
{
	return m_manage_pair_system_res.getPairSystemInfo();
}

const PairGamblingInfoVec_t & ManageResourceImp::getPairGamblingInfoVec()
{
	return m_manage_pair_gambling_res.getPairGamblingInfo();
}

const PairNameInfoVec_t & ManageResourceImp::getPairNameInfoVec()
{
	return m_manage_pair_name_res.getPairNameInfo();
}

const PairRewardsInfoVec_t & ManageResourceImp::getPairRewardsInfoVec()
{
	return m_manage_pair_rewards_res.getPairRewardsInfo();
}

const WebVipInfo & ManageResourceImp::getWebVipInfo()
{
	return m_manage_web_vip_res.getWebVipInfo();
}