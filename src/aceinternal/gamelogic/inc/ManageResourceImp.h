
#ifndef MANAGE_RESOURCE_IMP_HPP
#define MANAGE_RESOURCE_IMP_HPP

#include "Scene.h"
#include "ManageEnemyRes.h"
#include "ManageFormRes.h"
#include "ManageMapRes.h"
#include "ManageNpcRes.h"
#include "ManageSkillRes.h"
#include "ManageMonsterRes.h"
#include "ManagePlayerInit.h"
#include "ManageWarJobEnrageRes.h"
#include "ManageWarInitRes.h"
#include "ManagePropertyResInfo.h"
#include "ManageRoleRes.h"
#include "ManageBufferRes.h"
#include "ManageHireRes.h"
#include "ManageLevelExpRes.h"
#include "ManageExpAddition.h"
#include "ManageWarScoreRes.h"
#include "ManageLawRes.h"
#include "ManageItemRes.h"
#include "ManageBonusRes.h"
#include "ManageSectRes.h"
#include "ManagePrandomRes.h"
#include "ManageRandomSkillRes.h"
#include "ManageGoldParamRes.h"
#include "ManageTalkRes.h"
#include "ManageProhibitWordRes.h"
#include "ManageTalkRes.h"
#include "ManageItemStrengRes.h"
#include "ManageRecruitmentRes.h"
#include "ManageRecruitexpRes.h"
#include "ManageSystemRes.h"
#include "ManageStage1stRes.h"
#include "ManageQuestRes.h"
#include "ManageSynthesisRes.h"
#include "ManageRaiseRateRes.h"
#include "ManageRaiseRes.h"
#include "ManageShopRes.h"
#include "ManageArenaRes.h"
#include "ManageSpecialShopRes.h"
#include "ManageIllusionRes.h"
#include "ManageChakraRes.h"
#include "ManageDailyBonusRes.h"
#include "ManageCondorRes.h"
#include "ManageEggRes.h"
#include "ManageHammerRes.h"
#include "ManageCardAndJokerRes.h"
#include "ManageVipexpRes.h"
#include "ManageBossGuWuRes.h"
#include "ManageBossForRes.h"
#include "ManageBossRewardRes.h"
#include "ManageBossExpRes.h"
#include "ManageAnswerRes.h"
#include "ManageVipRes.h"
#include "ManageSystemUnlockRes.h"
#include "ManageHangUpRes.h"
#include "ManageGiftRes.h"
#include "ManageAchievementRes.h"
#include "ManageSweepStageResInfo.h"
#include "ManageMultiCopyRes.h"
#include "ManageFlowerResInfo.h"
#include "ManagePracticeRes.h"
#include "ManageTrainingRes.h"
#include "ManageDailyQuestRes.h"
#include "ManageDailyQuestLibraryRes.h"
#include "ManageDailyPointsRes.h"
#include "ManageGuildRes.h"
#include "ManageTowerStageRes.h"
#include "ManageTowerBuffRes.h"
#include "ManageTowerRewardsRes.h"
#include "ManageEscortTerminalRes.h"
#include "ManageEscortCarRes.h"
#include "ManageEscortSystemRes.h"
#include "ManageEscortAgencyRes.h"
#include "ManageFreeTimeBag.h"
#include "ManageTavernRes.h"
#include "ManageTavernhireRes.h"
#include "ManageTavernshopRes.h"
#include "ManageTavernprobabilityRes.h"
#include "ManageTavernshopRes.h"
#include "ManageFishingRes.h"
#include "ManageRingQuestRes.h"
#include "ManageRingRandomRes.h"
#include "ManageRingGuildRes.h"
#include "ManageRingRewardRes.h"
#include "ManageOfflinehangupRes.h"
#include "ManageGvgRes.h"
#include "ManageGvgrewardsRes.h"
#include "ManageDodgeRes.h"
#include "ManagePlumRes.h"
#include "ManageDodgeMiniRes.h"
#include "ManagePlumSystemRes.h"
#include "ManagePlumRewardRes.h"
#include "ManagePlumJarRes.h"
#include "ManageBattlegroundDatetimeRes.h"
#include "ManageBattlegroundRewardRes.h"
#include "ManageBattlegroundBetRes.h"
#include "ManageBattlegroundBuffRes.h"
#include "ManageTavernspebilityRes.h"
#include "ManageSuitRes.h"
#include "ManageMateupRes.h"
#include "ManageMateinheritRes.h"
#include "ManageMatecolorRes.h"
#include "ManageMategateRes.h"
#include "ManageMateRes.h"
#include "ManageMailRes.h"
#include "ManageMateskillRes.h"
#include "ManagePvpRes.h"
#include "ManagePvp2Res.h"
#include "ManageGamblingRes.h"
#include "ManagePvprewardsRes.h"
#include "ManageMail_formatRes.h"
#include "ManageTavern_groupRes.h"
#include "ManageGemgroupRes.h"
#include "ManageArrestListRes.h"
#include "ManageArrestControllerRes.h"
#include "ManagePelletInfoRes.h"
#include "ManageMazeRes.h"
#include "ManageMazeexplorationRes.h"
#include "ManageMazerewardsRes.h"
#include "ManageMazeroomRes.h"
#include "ManageTalklinkRes.h"
#include "ManageDodgeRevokeRes.h"
#include "ManageInheritInfoRes.h"
#include "ManageGemsynthesisRes.h"
#include "ManageAccOnlineRes.h"
#include "ManageSectionBonusRes.h"
#include "ManageFirstBuyRes.h"
#include "ManageAccBuyRes.h"
#include "ManageNextdayInterestRes.h"
#include "ManageFutureBonusRes.h"
#include "ManageShortcutBonusRes.h"
#include "ManageFriendAddInfoRes.h"
#include "FriendAddInfoRes.h"
#include "ManageAmahRes.h"
#include "ManageOpenactRes.h"
#include "ManageFailScoreRes.h"
#include "ManageEquip_gettingRes.h"
#include "ManageTitleRes.h"
#include "ManageJianZhongRes.h"
#include "ManageRankingRuleRes.h"
#include "ManageRankingRewardRes.h"
#include "ManageRaceRes.h"
#include "ManageSplendidRes.h"
#include "ManagePushRes.h"
#include "ManageChargequestRes.h"
#include "ManageOpenact1Res.h"
#include "ManageOpenact2Res.h"
#include "ManageRunesClassRes.h"
#include "ManageRunesExpRes.h"
#include "ManageRunesLevelRes.h"
#include "ManageRunesSystemRes.h"
#include "ManageFiresRes.h"
#include "ManageFiresattackRes.h"
#include "ManageShenghuoLingRes.h"
#include "ManageShlUnlockRes.h"
#include "ManageStageExpRes.h"
#include "ManageDoubleexpRes.h"
#include "ManageRankingTalkRes.h"
#include "ManageBillListRes.h"
#include "ManageSoulSoulRes.h"
#include "ManageSoulSetRes.h"
#include "ManageSoulChangeRes.h"
#include "ManageTreasureShopRes.h"
#include "ManageSoulStageRes.h"
#include "ManageSoulCfgRes.h"
#include "ManageDanyaoHechengRes.h"
#include "ManageLogingiftRes.h"
#include "ManageSoulSuperRes.h"
#include "ManageGetmoneyRes.h"
#include "ManagePairSystemRes.h"
#include "ManagePairGamblingRes.h"
#include "ManagePairNameRes.h"
#include "ManagePairRewardsRes.h"
#include "ManageTargetRes.h"
#include "ManageTargetquestRes.h"
#include "ManageGoldCardRes.h"
#include "ManageLuckyRewardsRes.h"
#include "ManageLuckyTurntableRes.h"
#include "ManageTalentRes.h"
#include "ManageTalentgroupRes.h"
#include "ManageTalentsystemRes.h"
#include "ManageWebVipRes.h"
#include "ManageHorseRoomRes.h"
#include "ManageHorseAttrRes.h"
#include "ManageRanchInfoRes.h"
#include "ManageRanchCfgRes.h"
#include "ManageHorsemanLevelupRes.h"
#include "ManageHelperInfoRes.h"
#include "ManageQqCfgRes.h"
#include "ManageQqinviteRes.h"
#include "ManageQqshareRes.h"
#include "ManageQqbragRes.h"


