/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#include "StdAfx.h"

#define ZONE_WINTERGRASP         4197
#define POS_X_CENTER             4700
#define MAX_VEHICLE_PER_WORKSHOP    4

const uint32 WintergraspFaction[3] = {1732, 1735, 35};
const uint32 WG_MARK_OF_HONOR = 43589;
const uint32 VehNumWorldState[2] = {3680,3490};
const uint32 MaxVehNumWorldState[2] = {3681,3491};
const uint32 ClockWorldState[5] = {3785,3784,3782,3976,3975};
#define REMOVE_TENACITY_AURA(p) CastTenacity(p, 0)
#define REMOVE_WARTIME_AURAS(p) (p)->RemoveAura(SPELL_RECRUIT);\
    (p)->RemoveAura(SPELL_CORPORAL);(p)->RemoveAura(SPELL_LIEUTENANT)

enum WintergraspSpell
{
    SPELL_RECRUIT           = 37795,
    SPELL_CORPORAL          = 33280,
    SPELL_LIEUTENANT        = 55629,

    SPELL_TENACITY          = 58549,
    SPELL_TENACITY_VEHICLE  = 59911,

    SPELL_TELEPORT_DALARAN  = 53360,
    SPELL_SHUTDOWN_VEHICLE  = 21247,

    SPELL_TOWER_CONTROL     = 62064,
    SPELL_VICTORY_REWARD    = 56902,
    SPELL_DEFEAT_REWARD     = 58494,
    SPELL_DAMAGED_TOWER     = 59135,
    SPELL_DESTROYED_TOWER   = 59136,
    SPELL_DAMAGED_BUILDING  = 59201,
    SPELL_INTACT_BUILDING   = 59203,

    SPELL_RULERS_OF_WG      = 52108,
    SPELL_ESSENCE_OF_WG     = 57940,
    SPELL_ESSENCE_OF_IN_WG  = 58045,
};

enum WintergraspCreature
{
    HORDE_GUARD				= 32307,
    ALLIANCE_GUARD			= 32307,
    HORDE_CHAMPION			= 30739,
    ALLIANCE_CHAMPION		= 30740,
    HORDE_QUARTERMASTER		= 32296,
    ALLIANCE_QUARTERMASTER	= 32294,
    HORDE_DAILY_1			= 31101,
    ALLIANCE_DAILY_1		= 31051,
    HORDE_DAILY_2			= 31102,
    ALLIANCE_DAILY_2		= 31052,
    HORDE_DAILY_3			= 31107,
    ALLIANCE_DAILY_3		= 31109,
    HORDE_DAILY_4			= 31151,
    ALLIANCE_DAILY_4		= 31153,
    HORDE_DAILY_5			= 31106,
    ALLIANCE_DAILY_5		= 31108,
    HORDE_DAILY_6			= 31053,
    ALLIANCE_DAILY_6		= 31054,
    HORDE_COMMANDER			= 31091,
    ALLIANCE_COMMANDER		= 31036,
    HORDE_WARBRINGER		= 32615,
    ALLIANCE_GENERAL		= 32626,
};

enum CreatureEntry
{
    CRE_ENG_A = 30499,
    CRE_ENG_H = 30400,
};

enum WintergraspRewardEvent
{
    WIN_BATTLE,
    LOSE_BATTLE,
    DAMAGED_TOWER,
    DESTROYED_TOWER,
    DAMAGED_BUILDING,
    INTACT_BUILDING,
    WG_REWARD_EVENT_MAX
};

enum OutdoorPvP_WG_Sounds
{
    OutdoorPvP_WG_SOUND_KEEP_CLAIMED            = 8192,
    OutdoorPvP_WG_SOUND_KEEP_CAPTURED_ALLIANCE  = 8173,
    OutdoorPvP_WG_SOUND_KEEP_CAPTURED_HORDE     = 8213,
    OutdoorPvP_WG_SOUND_KEEP_ASSAULTED_ALLIANCE = 8212,
    OutdoorPvP_WG_SOUND_KEEP_ASSAULTED_HORDE    = 8174,
    OutdoorPvP_WG_SOUND_NEAR_VICTORY            = 8456
};

