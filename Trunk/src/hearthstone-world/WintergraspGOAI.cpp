#include "StdAfx.h"

enum OwnerStatus
{
	OWNER_NONE		= 0,
	OWNER_ALLIANCE	= 1,
	OWNER_HORDE	= 2
};

enum Workshops
{
	BROKEN_TEMPLE,
	WESTSPARK,
	SUNKEN_RING,
	EASTSPARK,
};

int32 g_WorkshopOwners[4] = { -1, -1, -1, -1 };

class WGWorkshopAI : public GameObjectAIScript
{
	map<uint32, uint32> StoredPlayers;
	uint32 Status;
	const char * ControlPointName;
	uint32 workshopid;
	uint32 m_workshopStatus;
public:
	static GameObjectAIScript *Create(GameObject * GO) { return new WGWorkshopAI(GO); }
	GameObject *pworkshop;

	WGWorkshopAI(GameObject *go) : GameObjectAIScript(go)
	{
		m_workshopStatus = OWNER_NONE;
		Status = 0;
		switch(go->GetEntry())
		{
		case WORKSHOP_W:
			ControlPointName = "Broken Temple Workshop";
			workshopid = BROKEN_TEMPLE;
			break;

		case WORKSHOP_SW:
			ControlPointName = "Westspark Workshop";
			workshopid = WESTSPARK;
			break;

		case WORKSHOP_E:
			ControlPointName = "The Sunken Ring Workshop";
			workshopid = SUNKEN_RING;
			break;

		case WORKSHOP_SE:
			ControlPointName = "Eastspark Workshop";
			workshopid = EASTSPARK;
			break;
		default:
			break;
		}
	}

	void AIUpdate()
	{
		uint32 plrcounts[2] = { 0, 0 };
		unordered_set<Player *>::iterator itr = _gameobject->GetInRangePlayerSetBegin();
		unordered_set<Player *>::iterator itrend = _gameobject->GetInRangePlayerSetEnd();
		map<uint32,uint32>::iterator it2, it3;
		uint32 timeptr = (uint32)UNIXTIME;
		bool in_range;
		bool is_valid;
		Player *plr = NULLPLR;

		for(; itr != itrend; ++itr)
		{
			if( (*itr)->WinterGrasp == NULL )
				is_valid = false;
			else
				is_valid = true;

			in_range = (_gameobject->GetDistance2dSq((*itr)) <= 600) ? true : false;

			it2 = StoredPlayers.find((*itr)->GetLowGUID());
			if( it2 == StoredPlayers.end() )
			{
				if( in_range )
				{
					StoredPlayers.insert(make_pair((*itr)->GetLowGUID(), timeptr));
					if( is_valid )
						plrcounts[(*itr)->GetTeam()]++;
				}
			}
			else
			{
				if( !in_range )
				{
					StoredPlayers.erase(it2);
				}
				else
				{
					it2->second = timeptr;
					if( is_valid )
						plrcounts[(*itr)->GetTeam()]++;
				}
			}
		}

		if( Status == 100 && m_workshopStatus != OWNER_ALLIANCE )
		{
			m_workshopStatus = OWNER_ALLIANCE;
			char Text[1024];
			snprintf(Text, 1024, "%sThe Alliance has captured %s!", MSG_COLOR_YELLOW, ControlPointName);
			sWintergraspI.GetWintergrasp()->_SendMessage(Text);
			sWintergraspI.GetWintergrasp()->numworkshop[0]++;
			g_WorkshopOwners[workshopid] = 1;
			SpawnEngineer(_gameobject->GetEntry(),0);
		}
		else if( Status == 0 && m_workshopStatus !=  OWNER_HORDE )
		{
			m_workshopStatus = OWNER_HORDE;
			char Text[1024];
			snprintf(Text, 1024, "%sThe Horde has captured %s!", MSG_COLOR_YELLOW, ControlPointName);
			sWintergraspI.GetWintergrasp()->_SendMessage(Text);
			sWintergraspI.GetWintergrasp()->numworkshop[1]++;
			g_WorkshopOwners[workshopid] = 0;
			SpawnEngineer(_gameobject->GetEntry(),1);
		}
		else if( m_workshopStatus != OWNER_NONE )
		{
			if( m_workshopStatus == OWNER_ALLIANCE && Status <= 50 )
			{
				m_workshopStatus = OWNER_NONE;
				sWintergraspI.GetWintergrasp()->numworkshop[0]--;
				char Text[1024];
				snprintf(Text, 1024, "%sThe Alliance has lost %s!", MSG_COLOR_YELLOW, ControlPointName);
				sWintergraspI.GetWintergrasp()->_SendMessage(Text);
				g_WorkshopOwners[workshopid] = -1;
			}
			else if( m_workshopStatus == OWNER_HORDE && Status >= 50 )
			{
				m_workshopStatus = OWNER_NONE;
				sWintergraspI.GetWintergrasp()->numworkshop[1]--;
				char Text[1024];
				snprintf(Text, 1024, "%sThe Horde has lost %s!", MSG_COLOR_YELLOW, ControlPointName);
				sWintergraspI.GetWintergrasp()->_SendMessage(Text);
				g_WorkshopOwners[workshopid] = -1;
			}
		}

		for(it2 = StoredPlayers.begin(); it2 != StoredPlayers.end(); )
		{
			it3 = it2;
			++it2;

			if( it3->second != timeptr )
			{
				StoredPlayers.erase(it3);
			}
		}

		uint32 delta;
		if( plrcounts[0] > plrcounts[1] )
		{
			delta = plrcounts[0] - plrcounts[1];
			delta *= 20;

			if( delta > 25 )
				delta = 25;

			Status += delta;
			if( Status >= 100 )
				Status = 100;
		}
		else if( plrcounts[1] > plrcounts[0] )
		{
			delta = plrcounts[1] - plrcounts[0];
			delta *= 20;

			if( delta > 25 )
				delta = 25;

			if( delta > Status )
				Status = 0;
			else
				Status -= delta;
		}
	}