class GAMELOGIC_EXOPRT ManageResourceImp : public ManageResource
{
public:
	ManageResourceImp();
	~ManageResourceImp();

	int init(SceneCfg * scene_cfg);

public:
	virtual bool loadResource(const string & resource_name, Document * * xml_doc);

	virtual const MapResInfoMap_t & getMapResInfo();

	virtual const EnemyResInfoMap_t & getEnemyResInfo();

	virtual const SkillResInfoMap_t & getSkillResInfo();

	virtual const NpcResInfoMap_t & getNpcResInfo();

	virtual const FormResInfoMap_t & getFormResInfo();

	virtual const MonsterResInfoMap_t & getMonsterResInfo();

	virtual const InitializationCfg & getInitializationCfg();

	virtual const WarJobEnrageResInfoMap_t & getWarJobEnrageResInfo();

	virtual const WarInitResInfo & getWarInitResInfo();

	virtual const PropertyResInfoMap_t & getPropertyResInfo();

	virtual const RoleResInfoMap_t & getRoleResInfo();

	virtual const BufferResInfoMap_t & getBufferResInfo();

	virtual const HireResInfoMap_t & getHireResInfo();

	virtual const LevelExpResInfoMap_t & getLevelExpResInfo();

	virtual const ExpAdditionResInfoMap_t & getExpAdditionResInfo();

