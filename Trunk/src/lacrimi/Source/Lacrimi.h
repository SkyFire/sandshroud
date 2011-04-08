/*
 * Lacrimi Scripts Copyright 2010 - 2011
 *
 * ############################################################
 * # ##            #       ####### ####### ##    #    #    ## #
 * # ##           ###      ##      ##   ## ##   ###  ###   ## #
 * # ##          ## ##     ##      ##   ## ##   ###  ###   ## #
 * # ##         #######    ##      ####### ##  ## #### ##  ## #
 * # ##        ##     ##   ##      #####   ##  ## #### ##  ## #
 * # ##       ##       ##  ##      ##  ##  ## ##   ##   ## ## #
 * # ####### ##         ## ####### ##   ## ## ##   ##   ## ## #
 * # :::::::.::.........::.:::::::.::...::.::.::...::...::.:: #
 * ############################################################
 *
 */

#ifndef LACRIMI_H
#define LACRIMI_H

class LuaEngine;
class LuaEngineMgr;

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, Plr); \
	Menu->AddItem(0, "I was looking for something else.", 0); \
	Menu->SendTo(Plr);

class SCRIPT_DECL Lacrimi : public ThreadContext
{
private:
	ConfigFile LacrimiConfig;
	Database* LacrimiDB;
	ScriptMgr* sMgr;
	bool dumpstats;
	bool database;
	bool config;
	bool first;

public:
	Lacrimi(ScriptMgr* mgr);
	~Lacrimi();

	bool run();
	void terminate();
	void Delay(uint32 time);
	bool _StartDB();
	void _StopDB();
	ScriptMgr* GetScriptMgr() { return sMgr; };
	char* GetConfigString(char* configfamily, char* configoption, char* cdefault = "");
	float GetConfigfloat(char* configfamily, char* configoption, float fdefault = 0.0f);
	bool GetConfigBool(char* configfamily, char* configoption, bool bdefault = false);
	int GetConfigInt(char* configfamily, char* configoption, int intdefault = 0);
	Database* GetLDB() { if(database == true) return LacrimiDB; return NULL; };

public: // Script Related
	void SetupScripts();
	void SetupZoneScripts();
	void SetupCityScripts();

///////////////////
////// Zone Setups. // Level 0
///////////////////

	// Eastern Kingdom
	void SetupEasternKingdomScripts();
	void SetupAlteracMountains();
	void SetupArathiHighlands();
	void SetupBlastedLands();
	void SetupBurningSteppes();
	void SetupDunMorogh();
	void SetupDuskwood();
	void SetupEasternPlaguelands();
	void SetupElwynnForest();
	void SetupEversongWoods();
	void SetupGhostlands();
	void SetupHinterlands();
	void SetupIsleOfQuelDanas();
	void SetupLochModan();
	void SetupSearingGorge();
	void SetupSilverpineForest();
	void SetupStranglethornVale();
	void SetupTrisfalGlades();
	void SetupWesternPlaguelands();
	void SetupWestfall();
	void SetupWetlands();

	// Kalimdor
	void SetupKalimdorScripts();
	void SetupAshenvale();
	void SetupAzshara();
	void SetupAzuremystIsle();
	void SetupBloodmystIsle();
	void SetupDarkshore();
	void SetupDesolace();
	void SetupDustwallowMarsh();
	void SetupFelwood();
	void SetupFeralas();
	void SetupMoonglade();
	void SetupMulgore();
	void SetupSilithus();
	void SetupStonetalonMountains();
	void SetupTanaris();
	void SetupTeldrassil();
	void SetupTheBarrens();
	void SetupThousandNeedles();
	void SetupUngoroCrater();
	void SetupWinterspring();

	// Outland
	void SetupOutlandScripts();
	void SetupBladesEdgeMountains();
	void SetupHellfirePeninsula();
	void SetupNagrand();
	void SetupNetherstorm();
	void SetupShadowmoonValley();
	void SetupTerrokarForest();
	void SetupZangarmarsh();

	// Northrend
	void SetupNorthrendScripts();
	void SetupBoreanTundra();
	void SetupDragonblight();
	void SetupGrizzlyHills();
	void SetupHowlingFjord();
	void SetupIcecrown();
	void SetupSholazarBasin();
	void SetupStormPeaks();
	void SetupZulDrak();

	// Ebon Hold
	void SetupEbonHoldScripts();

///////////////////
////// City Setups.
///////////////////

	// Eastern Kingdom
	void SetupIronforge();
	void SetupSilvermoon();
	void SetupStormwind();
	void SetupUndercity();

	// Kalimdor
	void SetupDarnassus();
	void SetupExodar();
	void SetupOrgrimmar();
	void SetupThunderbluff();

	// Outlands
	void SetupShattrath();

	// Northrend
	void SetupDalaran();

public: // Instance Scripts
	void SetupInstanceScripts();

///////////////////
////// Classic Scripts // Level 2
///////////////////
	void SetupBlackfathomDeeps();

public: // Stat Dumper
	void DumpStats();
	void GenerateUptimeString(char * Dest);
	void FillOnlineTime(uint32 Time, char * Dest);

	char Filename[MAX_PATH];

public: // LuaEngine
	LuaEngine* L_LuaEngine;
	LuaEngineMgr* L_LuaEngineMgr;
	bool LuaEngineIsStarting;
};

#define sLacrimi (*sWorld.LacrimiPtr)

#define LacrimiDatabase (*(sWorld.LacrimiPtr->GetLDB()))

#define g_engine (sLacrimi.L_LuaEngine)
#define g_luaMgr (*sLacrimi.L_LuaEngineMgr)
#define LuaEvent (g_luaMgr.LuaEventMgr)

#endif