	void OnSpawn()
	{
		m_workshopStatus = OWNER_NONE;
		RegisterAIUpdateEvent(3000);
	}
	void SpawnEngineer(uint32 Entry, uint32 Side)
	{
		/*switch(Entry)
		{
		case WORKSHOP_W:
			{
				float espawnx = 4964.009277f; float espawny = 3380.656006f; float espawnz = 384.826752f;
				if(Side == 0)
					_gameobject->GetMapMgr()->SpawnCreature(30499,espawnx,  espawny,  espawnz,0.0f);
				else
					_gameobject->GetMapMgr()->SpawnCreature(30400,espawnx,  espawny,  espawnz,0.0f);
			}break;
		case WORKSHOP_SW:
			{
				float espawnx = 4354.149902f; float espawny = 3312.820068f; float espawnz = 390.045990f;
				if(Side == 0)
					_gameobject->GetMapMgr()->SpawnCreature(30499,espawnx,  espawny,  espawnz,0.0f);
				else
					_gameobject->GetMapMgr()->SpawnCreature(30400,espawnx,  espawny,  espawnz,0.0f);
			}break;
		case WORKSHOP_E:
			{
				float espawnx = 4964.009277f; float espawny = 3380.656006f; float espawnz = 384.826752f;
				if(Side == 0)
					_gameobject->GetMapMgr()->SpawnCreature(30499,espawnx,  espawny,  espawnz,0.0f);
				else
					_gameobject->GetMapMgr()->SpawnCreature(30400,espawnx,  espawny,  espawnz,0.0f);
			}break;
		}*/
	}
};

enum Towers
{
	SHADOWSIGHT,
	WINTERSEDGE,
	FLAMEWATCH,
};

int32 g_TowerOwners[3] = { -1, -1, -1 };

class WGTowerAI : public GameObjectAIScript
{
	map<uint32, uint32> StoredPlayers;
	uint32 Status;
	const char * ControlPointName;
	uint32 Towerid;
	uint32 m_TowerStatus;
public:
	static GameObjectAIScript *Create(GameObject * GO) { return new WGTowerAI(GO); }
	GameObject *pTower;

	WGTowerAI(GameObject *go) : GameObjectAIScript(go)
	{
		m_TowerStatus = OWNER_NONE;
		Status = 0;
		switch(go->GetEntry())
		{
		case TOWER_SHADOWSIGHT:
			ControlPointName = "Shadowsight Tower";
			Towerid = SHADOWSIGHT;
			break;

		case TOWER_WINTERS_EDGE:
			ControlPointName = "Winter's Edge Tower";
			Towerid = WINTERSEDGE;
			break;

		case TOWER_FLAMEWATCH:
			ControlPointName = "Flamewatch Tower";
			Towerid = FLAMEWATCH;
			break;
		default:
			break;
		}
	}