	virtual const WarScoreResInfoVec_t & getWarScoreResInfo();

	virtual const LawResInfoVec_t & getLawInfoVec();

	virtual const ItemResInfoMap_t & getItemInfoMap();

	virtual const BonusResInfoMap_t & getBonusInfoMap();

	virtual const SectResInfoMap_t & getSectInfoMap();

	virtual const PrandomResInfoMap_t & getPrandomInfoMap();

	virtual const RandomSkillResInfoMap_t & getRandomSkillInfoMap();

	virtual const GoldParamResInfoMap_t & getGoldParamResInfoMap();

	virtual const ProhibitWordResVec_t & getProhibitWordResVec();

	virtual const ProhibitWordResVec_t & getProhibitNameInfoMap();

	virtual const TalkResInfoMap_t & getTalkResInfoMap();

	virtual const ItemStrengResInfoMap_t & getItemStrengResInfoMap();

	virtual const RecruitmentResInfoMap_t & getRecruitmentResInfoMap();

	virtual const RecruitexpResInfoMap_t & getRecruitexpResInfoMap();

	virtual const SystemResInfo & getSystemResInfo();

	virtual const Stage1stResInfoMap_t & getStage1stResInfoMap();

	virtual const QuestResInfoMap_t & getQuestResInfoMap();

	virtual const SynthesisResInfoMap_t & getSynthesisResInfo();

	virtual const RaiseResInfoVec_t & getRaiseResInfo();

	virtual const RaiseRateResInfoVec_t & getRaiseRateResInfo();

	virtual const ShopInfoMap & getShopInfoMap();

	virtual const ArenaResInfoVec_t & getArenaResInfo();

	virtual const SpecialShopResInfoMap_t & getSpecialShopInfoMap();

    virtual const ColorInfoMap& getColorInfoMap();

    ChakrpInfoMap const& getChakrpInfoMap();
    ChakreInfoMap const& getChakreInfoMap();
    ChakroInfoMap const& getChakroInfoMap();
    ChakreSelectMap const& getChakreSelectMap();
    ChakroSortedVec const& getChakroSortedInfos();
    ChakrpSortedVec const& getChakrpSortedInfos();

	virtual const DailyBonusResInfoMap_t & getDailyBonusResInfoMap();

    CondorPreyMap const& getCondorPreyMap();
    CondorWeaponMap const& getCondorWeaponMap();
    CondorFormula const& getCondorFormula();

	virtual const EggResInfoVec_t & getEggResInfo();
	virtual const HammerResInfoMap_t & getHammerResInfoMap();
	virtual const CardAndJokerResInfoMap_t & getCardAndJokerResInfoMap();

	virtual const VipexpResInfoMap_t & getVipexpResInfoMap();

	virtual const BossGuWuResInfoMap_t & getBossGuWuResInfoMap();
	virtual const BossRewardResInfoMap_t & getBossRewardResInfoMap();
	virtual const BossForResInfoMap_t & getBossForResInfoMap();
	virtual const BossExpResInfoVec_t & getBossExpResInfo();

    virtual const AnswerResource& getAnswerResource();

	virtual const VipInfoMap_t & getVipInfoMap();

	virtual const SystemUnlockRes & getSystemUnlockInfoMap();
	
	virtual const HangUpRes & getHangUpRes();

    virtual const GiftResource & getGiftResource();

	virtual const AchievementInfoMap_t & getAchevementInfoMap();
	virtual const AchievementRewardMap_t & getAchievementRewardMap();
	virtual const AchievementUnlockMap_t & getAchievementUnlockMap();

	virtual const DailyPointsEventsMap_t & getDailyPointsEventsMap();
	virtual const DailyPointsRewardMap_t & getDailyPointsRewardMap();
	virtual const TavernInfoMap_t& getTavernInfo();
	virtual const TavernhireInfoMap_t& getTavernhireInfo();
	virtual const TavernprobabilityInfoMap_t& getTavernprobabilityInfo();
	virtual const TavernshopInfoVector_t& getTavernshopInfo();
	virtual const GvgInfo& getGvgInfo();
	virtual const GvgrewardsInfoMap_t& getGvgrewardsInfoMap();