enum DataId
{
    DATA_ENGINEER_DIE,
};

enum WG_KeepStatus
{
    WG_KEEP_TYPE_NEUTRAL             = 0,
    WG_KEEP_TYPE_CONTESTED           = 1,
    WG_KEEP_STATUS_ALLY_CONTESTED    = 1,
    WG_KEEP_STATUS_HORDE_CONTESTED   = 2,
    WG_KEEP_TYPE_OCCUPIED            = 3,
    WG_KEEP_STATUS_ALLY_OCCUPIED     = 3,
    WG_KEEP_STATUS_HORDE_OCCUPIED    = 4
};

enum WintergraspCreType
{
    CREATURE_OTHER,
    CREATURE_SIEGE_VEHICLE,
    CREATURE_TURRET,
    CREATURE_ENGINEER,
    CREATURE_GUARD,
};

enum BuildingType
{
    BUILDING_WALL,
    BUILDING_WORKSHOP,
    BUILDING_TOWER,
};

enum DamageState
{
    DAMAGE_INTACT,
    DAMAGE_DAMAGED,
    DAMAGE_DESTROYED,
};

const uint32 AreaPOIIconId[3][3] = {{7,8,9},{4,5,6},{1,2,3}};

struct BuildingState
{
    explicit BuildingState(uint32 _worldState, TeamId _team, bool asDefault)
        : worldState(_worldState), health(0)
        , defaultTeam(asDefault ? _team : OTHER_TEAM(_team)), team(_team), damageState(DAMAGE_INTACT)
        , building(NULL), type(BUILDING_WALL), graveTeam(NULL)
    {}
    uint32 worldState;
    uint32 health;
    TeamId defaultTeam;
    DamageState damageState;
    GameObject *building;
    uint32 *graveTeam;
    BuildingType type;

    void SendUpdate(Player *player) const
    {
        player->SendUpdateWorldState(worldState, AreaPOIIconId[team][damageState]);
    }

    void FillData(WorldPacket &data) const
    {
        data << worldState << AreaPOIIconId[team][damageState];
    }

    TeamId GetTeam() const { return team; }
    void SetTeam(TeamId t)
    {
        team = t;
        if(graveTeam)
            *graveTeam = TeamId2Team[t];
    }

private:
    TeamId team;
};

class Wintergrasp : public CBattleground
{
protected:
	typedef std::map<uint32, BuildingState *> BuildingStateMap;
	BuildingStateMap m_buildingStates;
	TeamId m_defender;

    bool m_sendUpdate;
	CreatureSet m_creatures;
	CreatureSet m_vehicles[2];
	GameObjectSet m_gobjects;
	typedef std::set<Player*> PlayerSet;

public:

    typedef std::map<uint32/*lowguid*/, Wintergrasp*> CapturePointMap;

	bool m_wartime;
	bool m_changeDefender;
	uint32 m_timer;
	uint32 m_clock[5];
	uint32 m_workshopCount[2];
	uint32 m_towerCount[2][2];
	int32 m_tenacityStack;

	bool Update(uint32 diff);

	//Battle Controllers
	//void StartBattle();
	//void EndBattle();


	uint32 GetTimer() const { return m_timer / 1000; };
	TeamId GetTeam() const { return m_defender; };
	bool isWarTime() const { return m_wartime; };

	// Updates/Casts Player Tenacity Stack
	void UpdateTenacityStack();
	void CastTenacity(Unit *utr, int32 newStack);
	// Handle Player Enter/Leave Zone
	void HandlePlayerEnterZone(Player *plr, uint32 zone);
	void HandlePlayerLeaveZone(Player *plr, uint32 zone);
    // send world state update to all players present
    void SendUpdateWorldState(uint32 field, uint32 value);
    void BroadcastPacket(WorldPacket & data) const;
	//void UpdateAllWorldObject();
	bool UpdateCreatureInfo(Creature *creature) const;
	bool UpdateGameObjectInfo(GameObject *go) const;

	void UpdateClock();
	void UpdateClockDigit(uint32 &timer, uint32 digit, uint32 mod);
	void HookGenerateLoot(Player* plr, Corpse* pCorpse);
	void SendInitWorldStatesTo(Player *player = NULL) const;
};
