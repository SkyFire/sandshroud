/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#include "PvPMgr.h"

PvPMgr::PvPMgr()
{
	Index = 0;
	CurrentKills = 0;
	PlayersHealed = 0;
	TotalDeaths = 0;
	LastGuid = 0;
	VP = 0;
	IsInTurny = false;
}

bool PvPMgr::ChecksPassed(Player * Attcker, Player * Vct)
{
	string Aip;
	string Vip;

	if(Attcker->GetSession()->HasGMPermissions() || Vct->GetSession()->HasGMPermissions())
		return true; // gm over-ride for testing

	if(Attcker == Vct)
		return false;

	if(Attcker->GetSession()->GetSocket() == NULL || Vct->GetSession()->GetSocket() == NULL)
		return false;

	Aip = Attcker->GetSession()->GetSocket()->GetRemoteIP();
	Vip = Vct->GetSession()->GetSocket()->GetRemoteIP();

	if(Aip.compare(Vip) == 0)
		return false;

	if(Attcker->GetLowGUID() == LastGuid)
		return false;

	return true;
}

PvPStorage *PvPStorage::Instance = NULL;

PvPStorage::PvPStorage()
{

}

PvPStorage *PvPStorage::GetInstance()
{
	if(Instance == NULL)
		Instance = new PvPStorage();
	return Instance;
}

PvPMgr *PvPStorage::CreateMgr(const uint32 &Index)
{
	PvPMgr *Mgr = new PvPMgr();

	if(Mgr == NULL)
		return NULL; // no memory to allocate

	Mgr->Index = Index;
	
	StorageLock.Acquire();
		Vect.push_back(Mgr);
	StorageLock.Release();
	return Mgr;
}

PvPMgr *PvPStorage::GetMgr(const uint32 &Index, const bool Create)
{
	StorageLock.Acquire();
	for(Itr = Vect.begin();Itr != Vect.end();Itr++)
	{
		if((*Itr)->Index == Index)
		{
			StorageLock.Release();
			return (*Itr);
		}

	}
	StorageLock.Release();

	if(Create)
		return CreateMgr(Index);
	return NULL;
}

void PvPStorage::CleanVect()
{
	vector<PvPMgr*> CleanVect;
	uint32 Pos = 0;

	if(Vect.empty())
		return;

	StorageLock.Acquire();
	for(Itr = Vect.begin();Itr != Vect.end();Itr++)
	{
		Player * Eraser = objmgr.GetPlayer((*Itr)->Index);
		if(Eraser == NULL || Eraser->IsInWorld() == false || Eraser->GetSession() == NULL)
		{
			(*Itr)->VP = Pos;
			CleanVect.push_back((*Itr));
	
		}
		Pos++;
	}

	for(Itr = CleanVect.begin();Itr != CleanVect.end();Itr++)
	{
		Vect.erase(Vect.begin()+(*Itr)->VP);
		delete (*Itr);

	}
	StorageLock.Release();
	CleanVect.clear();

}