	virtual const Guild_NityMap_t & getGuildNityMap();
	virtual const Guild_ActivityVector_t & getGuildActivity();

	virtual const SweepStageResInfo & getSweepStageResInfo();

	virtual const MultiCopyResMap_t & getMultiCopyResMap();

	virtual const FlowerResMap_t & getFlowerResMap();

	virtual const WineTankResInfoMap_t & getWineTankResInfoMap();

	virtual const GourdResInfo & getGourdResInfo();

	virtual const StomachResInfo & getStomachResInfo();

	virtual const WineIncreaseResInfo & getWineIncreaseResInfo();

	virtual const WineTankUpgradeResInfo & getWineTankUpgradeResInfo();

	virtual const TrainingPartnersInfoMap_t& getTrainingPartnersInfoMap();
	virtual const TrainingQualityMap_t& getTrainingQualityMap();

	virtual const DailyQuestInfoMap_t& getDailyQuestInfoMap();
	virtual const DailyQuestLibraryInfoVec_t& getDailyQuestLibraryInfo();

	virtual const TowerStageInfoMap_t& getTowerStageInfoMap();
	virtual const TowerBuffInfoMap_t& getTowerBuffInfoMap();
	virtual const TowerRewardsInfoMap_t& getTowerRewardsInfoMap();

	virtual const EscortTerminalInfoMap_t& getEscortTerminalInfoMap();
	virtual const EscortCarInfoMap_t& getEscortCarInfoMap();
	virtual const EscortSystemInfo& getEscortSystemInfo();
	virtual const EscortAgencyInfoMap_t& getEscortAgencyInfoMap();

	virtual const FreeTimeBagVec_t & getFreeTimeBagVec();

	virtual const FishingInfoMap_t& getFishingInfoMap();

	virtual const RingQuestInfoVec_t& getRingQuestInfoVec();
	virtual const RingRandomInfoVec_t& getRingRandomInfoVec();
	virtual const RingGuildInfoMap_t& getRingGuildInfoMap();
	virtual const RingRewardInfoMap_t& getRingRewardInfoMap();

	virtual const OfflinehangupInfo & getOfflinehangupInfo();

	virtual const DodgeInfoMap_t& getDodgeInfoMap();
	virtual const PlumInfoMap_t& getPlumInfoMap();
	virtual const DodgeMiniInfoMap_t& getDodgeMiniInfoMap();
	virtual const DodgeRevokeInfo& getDodgeRevokeInfo();
	virtual const PlumSystemInfo& getPlumSystemInfo();
	virtual const PlumRewardInfoVec_t& getPlumRewardInfoVec();
	virtual const PlumJarInfoMap_t& getPlumJarInfoMap();

	virtual const BattlegroundDatetimeInfoMap_t& getBattlegroundDatetimeInfoMap();
	virtual const BattlegroundRewardInfoMap_t& getBattlegroundRewardInfoMap();
	virtual const BattlegroundBetInfoMap_t& getBattlegroundBetInfoMap();
	virtual const BattlegroundBuffInfoMap_t& getBattlegroundBuffInfoMap();
	virtual const TavernspebilityInfoMap_t& getTavernspebilityInfoMap();
	virtual const SuitInfoMap_t& getSuitInfoMap();
	virtual const MateupInfoMap_t& getMateupInfoMap();
	virtual const MateinheritInfoMap_t& getMateinheritInfoMap();
	virtual const MatecolorInfoMap_t& getMatecolorInfoMap();
	virtual const MategateInfoMap_t& getMategateInfoMap();
	virtual const MateInfoMap_t& getMateInfoMap();
	virtual const MateskillInfoMap_t& getMateskillInfoMap();
	virtual const MailInfoMap_t& getMailInfoMap();
	virtual const PvpInfo& getPvpInfo();
	virtual const Pvp2Info& getPvp2Info();
	virtual const GamblingInfo& getGamblingInfo();
	virtual const PvprewardsInfoVec_t& getPvprewardsInfoVec();
	virtual const Mail_formatInfoMap_t& getMail_formatInfoMap();
	virtual const Tavern_groupInfoMap_t& getTavern_groupInfoMap();
	virtual const GemgroupInfoVec_t& getGemgroupInfoVec();
    virtual const ArrestListInfoMap_t& getArrestListInfoMap();
    virtual const ArrestControllerInfoVec_t& getArrestControllerInfoVec();
	virtual const PelletInfoResMap_t & getPelletInfoResMap();
	virtual const MazeInfoMap_t& getMazeInfoMap();
	virtual const MazeexplorationInfo& getMazeexplorationInfo();
	virtual const MazerewardsInfoVec_t& getMazerewardsInfoMap();
	virtual const MazeroomInfoVec_t& getMazeroomInfoMap();
	virtual const TalklinkInfoMap_t& getTalklinkInfoMap();

