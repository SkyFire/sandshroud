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

Database* LacrimiDB;
#define LacrimiDatabase (*LacrimiDB)

class SCRIPT_DECL Lacrimi : public ThreadContext
{
private:
	ScriptMgr* sMgr;

public:
	Lacrimi(ScriptMgr* mgr);
	~Lacrimi();

	bool run();
	void terminate();
	void Delay(uint32 time);

public: // Script Related
	void SetupScripts();
	void SetupZoneScripts();

	// Zone Setups.

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
	void SetupIronforge();
	void SetupSilvermoon();
	void SetupStormwind();
	void SetupUndercity();

	void SetupKalimdorScripts();

	void SetupOutlandScripts();

	void SetupEbonHoldScripts();

	void SetupNorthrendScripts();
};

#endif