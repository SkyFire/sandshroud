/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#ifndef PvPMgr_H
#define PvPMgr_h

#include "StdAfx.h"
#include "Setup.h"

#define CLEAN_DELAY 20000

class PvPMgr
{
	private:
		bool IsInTurny;
	
	public:
		uint32 Index; // Player GUID
		uint32 VP;
		uint32 CurrentKills;
		uint32 PlayersHealed;
		uint32 TotalDeaths;
		uint32 LastGuid;
		bool killingSpree;
		bool killingSpree2;
		
		PvPMgr();
		bool ChecksPassed(Player * Attcker, Player * Vct);
};

class PvPStorage
{
	private:
		static PvPStorage *Instance;
		vector<PvPMgr*> Vect;
		vector<PvPMgr*>::iterator Itr;
		Mutex StorageLock;
		
		PvPStorage();
		void CleanVect();

	public:
		static PvPStorage *GetInstance();
		PvPMgr *CreateMgr(const uint32 &Index);
		PvPMgr *GetMgr(const uint32 &Index, const bool Create);
		vector<PvPMgr*> GetVect() 
		{
			return Vect;
		}
};

#endif