	void AIUpdate()
	{
		uint32 plrcounts[2] = { 0, 0 };
		unordered_set<Player *>::iterator itr = _gameobject->GetInRangePlayerSetBegin();
		unordered_set<Player *>::iterator itrend = _gameobject->GetInRangePlayerSetEnd();
		map<uint32,uint32>::iterator it2, it3;
		uint32 timeptr = (uint32)UNIXTIME;
		bool in_range;
		bool is_valid;
		Player *plr = NULLPLR;
		if(m_TowerStatus != OWNER_NONE)
			return;
		for(; itr != itrend; ++itr)
		{
			if( (*itr)->WinterGrasp == NULL )
				is_valid = false;
			else
				is_valid = true;

			in_range = (_gameobject->GetDistance2dSq((*itr)) <= 600) ? true : false;

			it2 = StoredPlayers.find((*itr)->GetLowGUID());
			if( it2 == StoredPlayers.end() )
			{
				if( in_range )
				{
					StoredPlayers.insert(make_pair((*itr)->GetLowGUID(), timeptr));
					if( is_valid )
						plrcounts[(*itr)->GetTeam()]++;
				}
			}
			else
			{
				if( !in_range )
				{
					StoredPlayers.erase(it2);
				}
				else
				{
					it2->second = timeptr;
					if( is_valid )
						plrcounts[(*itr)->GetTeam()]++;
				}
			}
		}

		if( Status == 100 && m_TowerStatus != OWNER_ALLIANCE )
		{
			m_TowerStatus = OWNER_ALLIANCE;
			char Text[1024];
			snprintf(Text, 1024, "%sThe Alliance has captured %s!", MSG_COLOR_YELLOW, ControlPointName);
			sWintergraspI.GetWintergrasp()->_SendMessage(Text);
			g_TowerOwners[Towerid] = 1;
			_gameobject->GetMapMgr()->CastSpellOnPlayers(0,62064);
		}
		else if( Status == 0 && m_TowerStatus !=  OWNER_HORDE )
		{
			m_TowerStatus = OWNER_HORDE;
			char Text[1024];
			snprintf(Text, 1024, "%sThe Horde has captured %s!", MSG_COLOR_YELLOW, ControlPointName);
			sWintergraspI.GetWintergrasp()->_SendMessage(Text);
			g_TowerOwners[Towerid] = 0;
			_gameobject->GetMapMgr()->CastSpellOnPlayers(1,62064);
		}

		for(it2 = StoredPlayers.begin(); it2 != StoredPlayers.end(); )
		{
			it3 = it2;
			++it2;

			if( it3->second != timeptr )
			{
				StoredPlayers.erase(it3);
			}
		}

		uint32 delta;
		if( plrcounts[0] > plrcounts[1] )
		{
			delta = plrcounts[0] - plrcounts[1];
			delta *= 20;

			if( delta > 25 )
				delta = 25;

			Status += delta;
			if( Status >= 100 )
				Status = 100;
		}
		else if( plrcounts[1] > plrcounts[0] )
		{
			delta = plrcounts[1] - plrcounts[0];
			delta *= 20;

			if( delta > 25 )
				delta = 25;

			if( delta > Status )
				Status = 0;
			else
				Status -= delta;
		}
	}

	void OnSpawn()
	{
		m_TowerStatus = OWNER_NONE;
		RegisterAIUpdateEvent(3000);
	}
};

class WGTitanRelic : public GameObjectAIScript
{
public:
	static GameObjectAIScript *Create(GameObject * GO) { return new WGTitanRelic(GO); }

	WGTitanRelic(GameObject *go) : GameObjectAIScript(go){}
	void OnActivate(Player *pPlayer)
	{
		if(pPlayer->WinterGrasp == NULL)
			return;
		pPlayer->WinterGrasp->End(pPlayer);
	}
};

class WGVehicleTele : public GameObjectAIScript
{
public:
	static GameObjectAIScript *Create(GameObject * GO) { return new WGVehicleTele(GO); }

	WGVehicleTele(GameObject *go) : GameObjectAIScript(go){}
	void AIUpdate()
	{
		Object* veh;
		for (Object::InRangeSet::iterator itr = _gameobject->GetInRangeSetBegin(); itr != _gameobject->GetInRangeSetEnd(); ++itr)
		{
			veh = *itr;
			if (veh->IsVehicle() && veh->GetDistance2dSq(_gameobject->GetPositionX(), _gameobject->GetPositionY()) <= 4.0f && TO_VEHICLE(veh)->HasAura(49759) != true && TO_VEHICLE(veh)->GetControllingUnit() != NULL)
			{
				//TO_OBJECT(veh)->SetPosition(_gameobject->GetPositionX()-60.0f,_gameobject->GetPositionY(),_gameobject->GetPositionZ(),_gameobject->GetOrientation());
				_gameobject->CastSpell(veh,dbcSpell.LookupEntryForced(49759),true);
			}
		}
	}
	void OnSpawn()
	{
		RegisterAIUpdateEvent(5000);
	}
};

void WintergraspInternal::WintergraspGOAIRegister()
{
	sScriptMgr.register_gameobject_script(WORKSHOP_W, &WGWorkshopAI::Create);
	sScriptMgr.register_gameobject_script(WORKSHOP_SW, &WGWorkshopAI::Create);
	sScriptMgr.register_gameobject_script(WORKSHOP_E, &WGWorkshopAI::Create);
	sScriptMgr.register_gameobject_script(WORKSHOP_SE, &WGWorkshopAI::Create);
	sScriptMgr.register_gameobject_script(TOWER_SHADOWSIGHT, &WGTowerAI::Create);
	sScriptMgr.register_gameobject_script(TOWER_WINTERS_EDGE, &WGTowerAI::Create);
	sScriptMgr.register_gameobject_script(TOWER_FLAMEWATCH, &WGTowerAI::Create);
	sScriptMgr.register_gameobject_script(TITAN_RELIC, &WGTitanRelic::Create);
	sScriptMgr.register_gameobject_script(VEHICLE_TELEPORTER, &WGVehicleTele::Create);
}