	virtual const InheritInfoRes & getInheritInfoRes();
	virtual const GemsynthesisInfoMap_t& getGemsynthesisInfoMap();


    virtual const AccOnlineInfo& getAccOnlineInfo();
    virtual const SectionBonusInfoVec_t& getSectionBonusInfoVec();
    virtual const FirstBuyInfoVec_t& getFirstBuyInfoVec();
    virtual const AccBuyInfoVec_t& getAccBuyInfoVec();
    virtual const NextdayInterestInfoVec_t& getNextdayInterestInfoVec();
    virtual const FutureBonusInfoVec_t& getFutureBonusInfoVec();
    virtual const ShortcutBonusInfo& getShortcutBonusInfo();

	virtual const FriendAddInfoRes & getFriendAddInfoRes();
	virtual const AmahInfoMap_t& getAmahInfoMap();
	virtual const OpenactInfoMap_t& getOpenactInfoMap();

	virtual const FailScoreInfoMap_t& getFailScoreInfoMap();
	virtual const Equip_gettingInfoMap_t& getEquip_gettingInfoMap();

	virtual const TitleInfoMap_t& getTitleInfoMap();

	virtual const JianZhongInfoMap_t& getJianZhongInfoMap();
	virtual const SplendidVoteTimeMap_t & getSplendidVoteTimeMap();

	virtual const SplendidRankRewardMap_t & getSplendidRankRewardMap();

	virtual const SplendidWorshipRewardMap_t & getSplendidWorshipRewardMap();
    virtual const RankingRuleInfoMap_t& getRankingRuleInfoMap();
    virtual const RankingRewardInfoMap_t& getRankingRewardInfoMap();
	virtual const RaceInfoVec_t& getRaceInfoVec();
	virtual const PushInfoMap_t& getPushInfoMap();
	virtual const ChargequestInfo& getChargequestInfo();
	virtual const Openact1InfoVec_t& getOpenact1InfoVec();
	virtual const Openact2InfoVec_t& getOpenact2InfoVec();
	virtual const RunesClassInfoMap_t& getRunesClassInfoMap();
	virtual const RunesExpInfoMap_t& getRunesExpInfoMap();
	virtual const RunesLevelInfoMap_t& getRunesLevelInfoMap();
	virtual const RunesSystemInfo& getRunesSystemInfo();
	virtual const FiresattackInfoMap_t& getFiresattackInfoMap();
	virtual const FiresInfo& getFiresInfo();
    virtual const ShenghuoLingInfoVec_t& getShenghuoLingInfoVec();
    virtual const ShlUnlockInfoVec_t& getShlUnlockInfoVec();
    virtual const StageExpInfoMap_t& getStageExpInfoMap();

	virtual const DoubleexpResMap_t & getDoubleexpResMap();
    virtual const RankingTalkInfoMap_t& getRankingTalkInfoMap();
    virtual const BillListInfoMap_t& getBillListInfoMap();
	virtual const SoulSoulInfoMap_t& getSoulSoulInfoMap();
	virtual const SoulSetInfoMap_t& getSoulSetInfoMap();
	virtual const SoulChangeInfo& getSoulChangeInfo();
	virtual const TreasureShopInfoMap_t& getTreasureShopInfoMap();
    virtual const SoulStageInfoMap_t& getSoulStageInfoMap();
    virtual const SoulCfgInfo& getSoulCfgInfo();
	virtual const DanyaoHechengInfoMap_t& getDanyaoHechengInfoMap();
	virtual const PairSystemInfo& getPairSystemInfo();
	virtual const PairGamblingInfoVec_t& getPairGamblingInfoVec();
	virtual const PairNameInfoVec_t& getPairNameInfoVec();
	virtual const PairRewardsInfoVec_t& getPairRewardsInfoVec();
	virtual const LogingiftInfo& getLogingiftInfo();
	virtual const SoulSuperInfoMap_t& getSoulSuperInfoMap();
	virtual const GetmoneyInfoVec_t& getGetmoneyInfoVec();
	virtual const TargetInfoMap_t& getTargetInfoMap();
	virtual const TargetquestInfoVec_t& getTargetquestInfoVec();
	virtual const GoldCardInfoMap_t& getGoldCardInfoMap();
    virtual const LuckyRewardsInfoVec_t& getLuckyRewardsInfoVec();
    virtual const LuckyTurntableInfo& getLuckyTurntableInfo();
	virtual const TalentInfoMap_t& getTalentInfoMap();
	virtual const TalentgroupInfoMap_t& getTalentgroupInfoMap();
	virtual const TalentsystemInfo& getTalentsystemInfo();

