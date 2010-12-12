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

class SCRIPT_DECL Lacrimi : public ThreadContext
{
private:
	ConfigFile LacrimiConfig;
	Database* LacrimiDB;
	ScriptMgr* sMgr;
	bool database;
	bool config;

public:
	Lacrimi(ScriptMgr* mgr);
	~Lacrimi();

	bool run();
	void terminate();
	void Delay(uint32 time);
	bool _StartDB();
	void _StopDB();
	Database* GetLDB() { if(database == true) return LacrimiDB; return NULL; };

public: // Script Related
	void SetupScripts();
	void SetupZoneScripts();
	void SetupCityScripts();

///////////////////
////// Zone Setups.
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

};

#define LacrimiDatabase (*(sWorld.LacrimiPtr->GetLDB()))

#endif