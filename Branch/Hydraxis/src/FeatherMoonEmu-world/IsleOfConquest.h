/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#define IOC_NUM_REINFORCEMENTS 0

enum IOCControlPoints
{
	// TODO.
	IOC_NUM_CONTROL_POINTS		= 0,
};

class IsleOfConquest : public CBattleground
{
public:
	// Isle of Conquest definition
	IsleOfConquest(MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t);
	// Delete Isle of Conquest
	~IsleOfConquest();
	// Initialize
	virtual void Init();

	// Hooks, the following are required.
	// Activate on player death.
	void HookOnPlayerDeath(Player* plr);
	// Activate on flag drop. Isle of Conquest has no flag.
	void HookFlagDrop(Player* plr, GameObject* obj);
	// Activate on spawn. Isle of Conquest has no flag.
	void HookFlagStand(Player* plr, GameObject* obj);
	// Activate on player mount.
	void HookOnMount(Player* plr);
	// Activate on player passing through areatrigger.
	void HookOnAreaTrigger(Player* plr, uint32 id);
	// Activate on player repoping.
	bool HookHandleRepop(Player* plr);
	// Activate on adding player to battleground.
	void OnAddPlayer(Player* plr);
	// Activate on removing player from battleground.
	void OnRemovePlayer(Player* plr);
	// Activate on creation of the battleground.
	void OnCreate();
	// Activate on player killing anything.
	void HookOnPlayerKill(Player* plr, Unit* pVictim);
	// Activate on player killing player.
	void HookOnHK(Player* plr);
	// Never used.
	void HookOnShadowSight();
	// Activate on platform Teleport
//	void OnPlatformTeleport(Player* plr);
	// Activate on Respawn
//	void Respawn();
	// Map supports player looting player bodies.
	bool SupportsPlayerLoot() { return false; }
	// Activate on Slow Lock Open
	bool HookSlowLockOpen(GameObject* pGo, Player* pPlayer, Spell* pSpell);
	// Generate Player loot.
	void HookGenerateLoot(Player* plr, Corpse* pCorpse);
	// End Hooks

	// Sets starting locations.
	LocationVector GetStartingCoords(uint32 Team);
	// Gets the name of the battleground.
	const char * GetName() { return "Isle of Conquest"; }

	// Create Function
	static CBattleground* Create(MapMgr* m, uint32 i, uint32 l, uint32 t) { return (new IsleOfConquest(m, i, l, t)); }
private:
	// Is captian of that team alive?
	bool m_LiveCaptain[2];
	// Bonus Honor from battleground.
	int m_bonusHonor;
	// The amount of reinforcements the team has.
	uint32 m_reinforcements[2];
	// IoC capture nodes, todo.
//	IOCNode *m_nodes[IOC_NUM_CONTROL_POINTS];
};