	virtual const WebVipInfo& getWebVipInfo();
    virtual const HorseRoomInfoMap_t& getHorseRoomInfoMap();
    virtual const HorseAttrInfoVec_t& getHorseAttrInfoVec();
    virtual const RanchInfoInfoVec_t& getRanchInfoInfoVec();
    virtual const RanchCfgInfoVec_t& getRanchCfgInfoVec();
    virtual const HorsemanLevelupInfoVec_t& getHorsemanLevelupInfoVec();
    virtual const HelperInfoInfoVec_t& getHelperInfoInfoVec();
    virtual const QqCfgInfoMap_t& getQqCfgInfoMap();
	virtual const QqinviteInfoMap_t& getQqinviteInfoMap();
	virtual const QqshareInfo& getQqshareInfo();
	virtual const QqbragInfoMap_t& getQqbragInfoMap();

protected:
	int loadAllRes();

	void convertSchemePropertyID();

	void convertSchemeSkillPropertyID();

	void convertSchemeFormPropertyID();

	int getAttrIdBySchemeId(int scheme_id);

    bool loadIllusionRes();
    bool loadChakraRes();
    bool loadCondorRes();
    bool loadAnswerRes();
    bool loadGiftRes();

private:
	SceneCfg * m_scene_cfg;

	typedef map<string, Document *>		FileToXmlMap_t;

	FileToXmlMap_t	m_file_to_xml_map;

	ManageEnemyRes	m_manage_enemy_res;
	ManageFormRes	m_manage_form_res;
	ManageMapRes	m_manage_map_res;
	ManageNpcRes	m_manage_npc_res;
	ManageSkillRes	m_manage_skill_res;
	ManageMonsterRes	m_manage_monster_res;
	ManagePlayerInit m_manage_initialization_cfg;
	ManageWarJobEnrageRes m_manage_warjob_res;
	ManageWarInitRes	m_manage_war_init_res;
	ManagePropertyResInfo m_manage_property_res;
	ManageRoleRes		m_manage_role_res;
	ManageBufferRes		m_manage_buffer_res;
	ManageHireRes		m_manage_hire_res;
	ManageLevelExpRes	m_manage_level_exp_res;
	ManageExpAddition	m_manage_exp_addition_res;
	ManageWarScoreRes	m_manage_war_score_res;
	ManageLawRes		m_manage_law_res;
	ManageItemRes		m_manage_item_res;
	ManageBonusRes		m_manage_bonus_res;
	ManageSectRes		m_manage_sect_res;
	ManagePrandomRes	m_manage_prandom_res;
	ManageRandomSkillRes	m_manage_random_skill_res;
	ManageGoldParamRes		m_manage_gold_param_res;
	ManageProhibitWordRes m_manage_prohibit_word;
	ManageTalkRes m_manage_talk_res;

	ManageItemStrengRes	m_manage_item_streng_res;
	ManageRecruitmentRes	m_manage_recruitment_res;
	ManageRecruitexpRes		m_manage_recruitexp_res;
	ManageSystemRes			m_manage_system_res;
	ManageStage1stRes		m_manage_stage1st_res;
	ManageQuestRes			m_manage_quest_res;
	ManageSynthesisRes		m_manage_synthesis_res;

	ManageRaiseRes			m_manage_raise_res;
	ManageRaiseRateRes		m_manage_raise_rate_res;

	ManageShopRes			m_manage_shop_res;
	ManageArenaRes			m_manage_arena_res;
	ManageSpecialShopRes	m_manage_special_shop_res;

	ManageIllusionRes		m_manage_illusion_res;
    ManageChakraRes         m_manage_chakra_res;

	ManageDailyBonusRes		m_manage_daily_bonus_res;
    ManageCondorRes         m_manage_condor_res;

	ManageEggRes			m_manage_egg_res;
	ManageHammerRes			m_manage_hammer_res;
	ManageCardAndJokerRes	m_manage_card_and_joker_res;

	ManageVipexpRes			m_manage_vipexp_res;

	ManageBossGuWuRes		m_manage_boss_guwu_res;
	ManageBossRewardRes		m_manage_boss_reward_res;
	ManageBossForRes		m_manage_boss_for_res;
	ManageBossExpRes		m_manage_boss_exp_res;

    ManageAnswerRes         m_manage_answer_res;

	ManageVipRes			m_manage_vip_res;

	ManageSystemUnlockRes	m_manage_system_unlock_res;

	ManageHangUpRes			m_manage_hang_up_res;

    ManageGiftRes           m_manage_gift_res;

	ManageAchievementRes	m_manage_achievement_res;

	ManageDailyPointsRes	m_manage_dailypoints_res;

	ManageGuildRes			m_manage_guild_res;

	ManageSweepStageResInfo m_manage_sweep_stage_res;

	ManageMultiCopyRes		m_manage_multi_copy_res;

	ManageFlowerResInfo		m_manage_flower_res_info;

	ManageTrainingRes		m_manage_training_res;

	ManagePracticeRes		m_manage_practice_re_info;

	ManageDailyQuestRes		m_manage_daily_quest_res;

	ManageDailyQuestLibraryRes m_manage_daily_quest_library_res;


	ManageTowerStageRes	m_manage_tower_stage_res;
	ManageTowerBuffRes	m_manage_tower_buff_res;
	ManageTowerRewardsRes	m_manage_tower_rewards_res;

	ManageEscortTerminalRes	m_manage_escort_terminal_res;
	ManageEscortCarRes	m_manage_escort_car_res;
	ManageEscortSystemRes	m_manage_escort_system_res;
	ManageEscortAgencyRes	m_manage_escort_agency_res;

	ManageFreeTimeBag		m_manage_free_time_bag;
	ManageTavernRes			m_manage_tavern_res;
	ManageTavernhireRes		m_manage_tavernhire_res;
	ManageTavernprobabilityRes	m_manage_tavernprobability_res;
	ManageTavernshopRes			m_manage_tavernshop_res;

	ManageFishingRes		m_manage_fishing__res;

	ManageRingQuestRes	m_manage_ring_quest_res;
	ManageRingRandomRes	m_manage_ring_random_res;
	ManageRingGuildRes	m_manage_ring_guild_res;
	ManageRingRewardRes	m_manage_ring_reward_res;

	ManageOfflinehangupRes		m_manage_offline_hangup_res;
	ManageGvgRes				m_manage_gvg__res;
	ManageGvgrewardsRes			m_manage_gvgrewards__res;

	ManageDodgeRes	m_manage_dodge__res;
	ManagePlumRes	m_manage_plum__res;
	ManageDodgeMiniRes	m_manage_dodge_mini_res;
	ManageDodgeRevokeRes	m_manage_dodge_revoke_res;
	ManagePlumSystemRes	m_manage_plum_system_res;
	ManagePlumRewardRes	m_manage_plum_reward_res;
	ManagePlumJarRes	m_manage_plum_jar_res;
	ManageBattlegroundDatetimeRes	m_manage_battleground_datetime_res;
	ManageBattlegroundRewardRes	m_manage_battleground_reward_res;
	ManageBattlegroundBetRes	m_manage_battleground_bet_res;
	ManageBattlegroundBuffRes	m_manage_battleground_buff_res;
	ManageTavernspebilityRes	m_manage_tavernspebility__res;
	ManageSuitRes	m_manage_suit__res;
	ManageMateupRes	m_manage_mateup__res;
	ManageMateinheritRes	m_manage_mateinherit__res;
	ManageMatecolorRes	m_manage_matecolor__res;
	ManageMategateRes	m_manage_mategate__res;
	ManageMateRes	m_manage_mate__res;
	ManageMateskillRes	m_manage_mateskill__res;
	ManageMailRes	m_manage_mail__res;
	ManagePvpRes	m_manage_pvp__res;
	ManagePvp2Res	m_manage_pvp_2_res;
	ManageGamblingRes	m_manage_gambling__res;
	ManagePvprewardsRes	m_manage_pvprewards__res;
	ManageMail_formatRes	m_manage_mail_format__res;
	ManageTavern_groupRes	m_manage_tavern_group__res;
	ManageGemgroupRes	m_manage_gemgroup__res;
	ManagePelletInfoRes		m_manage_pellet_info_res;    
	ManageArrestListRes	m_manage_arrest_list_res;
    ManageArrestControllerRes	m_manage_arrest_controller_res;
	ManageMazeRes	m_manage_maze__res;
	ManageMazeexplorationRes	m_manage_mazeexploration__res;
	ManageMazerewardsRes	m_manage_mazerewards__res;
	ManageMazeroomRes	m_manage_mazeroom__res;
	ManageTalklinkRes	m_manage_talklink__res;

	ManageInheritInfoRes		m_manage_inherit_res;
	ManageGemsynthesisRes	m_manage_gemsynthesis__res;
    ManageAccOnlineRes	m_manage_acc_online_res;
    ManageSectionBonusRes	m_manage_section_bonus_res;
    ManageFirstBuyRes	m_manage_first_buy_res;
    ManageAccBuyRes	m_manage_acc_buy_res;
    ManageNextdayInterestRes	m_manage_nextday_interest_res;
    ManageFutureBonusRes	m_manage_future_bonus_res;
    ManageShortcutBonusRes	m_manage_shortcut_bonus_res;
	ManageFriendAddInfoRes m_manage_friend_add_res;
	ManageAmahRes	m_manage_amah__res;
	ManageOpenactRes	m_manage_openact__res;
	ManageFailScoreRes	m_manage_fail_score_res;
	ManageEquip_gettingRes	m_manage_equip_getting__res;
	ManageTitleRes	m_manage_title__res;
	ManageJianZhongRes	m_manage_jian_zhong_res;
    ManageRankingRuleRes	m_manage_ranking_rule_res;
    ManageRankingRewardRes	m_manage_ranking_reward_res;
	ManageRaceRes	m_manage_race__res;
	ManageSplendidRes		m_manage_splendid_res;
	ManagePushRes	m_manage_push__res;
	ManageChargequestRes	m_manage_chargequest__res;
	ManageOpenact1Res	m_manage_openact1__res;
	ManageOpenact2Res	m_manage_openact2__res;

	ManageRunesClassRes	m_manage_runes_class_res;
	ManageRunesExpRes	m_manage_runes_exp_res;
	ManageRunesLevelRes	m_manage_runes_level_res;
	ManageRunesSystemRes	m_manage_runes_system_res;

	ManageFiresRes	m_manage_fires__res;
	ManageFiresattackRes	m_manage_firesattack__res;
    ManageShenghuoLingRes	m_manage_shenghuo_ling_res;
    ManageShlUnlockRes	m_manage_shl_unlock_res;
    ManageStageExpRes	m_manage_stage_exp_res;

	ManageDoubleexpRes	m_manage_doubleexp_res;
    ManageRankingTalkRes	m_manage_ranking_talk_res;
    ManageBillListRes	m_manage_bill_list_res;
	ManageSoulSoulRes	m_manage_soul_soul_res;
	ManageSoulSetRes	m_manage_soul_set_res;
	ManageSoulChangeRes	m_manage_soul_change_res;
	ManageTreasureShopRes	m_manage_treasure_shop_res;
    ManageSoulStageRes	m_manage_soul_stage_res;
    ManageSoulCfgRes	m_manage_soul_cfg_res;
	ManageDanyaoHechengRes	m_manage_danyao_hecheng_res;
	ManagePairSystemRes	m_manage_pair_system_res;
	ManagePairGamblingRes	m_manage_pair_gambling_res;
	ManagePairNameRes	m_manage_pair_name_res;
	ManagePairRewardsRes	m_manage_pair_rewards_res;
	ManageLogingiftRes	m_manage_logingift__res;
	ManageSoulSuperRes	m_manage_soul_super_res;
	ManageGetmoneyRes	m_manage_getmoney__res;
	ManageTargetRes	m_manage_target__res;
	ManageTargetquestRes	m_manage_targetquest__res;
	ManageGoldCardRes	m_manage_gold_card_res;
    ManageLuckyRewardsRes	m_manage_lucky_rewards_res;
    ManageLuckyTurntableRes	m_manage_lucky_turntable_res;
	ManageTalentRes	m_manage_talent__res;
	ManageTalentgroupRes	m_manage_talentgroup__res;
	ManageTalentsystemRes	m_manage_talentsystem__res;
	ManageWebVipRes	m_manage_web_vip_res;
    ManageHorseRoomRes	m_manage_horse_room_res;
    ManageHorseAttrRes	m_manage_horse_attr_res;
    ManageRanchInfoRes	m_manage_ranch_info_res;
    ManageRanchCfgRes	m_manage_ranch_cfg_res;
    ManageHorsemanLevelupRes	m_manage_horseman_levelup_res;
    ManageHelperInfoRes	m_manage_helper_info_res;
    ManageQqCfgRes	m_manage_qq_cfg_res;
	ManageQqinviteRes	m_manage_qqinvite__res;
	ManageQqshareRes	m_manage_qqshare__res;
	ManageQqbragRes	m_manage_qqbrag__res;
};
#endif
