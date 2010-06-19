/*
 * Sun++ Scripts
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunplusplus.info/> 
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, or 
 * any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License 
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */
 
#include "StdAfx.h"
#include "Setup.h"
#include "Base.h"

/************************************************************************/
/* Raid_CoT_BattleForMountHyjal.cpp Script								*/
/************************************************************************/
/* Everything that isn't boss by Soulshifter
* Gracias a todos los que me ayudaron en escribir este gran script (7549 líneas!!!). A Prisma, Neltharion y Nano por recopilar información. A VirtualAngel por ayudarme con el sistema de puertas. (Thanks to VirtualAngel by helping me with the gates system. BIG Thanks for you ^^.
* Soulshifter GM DEV One WoW Chile.
* http://wowchile.one.cl
*/
#define GENERIC_ENRAGE 19953 //enrege usado por archimonde ya que el script de archimonde de soulshifter crasheaba apenas  empesaba el combatstart ^^ (patoman)

//Coords
// Waves Jaina
float PosX = 4990.216797f;
float PosY = -1721.132690f;
float PosZ = 1335.025513f;
float PosO = 5.768376f;
// Waves Thrall
float PosXa = 5546.981445f;
float PosYa = -2592.005859f;
float PosZa = 1479.623535f;
float PosOa = 4.662660f;
// Jaina coords
float JainaX = 5083.910156f;
float JainaY = -1789.040039f;
float JainaZ = 1322.569946f;
// Thrall coords
float ThrallX = 5449.970215f;
float ThrallY = -2723.770020f;
float ThrallZ = 1485.670044f;
// Eventos usados
enum HyjalPhases
{
	HYJAL_PHASE_NOT_STARTED = 1,
	HYJAL_PHASE_RAGE_WINTERCHILL_COMPLETE,
	HYJAL_PHASE_ANETHERON_COMPLETE,
	HYJAL_PHASE_KAZROGAL_COMPLETE,
	HYJAL_PHASE_AZGALOR_COMPLETE,
	HYJAL_PHASE_ARCHIMONDE_COMPLETE
};
uint32 HyjalPhase[1000000];
bool Started = false;
bool Tyrande = false;
#define CN_TRIGGER_HYJALA	90002
#define CN_TYRANDE			17948
#define CN_GHOUL			17895
#define CN_CRYPT			17897
#define CN_ABOMINATION		17898
#define CN_NECROMANCER		17899
#define CN_GINFE			17908
#define CN_FSTAL			17916
#define CN_BANSH			17905
#define CN_FWYRM			17907
#define CN_GARGO			17906

class TriggerHyjalaAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(TriggerHyjalaAI);
    TriggerHyjalaAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->GetAIInterface()->m_canMove = false;
		RegisterAIUpdateEvent(1000);//TIEMPO
		EventTimer = 1;
		Jaina = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(JainaX, JainaY, JainaZ, 17772);
		Thrall = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(ThrallX, ThrallY, ThrallZ, 17852);
	}
	void AIUpdate()
		{
			EventTimer++;
			if(HyjalPhase[_unit->GetInstanceID()] == HYJAL_PHASE_NOT_STARTED) //Ragewinter
				{
				switch(EventTimer)
					{
						case 4:
							Success();
							break;
						case 15:
							Incoming();
							break;
						case 20:	//Wave 1. 10 ghouls
							Rally();
							Wave1();
							break;
						case 135:
							Incoming();
							break;
						case 140:	//Wave 2. 10 ghouls & 2 CF
							Rally();
							Wave2();
							break;
						case 255:
							Incoming();
							break;
						case 260:	//Wave 3. 6 Ghouls & 6 CF
							Rally();
							Wave3();
							break;
						case 375:
							Incoming();
							break;
						case 380:	//Wave 4. 6 ghouls & 4 CF & 2 Necro.
							Rally();
							Wave4();
							break;
						case 495:
							Incoming();
							break;
						case 500:	//Wave 5. 2 Ghoul, 6 Crypt Fiends, 4 Necromancer.
							Rally();
							Wave5();
							break;
						case 615:
							Incoming();
							break;
						case 620:	//Wave 6.	 6 Ghoul, 6 Abomination
							Rally();
							Wave6();
							break;
						case 735:
							Incoming();
							break;
						case 740:	//Wave 7.	 4 Ghoul, 4 Necromancer, 4 Abomination
							Rally();
							Wave7();
							break;
						case 855:
							Incoming();
							break;
						case 860:	//Wave 8. 	 6 Ghoul, 4 Crypt Fiends, 2 Abomination, 2 Necromancer
							Rally();
							Wave8();
							break;
						case 980:
							UnitPointer RageW = NULLUNIT;
							RageW = _unit->GetMapMgr()->GetInterface()->SpawnCreature(17767, PosX, PosY, PosZ, PosO, true, false, 0, 0);
							RageW->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
							Jaina->SetUInt32Value(UNIT_NPC_FLAGS, 1);
							Reset();
							break;	
					}
				}
			if(HyjalPhase[_unit->GetInstanceID()] == HYJAL_PHASE_RAGE_WINTERCHILL_COMPLETE) // Anetheron
				{
					switch(EventTimer)
					{
						case 4:
							Success();
							break;	
						case 15:
							Incoming();
							break;
						case 20:	//Wave 1. 10 ghouls
							Rally();
							Wave1b();
							break;
						case 135:
							Incoming();
							break;
						case 140:	//Wave 2. 8 ghouls & 4 Abo
							Rally();
							Wave2b();
							break;
						case 255:
							Incoming();
							break;
						case 260:	//Wave 3. 4 Ghouls & 4 CF & 4 necro
							Rally();
							Wave3b();
							break;
						case 375:
							Incoming();
							break;
						case 380:	//Wave 4. 4 CF & 4 Necro & 4 Banshee.
							Rally();
							Wave4b();
							break;
						case 495:
							Incoming();
							break;
						case 500:	//Wave 5. 6 Ghoul, 2 NEcro, 4 Banshee.
							Rally();
							Wave5b();
							break;
						case 615:
							Incoming();
							break;
						case 620:	//Wave 6.	 4 Ghoul, 2 Abomination, 4 Necro
							Rally();
							Wave6b();
							break;
						case 735:
							Incoming();
							break;
						case 740:	//Wave 7.	 4 CF, 4 Abomination, 2 Banshee, 2 Ghoul.
							Rally();
							Wave7b();
							break;
						case 855:
							Incoming();
							break;
						case 860:	//Wave 8. 	 3 Ghoul, 3 Crypt Fiends, 4 Abomination, 2 Necromancer, 2 Banshee
							Rally();
							Wave8b();
							break;
						case 980:	//Boss
							UnitPointer Anetheron = NULLUNIT;
							Anetheron = _unit->GetMapMgr()->GetInterface()->SpawnCreature(17808, PosX, PosY, PosZ, PosO, true, false, 0, 0);
							Anetheron->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
							Jaina->SetUInt32Value(UNIT_NPC_FLAGS, 1);
							Reset();
							break;
					}
				}
			if(HyjalPhase[_unit->GetInstanceID()] == HYJAL_PHASE_ANETHERON_COMPLETE) // Kazrogal
				{
					switch(EventTimer)
						{
							case 4:
								Success();
								break;		
							case 15:
								Incoming2();
								break;
							case 20:	//Wave 1. 4 ghouls, 4 ABO, 2 Banshee, 2 Necro
								Rally2();
								Wave1c();
								break;
							case 135:
								Incoming2();
								break;
							case 140:	//Wave 2. 2 ghouls & 10 Gargo
								Rally2();
								Wave2c();
								break;
							case 255:
								Incoming2();
								break;
							case 260:	//Wave 3. 6 Ghouls & 6 CF & 2 necro
								Rally2();
								Wave3c();
								break;
							case 375:
								Incoming2();
								break;
							case 380:	//Wave 4. 6 CF & 2 Necro & 6 Gargo.
								Rally2();
								Wave4c();
								break;
							case 495:
								Incoming2();
								break;
							case 500:	//Wave 5. 4 Ghoul, 4 NEcro, 6 Abo.
								Rally2();
								Wave5c();
								break;
							case 615:
								Incoming2();
								break;
							case 620:	//Wave 6.	 8 Gargo, 1 Frost Wyrm.
								Rally2();
								Wave6c();
								break;
							case 735:
								Incoming2();
								break;
							case 740:	//Wave 7.	 4 Abomination, 1 Frost Wyrm, 6 Ghoul.
								Rally2();
								Wave7c();
								break;
							case 855:
								Incoming2();
								break;
							case 860:	//Wave 8. 	 6 Ghoul, 2 Crypt Fiends, 4 Abomination, 2 Necromancer, 2 Banshee
								Rally2();
								Wave8c();
								break;
							case 980:
								UnitPointer Kazrogal = NULLUNIT;
								Kazrogal = _unit->GetMapMgr()->GetInterface()->SpawnCreature(17888, PosXa, PosYa, PosZa, PosOa, true, false, 0, 0);
								Kazrogal->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
								Thrall->SetUInt32Value(UNIT_NPC_FLAGS, 1);
								Reset();
								break;
						}
				}
			if (HyjalPhase[_unit->GetInstanceID()] == HYJAL_PHASE_KAZROGAL_COMPLETE)	//Azgalor
				{
					switch(EventTimer)
						{
							case 4:
								Success();
								break;
							case 15:
								Incoming2();
								break;
							case 20:	//Wave 1. 6 ABO, 6 Necro
								Rally2();
								Wave1d();
								break;
							case 135:
								Incoming2();
								break;
							case 140:	//Wave 2. 5 ghouls, 8 Gargo, 1 Fwyrm
								Rally2();
								Wave2d();
								break;
							case 255:
								Incoming2();
								break;
							case 260:	//Wave 3. 6 Ghouls & 8 Giant Infernal
								Rally2();
								Wave3d();
								break;
							case 375:
								Incoming2();
								break;
							case 380:	//Wave 4. 6 FS & 8 Giant Infernal.
								Rally2();
								Wave4d();
								break;
							case 495:
								Incoming2();
								break;
							case 500:	//Wave 5. 6 Fel Stalker, 4 NEcro, 4 Abo.
								Rally2();
								Wave5d();
								break;
							case 615:
								Incoming2();
								break;
							case 620:	//Wave 6.	 6 Necro, 6 Banshee
								Rally2();
								Wave6d();
								break;
							case 735:
								Incoming2();
								break;
							case 740:	//Wave 7.	 2 Fel Stalker, 8 Giant Infernal, 2 Ghoul, 2 CF
								Rally2();
								Wave7d();
								break;
							case 855:
								Incoming2();
								break;
							case 860:	//Wave 8. 	 2 Fel Stalker, 4 Crypt Fiends, 4 Abomination, 2 Necromancer, 4 Banshee
								Rally2();
								Wave8d();
								break;
							case 980:
								UnitPointer Azgalor = NULLUNIT;
								Azgalor = _unit->GetMapMgr()->GetInterface()->SpawnCreature(17842, PosXa, PosYa, PosZa, PosOa, true, false, 0, 0);
								Azgalor->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
								Thrall->SetUInt32Value(UNIT_NPC_FLAGS, 1);
								Reset();
								break;
						}
				}
			if(HyjalPhase[_unit->GetInstanceID()] == HYJAL_PHASE_AZGALOR_COMPLETE) // Archimonde
				{
					switch(EventTimer)
					{
					case 3:
						UnitPointer Archimonde = NULLUNIT;
						Archimonde = _unit->GetMapMgr()->GetInterface()->SpawnCreature(17968, 5641.959961f, -3445.429932f, 1586.349976f, 3.663890f, true, false, 0, 0);
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "All of your efforts have been in vain, for the draining of the World Tree has already begun. Soon the heart of your world will beat no more.");
						_unit->PlaySoundToSet(10986);
						Reset();
						break;
					}
				}
		}
	void Wave1()// 5 trash
		{
			UnitPointer Ghoul1 = NULLUNIT;
			UnitPointer Ghoul2 = NULLUNIT;
			UnitPointer Ghoul3 = NULLUNIT;
			UnitPointer Ghoul4 = NULLUNIT;
			UnitPointer Ghoul5 = NULLUNIT;
		//	UnitPointer Ghoul6 = NULLUNIT;
		//	UnitPointer Ghoul7 = NULLUNIT;
		//	UnitPointer Ghoul8 = NULLUNIT;
		//	UnitPointer Ghoul9 = NULLUNIT;
		//	UnitPointer Ghoul10 = NULLUNIT;
			Ghoul1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, PosY, PosZ, PosO, false, false, 0, 0);
			Ghoul2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
			Ghoul3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
			Ghoul4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
			Ghoul5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
		//	Ghoul6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY + 2.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul7 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY - 2.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul8 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY + 4.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul9 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY - 4.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul10 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 3.0f), (PosY + 3.0f), PosZ, PosO, false, false, 0, 0);
			Ghoul1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Ghoul2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Ghoul3->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Ghoul4->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Ghoul5->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul6->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul7->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul8->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul9->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul10->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		}
	void Wave2() // 5 trash
		{
				UnitPointer Ghoul1a = NULLUNIT;
				UnitPointer Ghoul2a = NULLUNIT;
				UnitPointer Ghoul3a = NULLUNIT;
			//	UnitPointer Ghoul4a = NULLUNIT;
			//	UnitPointer Ghoul5a = NULLUNIT;
			//	UnitPointer Ghoul6a = NULLUNIT;
			//	UnitPointer Ghoul7a = NULLUNIT;
			//	UnitPointer Ghoul8a = NULLUNIT;
			//	UnitPointer Ghoul9a = NULLUNIT;
			//	UnitPointer Ghoul10a = NULLUNIT;
				UnitPointer CryptFiend1a = NULLUNIT;
				UnitPointer CryptFiend2a = NULLUNIT;
				CryptFiend1a = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosX, PosY, PosZ, PosO, false, false, 0, 0);
				CryptFiend2a = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX + 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
				Ghoul1a = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
				Ghoul2a = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
				Ghoul3a = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
			//	Ghoul4a = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY + 2.0f), PosZ, PosO, false, false, 0, 0);
			//	Ghoul5a = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY - 2.0f), PosZ, PosO, false, false, 0, 0);
			//	Ghoul6a = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY + 4.0f), PosZ, PosO, false, false, 0, 0);
			//	Ghoul7a = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY - 4.0f), PosZ, PosO, false, false, 0, 0);
			//	Ghoul8a = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 3.0f), (PosY + 3.0f), PosZ, PosO, false, false, 0, 0);
			//	Ghoul9a = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 3.0f), (PosY - 3.0f), PosZ, PosO, false, false, 0, 0);
			//	Ghoul10a = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 5.0f), (PosY + 5.0f), PosZ, PosO, false, false, 0, 0);
				Ghoul1a->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
				Ghoul2a->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
				Ghoul3a->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			//	Ghoul4a->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			//	Ghoul5a->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			//	Ghoul6a->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			//	Ghoul7a->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			//	Ghoul8a->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			//	Ghoul9a->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			//	Ghoul10a->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
				CryptFiend1a->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
				CryptFiend2a->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		}
	void Wave3() // 5 trash
	{
			UnitPointer Ghoul1b = NULLUNIT;
			UnitPointer Ghoul2b = NULLUNIT;
			UnitPointer Ghoul3b = NULLUNIT;
		//	UnitPointer Ghoul4b = NULLUNIT;
		//	UnitPointer Ghoul5b = NULLUNIT;
		//	UnitPointer Ghoul6b = NULLUNIT;
			UnitPointer CryptFiend1b = NULLUNIT;
			UnitPointer CryptFiend2b = NULLUNIT;
		//	UnitPointer CryptFiend3b = NULLUNIT;
		//	UnitPointer CryptFiend4b = NULLUNIT;
		//	UnitPointer CryptFiend5b = NULLUNIT;
		//	UnitPointer CryptFiend6b = NULLUNIT;
			CryptFiend1b = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosX, PosY, PosZ, PosO, false, false, 0, 0);
			CryptFiend2b = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX + 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
		//	CryptFiend3b = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX - 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
		//	CryptFiend4b = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX + 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
		//	CryptFiend5b = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX - 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
		//	CryptFiend6b = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosX, (PosY + 2.0f), PosZ, PosO, false, false, 0, 0);
			Ghoul1b = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY - 2.0f), PosZ, PosO, false, false, 0, 0);
			Ghoul2b = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY + 4.0f), PosZ, PosO, false, false, 0, 0);
			Ghoul3b = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY - 4.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul4b = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 3.0f), (PosY + 3.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul5b = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 3.0f), (PosY - 3.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul6b = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 5.0f), (PosY + 5.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul1b->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul2b->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul3b->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul4b->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul5b->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul6b->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			CryptFiend1b->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			CryptFiend2b->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	CryptFiend3b->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	CryptFiend4b->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	CryptFiend5b->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	CryptFiend6b->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	}
	void Wave4()//5 trash
	{
		UnitPointer Ghoul1c = NULLUNIT;
		UnitPointer Ghoul2c = NULLUNIT;
	//	UnitPointer Ghoul3c = NULLUNIT;
	//	UnitPointer Ghoul4c = NULLUNIT;
	//	UnitPointer Ghoul5c = NULLUNIT;
	//	UnitPointer Ghoul6c = NULLUNIT;
		UnitPointer CryptFiend1c = NULLUNIT;
		UnitPointer CryptFiend2c = NULLUNIT;
	//	UnitPointer CryptFiend3c = NULLUNIT;
	//	UnitPointer CryptFiend4c = NULLUNIT;
		UnitPointer Necromancer1c = NULLUNIT;
	//	UnitPointer Necromancer2c = NULLUNIT;
		Necromancer1c = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, PosY, PosZ, PosO, false, false, 0, 0);
	//	Necromancer2c = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
		CryptFiend1c = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX - 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
		CryptFiend2c = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX + 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
	//	CryptFiend3c = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX - 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
	//	CryptFiend4c = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosX, (PosY + 2.0f), PosZ, PosO, false, false, 0, 0);
		Ghoul1c = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY - 2.0f), PosZ, PosO, false, false, 0, 0);
		Ghoul2c = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY + 4.0f), PosZ, PosO, false, false, 0, 0);
	//	Ghoul3c = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY - 4.0f), PosZ, PosO, false, false, 0, 0);
	//	Ghoul4c = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 3.0f), (PosY + 3.0f), PosZ, PosO, false, false, 0, 0);
	//	Ghoul5c = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 3.0f), (PosY - 3.0f), PosZ, PosO, false, false, 0, 0);
	//	Ghoul6c = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 5.0f), (PosY + 5.0f), PosZ, PosO, false, false, 0, 0);
		Ghoul1c->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		Ghoul2c->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Ghoul3c->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Ghoul4c->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Ghoul5c->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	///	Ghoul6c->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		CryptFiend1c->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		CryptFiend2c->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	CryptFiend3c->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	CryptFiend4c->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		Necromancer1c->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Necromancer2c->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
}
	void Wave5() // 5 trash
	{
		UnitPointer Ghoul1d = NULLUNIT;
	//	UnitPointer Ghoul2d = NULLUNIT;
		UnitPointer CryptFiend1d = NULLUNIT;
		UnitPointer CryptFiend2d = NULLUNIT;
	//	UnitPointer CryptFiend3d = NULLUNIT;
	//	UnitPointer CryptFiend4d = NULLUNIT;
	//	UnitPointer CryptFiend5d = NULLUNIT;
	//	UnitPointer CryptFiend6d = NULLUNIT;
		UnitPointer Necromancer1d = NULLUNIT;
		UnitPointer Necromancer2d = NULLUNIT;
	//	UnitPointer Necromancer3d = NULLUNIT;
	//	UnitPointer Necromancer4d = NULLUNIT;
		Necromancer1d = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, PosY, PosZ, PosO, false, false, 0, 0);
		Necromancer2d = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
	//	Necromancer3d = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
	//	Necromancer4d = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
		CryptFiend1d = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX - 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
		CryptFiend2d = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosX, (PosY + 2.0f), PosZ, PosO, false, false, 0, 0);
	//	CryptFiend3d = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosX, (PosY - 2.0f), PosZ, PosO, false, false, 0, 0);
	//	CryptFiend4d = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosX, (PosY + 4.0f), PosZ, PosO, false, false, 0, 0);
	//	CryptFiend5d = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosX, (PosY - 4.0f), PosZ, PosO, false, false, 0, 0);
	//	CryptFiend6d = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX + 3.0f), (PosY + 3.0f), PosZ, PosO, false, false, 0, 0);
		Ghoul1d = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 3.0f), (PosY - 3.0f), PosZ, PosO, false, false, 0, 0);
	//	Ghoul2d = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 5.0f), (PosY + 5.0f), PosZ, PosO, false, false, 0, 0);
		Ghoul1d->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Ghoul2d->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		CryptFiend1d->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		CryptFiend2d->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	CryptFiend3d->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	CryptFiend4d->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	CryptFiend5d->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	CryptFiend6d->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		Necromancer1d->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		Necromancer2d->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Necromancer3d->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Necromancer4d->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	}
	void Wave6()// 5 trash
	{
		UnitPointer Ghoul1e = NULLUNIT;
		UnitPointer Ghoul2e = NULLUNIT;
		UnitPointer Ghoul3e = NULLUNIT;
	//	UnitPointer Ghoul4e = NULLUNIT;
	//	UnitPointer Ghoul5e = NULLUNIT;
	//	UnitPointer Ghoul6e = NULLUNIT;
		UnitPointer Abomination1e = NULLUNIT;
		UnitPointer Abomination2e = NULLUNIT;
	//	UnitPointer Abomination3e = NULLUNIT;
	//	UnitPointer Abomination4e = NULLUNIT;
	//	UnitPointer Abomination5e = NULLUNIT;
	//	UnitPointer Abomination6e = NULLUNIT;
		Ghoul1e = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, PosY, PosZ, PosO, false, false, 0, 0);
		Ghoul2e = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
		Ghoul3e = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
	//	Ghoul4e = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
	//	Ghoul5e = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
	//	Ghoul6e = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY + 2.0f), PosZ, PosO, false, false, 0, 0);
		Abomination1e = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosX, (PosY - 2.0f), PosZ, PosO, false, false, 0, 0);
		Abomination2e = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosX, (PosY + 4.0f), PosZ, PosO, false, false, 0, 0);
	//	Abomination3e = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosX, (PosY - 4.0f), PosZ, PosO, false, false, 0, 0);
	//	Abomination4e = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosX + 3.0f), (PosY + 3.0f), PosZ, PosO, false, false, 0, 0);
	//	Abomination5e = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosX - 3.0f), (PosY - 3.0f), PosZ, PosO, false, false, 0, 0);
	//	Abomination6e = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosX + 5.0f), (PosY + 5.0f), PosZ, PosO, false, false, 0, 0);
		Ghoul1e->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		Ghoul2e->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		Ghoul3e->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Ghoul4e->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Ghoul5e->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Ghoul6e->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		Abomination1e->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		Abomination2e->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Abomination3e->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Abomination4e->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Abomination5e->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Abomination6e->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	}
	void Wave7()// 5 trash
	{
		UnitPointer Ghoul1f = NULLUNIT;
		UnitPointer Ghoul2f = NULLUNIT;
	//	UnitPointer Ghoul3f = NULLUNIT;
	//	UnitPointer Ghoul4f = NULLUNIT;
		UnitPointer Abomination1f = NULLUNIT;
		UnitPointer Abomination2f = NULLUNIT;
		UnitPointer Abomination3f = NULLUNIT;
	//	UnitPointer Abomination4f = NULLUNIT;
		UnitPointer Necromancer1f = NULLUNIT;
	//	UnitPointer Necromancer2f = NULLUNIT;
	//	UnitPointer Necromancer3f = NULLUNIT;
	//	UnitPointer Necromancer4f = NULLUNIT;
		Ghoul1f = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, PosY, PosZ, PosO, false, false, 0, 0);
		Ghoul2f = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
	//	Ghoul3f = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
	//	Ghoul4f = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
		Abomination1f = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosX - 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
		Abomination2f = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosX, (PosY + 2.0f), PosZ, PosO, false, false, 0, 0);
		Abomination3f = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosX, (PosY - 2.0f), PosZ, PosO, false, false, 0, 0);
	//	Abomination4f = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosX, (PosY + 4.0f), PosZ, PosO, false, false, 0, 0);
		Necromancer1f = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY - 4.0f), PosZ, PosO, false, false, 0, 0);
	//	Necromancer2f = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 3.0f), (PosY + 3.0f), PosZ, PosO, false, false, 0, 0);
	//	Necromancer3f = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 3.0f), (PosY - 3.0f), PosZ, PosO, false, false, 0, 0);
	//	Necromancer4f = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 5.0f), (PosY + 5.0f), PosZ, PosO, false, false, 0, 0);
		Ghoul1f->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		Ghoul2f->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Ghoul3f->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Ghoul4f->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		Abomination1f->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		Abomination2f->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		Abomination3f->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Abomination4f->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		Necromancer1f->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Necromancer2f->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Necromancer3f->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Necromancer4f->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	}
	void Wave8()// 5 trash
	{
		UnitPointer Ghoul1g = NULLUNIT;
	//	UnitPointer Ghoul2g = NULLUNIT;
	//	UnitPointer Ghoul3g = NULLUNIT;
	//	UnitPointer Ghoul4g = NULLUNIT;
	//	UnitPointer Ghoul5g = NULLUNIT;
	//	UnitPointer Ghoul6g = NULLUNIT;
		UnitPointer Abomination1g = NULLUNIT;
		UnitPointer Abomination2g = NULLUNIT;
	//	UnitPointer Necromancer1g = NULLUNIT;
	//	UnitPointer Necromancer2g = NULLUNIT;
		UnitPointer CryptFiend1g = NULLUNIT;
		UnitPointer CryptFiend2g = NULLUNIT;
	//	UnitPointer CryptFiend3g = NULLUNIT;
	//	UnitPointer CryptFiend4g = NULLUNIT;
		CryptFiend1g = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosX, PosY, PosZ, PosO, false, false, 0, 0);
		CryptFiend2g = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX + 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
	//	CryptFiend3g = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX - 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
	//	CryptFiend4g = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX + 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
		Ghoul1g = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
	//	Ghoul2g = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY + 2.0f), PosZ, PosO, false, false, 0, 0);
	//	Ghoul3g = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY - 2.0f), PosZ, PosO, false, false, 0, 0);
	//	Ghoul4g = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY + 4.0f), PosZ, PosO, false, false, 0, 0);
	//	Ghoul5g = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY - 4.0f), PosZ, PosO, false, false, 0, 0);
	//	Ghoul6g = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 3.0f), (PosY + 3.0f), PosZ, PosO, false, false, 0, 0);
		Abomination1g = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosX - 3.0f), (PosY - 3.0f), PosZ, PosO, false, false, 0, 0);
		Abomination2g = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosX + 5.0f), (PosY + 5.0f), PosZ, PosO, false, false, 0, 0);
	//	Necromancer1g = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 7.0f), (PosY + 7.0f), PosZ, PosO, false, false, 0, 0);
	//	Necromancer2g = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 5.0f), (PosY - 5.0f), PosZ, PosO, false, false, 0, 0);
		Ghoul1g->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Ghoul2g->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Ghoul3g->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Ghoul4g->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Ghoul5g->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Ghoul6g->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		Abomination1g->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		Abomination2g->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Necromancer1g->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	Necromancer2g->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		CryptFiend1g->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		CryptFiend2g->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	CryptFiend3g->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	//	CryptFiend4g->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
	}
	void Wave1b()// 5 trash
		{
			UnitPointer Ghoul1 = NULLUNIT;
			UnitPointer Ghoul2 = NULLUNIT;
			UnitPointer Ghoul3 = NULLUNIT;
			UnitPointer Ghoul4 = NULLUNIT;
			UnitPointer Ghoul5 = NULLUNIT;
		//	UnitPointer Ghoul6 = NULLUNIT;
		//	UnitPointer Ghoul7 = NULLUNIT;
		//	UnitPointer Ghoul8 = NULLUNIT;
		//	UnitPointer Ghoul9 = NULLUNIT;
		//	UnitPointer Ghoul10 = NULLUNIT;
			Ghoul1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, PosY, PosZ, PosO, false, false, 0, 0);
			Ghoul2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
			Ghoul3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
			Ghoul4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
			Ghoul5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
		//	Ghoul6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY + 2.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul7 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY - 2.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul8 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY + 4.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul9 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY - 4.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul10 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 3.0f), (PosY + 3.0f), PosZ, PosO, false, false, 0, 0);
			Ghoul1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Ghoul2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Ghoul3->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Ghoul4->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Ghoul5->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul6->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul7->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul8->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul9->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul10->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		}
	void Wave2b() // 5 trash
		{
			UnitPointer Ghoul1 = NULLUNIT;
			UnitPointer Ghoul2 = NULLUNIT;
			UnitPointer Ghoul3 = NULLUNIT;
		//	UnitPointer Ghoul4 = NULLUNIT;
		//	UnitPointer Ghoul5 = NULLUNIT;
		//	UnitPointer Ghoul6 = NULLUNIT;
		//	UnitPointer Ghoul7 = NULLUNIT;
		//	UnitPointer Ghoul8 = NULLUNIT;
			UnitPointer Abomination1 = NULLUNIT;
			UnitPointer Abomination2 = NULLUNIT;
		//	UnitPointer Abomination3 = NULLUNIT;
		//	UnitPointer Abomination4 = NULLUNIT;
			Abomination1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosX, PosY, PosZ, PosO, false, false, 0, 0);
			Abomination2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosX + 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
		//	Abomination3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosX - 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
		//	Abomination4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosX + 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
			Ghoul1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
			Ghoul2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY + 2.0f), PosZ, PosO, false, false, 0, 0);
			Ghoul3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY - 2.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY + 4.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY - 4.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 3.0f), (PosY + 3.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul7 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 3.0f), (PosY - 3.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul8 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 5.0f), (PosY + 5.0f), PosZ, PosO, false, false, 0, 0);
			Ghoul1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Ghoul2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Ghoul3->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul4->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul5->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul6->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul7->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul8->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Abomination1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Abomination2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Abomination3->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Abomination4->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		}
	void Wave3b() //5 trash
		{
			UnitPointer Ghoul1 = NULLUNIT;
			UnitPointer Ghoul2 = NULLUNIT;
		//	UnitPointer Ghoul3 = NULLUNIT;
		//	UnitPointer Ghoul4 = NULLUNIT;
			UnitPointer CryptFiend1 = NULLUNIT;
			UnitPointer CryptFiend2 = NULLUNIT;
		//	UnitPointer CryptFiend3 = NULLUNIT;
		//	UnitPointer CryptFiend4 = NULLUNIT;
			UnitPointer Necromancer1 = NULLUNIT;
		//	UnitPointer Necromancer2 = NULLUNIT;
		//	UnitPointer Necromancer3 = NULLUNIT;
		//	UnitPointer Necromancer4 = NULLUNIT;
			Necromancer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, PosY, PosZ, PosO, false, false, 0, 0);
		//	Necromancer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
		//	Necromancer3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
		//	Necromancer4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
			CryptFiend1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX - 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
			CryptFiend2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosX, (PosY + 2.0f), PosZ, PosO, false, false, 0, 0);
		//	CryptFiend3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosX, (PosY - 2.0f), PosZ, PosO, false, false, 0, 0);
		//	CryptFiend4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosX, (PosY + 4.0f), PosZ, PosO, false, false, 0, 0);
			Ghoul1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY - 4.0f), PosZ, PosO, false, false, 0, 0);
			Ghoul2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 3.0f), (PosY + 3.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 3.0f), (PosY - 3.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 5.0f), (PosY + 5.0f), PosZ, PosO, false, false, 0, 0);
			Ghoul1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Ghoul2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul3->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul4->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			CryptFiend1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			CryptFiend2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	CryptFiend3->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	CryptFiend4->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Necromancer1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Necromancer2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Necromancer3->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Necromancer4->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		}
	void Wave4b()// 5 trash
		{
			UnitPointer CryptFiend1 = NULLUNIT;
		//	UnitPointer CryptFiend2 = NULLUNIT;
		//	UnitPointer CryptFiend3 = NULLUNIT;
		//	UnitPointer CryptFiend4 = NULLUNIT;
			UnitPointer Necromancer1 = NULLUNIT;
			UnitPointer Necromancer2 = NULLUNIT;
		//	UnitPointer Necromancer3 = NULLUNIT;
		//	UnitPointer Necromancer4 = NULLUNIT;
			UnitPointer Banshee1 = NULLUNIT;
			UnitPointer Banshee2 = NULLUNIT;
		//	UnitPointer Banshee3 = NULLUNIT;
		//	UnitPointer Banshee4 = NULLUNIT;
			Banshee1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, PosX, PosY, PosZ, PosO, false, false, 0, 0);
			Banshee2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, (PosX + 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
		//	Banshee3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, (PosX - 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
		//	Banshee4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, (PosX + 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
			Necromancer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
			Necromancer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY + 2.0f), PosZ, PosO, false, false, 0, 0);
		//	Necromancer3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY - 2.0f), PosZ, PosO, false, false, 0, 0);
		//	Necromancer4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY + 4.0f), PosZ, PosO, false, false, 0, 0);
			CryptFiend1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosX, (PosY - 4.0f), PosZ, PosO, false, false, 0, 0);
		//	CryptFiend2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX + 3.0f), (PosY + 3.0f), PosZ, PosO, false, false, 0, 0);
		//	CryptFiend3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX - 3.0f), (PosY - 3.0f), PosZ, PosO, false, false, 0, 0);
		//	CryptFiend4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX + 5.0f), (PosY + 5.0f), PosZ, PosO, false, false, 0, 0);
			CryptFiend1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	CryptFiend2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	CryptFiend3->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	CryptFiend4->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Necromancer1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Necromancer2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Necromancer3->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Necromancer4->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Banshee1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Banshee2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Banshee3->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Banshee4->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		}
	void Wave5b()// 5 trash
		{
			UnitPointer Ghoul1 = NULLUNIT;
		//  UnitPointer Ghoul2 = NULLUNIT;
		//	UnitPointer Ghoul3 = NULLUNIT;
		//	UnitPointer Ghoul4 = NULLUNIT;
		//	UnitPointer Ghoul5 = NULLUNIT;
		//	UnitPointer Ghoul6 = NULLUNIT;
			UnitPointer Necromancer1 = NULLUNIT;
			UnitPointer Necromancer2 = NULLUNIT;
			UnitPointer Banshee1 = NULLUNIT;
			UnitPointer Banshee2 = NULLUNIT;
		//	UnitPointer Banshee3 = NULLUNIT;
		//	UnitPointer Banshee4 = NULLUNIT;
			Banshee1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, PosX, PosY, PosZ, PosO, false, false, 0, 0);
			Banshee2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, (PosX + 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
		//	Banshee3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, (PosX - 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
		//	Banshee4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, (PosX + 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
			Necromancer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
			Necromancer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY + 2.0f), PosZ, PosO, false, false, 0, 0);
			Ghoul1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY - 2.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY + 4.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY - 4.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 3.0f), (PosY + 3.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 3.0f), (PosY - 3.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 5.0f), (PosY + 5.0f), PosZ, PosO, false, false, 0, 0);
			Ghoul1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul3->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul4->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul5->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul6->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Necromancer1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Necromancer2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Banshee1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Banshee2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Banshee3->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Banshee4->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		}
	void Wave6b() // 5 trash
		{
			UnitPointer Ghoul1 = NULLUNIT;
		//	UnitPointer Ghoul2 = NULLUNIT;
		//	UnitPointer Ghoul3 = NULLUNIT;
		//	UnitPointer Ghoul4 = NULLUNIT;
			UnitPointer Abomination1 = NULLUNIT;
			UnitPointer Abomination2 = NULLUNIT;
			UnitPointer Necromancer1 = NULLUNIT;
			UnitPointer Necromancer2 = NULLUNIT;
		//	UnitPointer Necromancer3 = NULLUNIT;
		//	UnitPointer Necromancer4 = NULLUNIT;
			Necromancer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, PosY, PosZ, PosO, false, false, 0, 0);
			Necromancer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
		//	Necromancer3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
		//	Necromancer4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
			Ghoul1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
		//	Ghoul2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY + 2.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY - 2.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY + 4.0f), PosZ, PosO, false, false, 0, 0);
			Abomination1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosX - 3.0f), (PosY - 3.0f), PosZ, PosO, false, false, 0, 0);
			Abomination2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosX + 5.0f), (PosY + 5.0f), PosZ, PosO, false, false, 0, 0);
			Ghoul1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul3->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul4->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Abomination1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Abomination2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Necromancer1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Necromancer2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Necromancer3->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Necromancer4->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			
		}
	void Wave7b()// 5 trash
		{
			UnitPointer Ghoul1 = NULLUNIT;
		//	UnitPointer Ghoul2 = NULLUNIT;
			UnitPointer Abomination1 = NULLUNIT;
		//	UnitPointer Abomination2 = NULLUNIT;
		//	UnitPointer Abomination3 = NULLUNIT;
		//	UnitPointer Abomination4 = NULLUNIT;
			UnitPointer CryptFiend1 = NULLUNIT;
			UnitPointer CryptFiend2 = NULLUNIT;
		//	UnitPointer CryptFiend3 = NULLUNIT;
		//	UnitPointer CryptFiend4 = NULLUNIT;
			UnitPointer Banshee1 = NULLUNIT;
		//	UnitPointer Banshee2 = NULLUNIT;
			Banshee1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, PosX, PosY, PosZ, PosO, false, false, 0, 0);
		//	Banshee2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, (PosX + 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
			CryptFiend1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX - 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
			CryptFiend2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX + 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
		//	CryptFiend3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX - 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
		//	CryptFiend4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosX, (PosY + 2.0f), PosZ, PosO, false, false, 0, 0);
			Ghoul1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY - 2.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY + 4.0f), PosZ, PosO, false, false, 0, 0);
			Abomination1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosX, (PosY - 4.0f), PosZ, PosO, false, false, 0, 0);
		//	Abomination2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosX + 3.0f), (PosY + 3.0f), PosZ, PosO, false, false, 0, 0);
		//	Abomination3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosX - 3.0f), (PosY - 3.0f), PosZ, PosO, false, false, 0, 0);
		//	Abomination4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosX + 5.0f), (PosY + 5.0f), PosZ, PosO, false, false, 0, 0);	
			Ghoul1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Abomination1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Abomination2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Abomination3->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Abomination4->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			CryptFiend1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			CryptFiend2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	CryptFiend3->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	CryptFiend4->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Banshee1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Banshee2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		}
	void Wave8b()
		{
			UnitPointer Ghoul1 = NULLUNIT;
		//	UnitPointer Ghoul2 = NULLUNIT;
		//	UnitPointer Ghoul3 = NULLUNIT;
			UnitPointer Abomination1 = NULLUNIT;
		//	UnitPointer Abomination2 = NULLUNIT;
		//	UnitPointer Abomination3 = NULLUNIT;
		//	UnitPointer Abomination4 = NULLUNIT;
			UnitPointer Necromancer1 = NULLUNIT;
		//	UnitPointer Necromancer2 = NULLUNIT;
			UnitPointer CryptFiend1 = NULLUNIT;
		//	UnitPointer CryptFiend2 = NULLUNIT;
		//	UnitPointer CryptFiend3 = NULLUNIT;
			UnitPointer Banshee1 = NULLUNIT;
		//	UnitPointer Banshee2 = NULLUNIT;
			Banshee1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, PosX, PosY, PosZ, PosO, false, false, 0, 0);
		//	Banshee2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, (PosX + 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
			CryptFiend1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX - 2.0f), PosY, PosZ, PosO, false, false, 0, 0);
		//	CryptFiend2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX + 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
		//	CryptFiend3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosX - 4.0f), PosY, PosZ, PosO, false, false, 0, 0);
			Ghoul1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY + 2.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY - 2.0f), PosZ, PosO, false, false, 0, 0);
		//	Ghoul3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosX, (PosY + 4.0f), PosZ, PosO, false, false, 0, 0);
			Abomination1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosX, (PosY - 4.0f), PosZ, PosO, false, false, 0, 0);
		//	Abomination2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosX + 3.0f), (PosY + 3.0f), PosZ, PosO, false, false, 0, 0);
		//	Abomination3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosX - 3.0f), (PosY - 3.0f), PosZ, PosO, false, false, 0, 0);
		//	Abomination4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosX + 5.0f), (PosY + 5.0f), PosZ, PosO, false, false, 0, 0);
			Necromancer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX - 5.0f), (PosY - 5.0f), PosZ, PosO, false, false, 0, 0);
		//	Necromancer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosX + 7.0f), (PosY + 7.0f), PosZ, PosO, false, false, 0, 0);
			Ghoul1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Ghoul3->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Abomination1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Abomination2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Abomination3->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Abomination4->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Necromancer1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Necromancer2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			CryptFiend1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	CryptFiend2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	CryptFiend3->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			Banshee1->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
		//	Banshee2->GetAIInterface()->MoveTo(5043.322754f, -1774.035156f, 1323.225830f, 1.995326f);
			
		}
	void Wave1c()//5 trash
	{
		UnitPointer Ghoul1 = NULLUNIT;
	//	UnitPointer Ghoul2 = NULLUNIT;
	//	UnitPointer Ghoul3 = NULLUNIT;
	//	UnitPointer Ghoul4 = NULLUNIT;
		UnitPointer Abomination1 = NULLUNIT;
	//	UnitPointer Abomination2 = NULLUNIT;
	//	UnitPointer Abomination3 = NULLUNIT;
	//	UnitPointer Abomination4 = NULLUNIT;
		UnitPointer Banshee1 = NULLUNIT;
	//	UnitPointer Banshee2 = NULLUNIT;
		UnitPointer Necromancer1 = NULLUNIT;
		UnitPointer Necromancer2 = NULLUNIT;
		Necromancer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, PosYa, PosZa, PosOa, false, false, 0, 0);
		Necromancer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa + 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		Banshee1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, (PosXa - 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Banshee2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, (PosXa + 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		Abomination1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosXa - 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Abomination2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosXa, (PosYa + 2.0f), PosZa, PosOa, false, false, 0, 0);
	//	Abomination3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosXa, (PosYa - 2.0f), PosZa, PosOa, false, false, 0, 0);
	//	Abomination4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosXa, (PosYa + 4.0f), PosZa, PosOa, false, false, 0, 0);
		Ghoul1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa - 4.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa + 3.0f), (PosYa + 3.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa - 3.0f), (PosYa - 3.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa + 5.0f), (PosYa + 5.0f), PosZa, PosOa, false, false, 0, 0);
		Ghoul1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Abomination1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Abomination2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Abomination3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Abomination4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Banshee1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Banshee2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Necromancer1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Necromancer2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	}
	void Wave2c()
	{
		UnitPointer Ghoul1 = NULLUNIT;
		UnitPointer Ghoul2 = NULLUNIT;
		UnitPointer Gargoyle1 = NULLUNIT;
		UnitPointer Gargoyle2 = NULLUNIT;
		UnitPointer Gargoyle3 = NULLUNIT;
	//	UnitPointer Gargoyle4 = NULLUNIT;
	//	UnitPointer Gargoyle5 = NULLUNIT;
	//	UnitPointer Gargoyle6 = NULLUNIT;
	//	UnitPointer Gargoyle7 = NULLUNIT;
	//	UnitPointer Gargoyle8 = NULLUNIT;
	//	UnitPointer Gargoyle9 = NULLUNIT;
	//	UnitPointer Gargoyle10 = NULLUNIT;
		Gargoyle1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, PosXa, PosYa, PosZa, PosOa, false, false, 0, 0);
		Gargoyle2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, (PosXa + 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		Gargoyle3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, (PosXa - 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Gargoyle4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, (PosXa + 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Gargoyle5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, (PosXa - 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Gargoyle6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, PosXa, (PosYa + 2.0f), PosZa, PosOa, false, false, 0, 0);
	//	Gargoyle7 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, PosXa, (PosYa - 2.0f), PosZa, PosOa, false, false, 0, 0);
	//	Gargoyle8 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, PosXa, (PosYa + 4.0f), PosZa, PosOa, false, false, 0, 0);
	//	Gargoyle9 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, PosXa, (PosYa - 4.0f), PosZa, PosOa, false, false, 0, 0);
	//	Gargoyle10 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, (PosXa + 3.0f), (PosYa + 3.0f), PosZa, PosOa, false, false, 0, 0);
		Ghoul1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa - 3.0f), (PosYa - 3.0f), PosZa, PosOa, false, false, 0, 0);
		Ghoul2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa + 5.0f), (PosYa + 5.0f), PosZa, PosOa, false, false, 0, 0);
		Ghoul1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Ghoul2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Gargoyle1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Gargoyle2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Gargoyle3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Gargoyle4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Gargoyle5->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Gargoyle6->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Gargoyle7->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Gargoyle8->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Gargoyle9->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Gargoyle10->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	}
	void Wave3c()
	{
	//	UnitPointer Ghoul1 = NULLUNIT;
	//	UnitPointer Ghoul2 = NULLUNIT;
	//	UnitPointer Ghoul3 = NULLUNIT;
	//	UnitPointer Ghoul4 = NULLUNIT;
	//	UnitPointer Ghoul5 = NULLUNIT;
	//	UnitPointer Ghoul6 = NULLUNIT;
	//	UnitPointer CryptFiend1 = NULLUNIT;
	//	UnitPointer CryptFiend2 = NULLUNIT;
	//	UnitPointer CryptFiend3 = NULLUNIT;
		UnitPointer CryptFiend4 = NULLUNIT;
		UnitPointer CryptFiend5 = NULLUNIT;
		UnitPointer CryptFiend6 = NULLUNIT;
		UnitPointer Necromancer1 = NULLUNIT;
		UnitPointer Necromancer2 = NULLUNIT;
		Necromancer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, PosYa, PosZa, PosOa, false, false, 0, 0);
		Necromancer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa + 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	CryptFiend1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosXa - 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	CryptFiend2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosXa + 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	CryptFiend3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosXa - 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		CryptFiend4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosXa, (PosYa + 2.0f), PosZa, PosOa, false, false, 0, 0);
		CryptFiend5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosXa, (PosYa - 2.0f), PosZa, PosOa, false, false, 0, 0);
		CryptFiend6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosXa, (PosYa + 4.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa - 4.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa + 3.0f), (PosYa + 3.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa - 3.0f), (PosYa - 3.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa + 5.0f), (PosYa + 5.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa - 5.0f), (PosYa - 5.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa + 7.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul5->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul6->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	CryptFiend1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	CryptFiend2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	CryptFiend3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		CryptFiend4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		CryptFiend5->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		CryptFiend6->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Necromancer1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Necromancer2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	}
	void Wave4c()
	{
		UnitPointer CryptFiend1 = NULLUNIT;
	//	UnitPointer CryptFiend2 = NULLUNIT;
	//	UnitPointer CryptFiend3 = NULLUNIT;
	//	UnitPointer CryptFiend4 = NULLUNIT;
	//	UnitPointer CryptFiend5 = NULLUNIT;
	//	UnitPointer CryptFiend6 = NULLUNIT;
	//	UnitPointer Necromancer1 = NULLUNIT;
	//	UnitPointer Necromancer2 = NULLUNIT;
		UnitPointer Gargoyle1 = NULLUNIT;
		UnitPointer Gargoyle2 = NULLUNIT;
		UnitPointer Gargoyle3 = NULLUNIT;
		UnitPointer Gargoyle4 = NULLUNIT;
	//	UnitPointer Gargoyle5 = NULLUNIT;
	//	UnitPointer Gargoyle6 = NULLUNIT;
		Gargoyle1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, PosXa, PosYa, PosZa, PosOa, false, false, 0, 0);
		Gargoyle2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, (PosXa + 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		Gargoyle3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, (PosXa - 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		Gargoyle4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, (PosXa + 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Gargoyle5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, (PosXa - 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Gargoyle6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, PosXa, (PosYa + 2.0f), PosZa, PosOa, false, false, 0, 0);
	//	Necromancer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa - 2.0f), PosZa, PosOa, false, false, 0, 0);
	//	Necromancer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa + 4.0f), PosZa, PosOa, false, false, 0, 0);
		CryptFiend1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosXa, (PosYa - 4.0f), PosZa, PosOa, false, false, 0, 0);
	//	CryptFiend2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosXa + 3.0f), (PosYa + 3.0f), PosZa, PosOa, false, false, 0, 0);
	//	CryptFiend3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosXa - 3.0f), (PosYa - 3.0f), PosZa, PosOa, false, false, 0, 0);
	//	CryptFiend4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosXa + 5.0f), (PosYa + 5.0f), PosZa, PosOa, false, false, 0, 0);
	//	CryptFiend5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosXa - 5.0f), (PosYa - 5.0f), PosZa, PosOa, false, false, 0, 0);
	//	CryptFiend6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosXa, (PosYa + 7.0f), PosZa, PosOa, false, false, 0, 0);
		CryptFiend1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	CryptFiend2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	CryptFiend3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	CryptFiend4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	CryptFiend5->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	CryptFiend6->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Necromancer1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Necromancer2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Gargoyle1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Gargoyle2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Gargoyle3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Gargoyle4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Gargoyle5->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Gargoyle6->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	}
	void Wave5c()
	{
	//	UnitPointer Ghoul1 = NULLUNIT;
	//	UnitPointer Ghoul2 = NULLUNIT;
	//	UnitPointer Ghoul3 = NULLUNIT;
	//	UnitPointer Ghoul4 = NULLUNIT;
	//	UnitPointer Necromancer1 = NULLUNIT;
	//	UnitPointer Necromancer2 = NULLUNIT;
	//	UnitPointer Necromancer3 = NULLUNIT;
	//	UnitPointer Necromancer4 = NULLUNIT;
	//	UnitPointer Abomination1 = NULLUNIT;
		UnitPointer Abomination2 = NULLUNIT;
		UnitPointer Abomination3 = NULLUNIT;
		UnitPointer Abomination4 = NULLUNIT;
		UnitPointer Abomination5 = NULLUNIT;
		UnitPointer Abomination6 = NULLUNIT;
	//	Abomination1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosXa, PosYa, PosZa, PosOa, false, false, 0, 0);
		Abomination2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosXa + 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		Abomination3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosXa - 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		Abomination4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosXa + 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		Abomination5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosXa - 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		Abomination6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosXa, (PosYa + 2.0f), PosZa, PosOa, false, false, 0, 0);
	//	Necromancer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa - 2.0f), PosZa, PosOa, false, false, 0, 0);
	//	Necromancer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa + 4.0f), PosZa, PosOa, false, false, 0, 0);
	//	Necromancer3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa - 4.0f), PosZa, PosOa, false, false, 0, 0);
	//	Necromancer4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa + 3.0f), (PosYa + 3.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa - 3.0f), (PosYa - 3.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa + 5.0f), (PosYa + 5.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa - 5.0f), (PosYa - 5.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa + 7.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Necromancer1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Necromancer2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Necromancer3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Necromancer4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Abomination1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Abomination2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Abomination3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Abomination4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Abomination5->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Abomination6->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	}
	void Wave6c()//6 trash
	{
		UnitPointer Gargoyle1 = NULLUNIT;
		UnitPointer Gargoyle2 = NULLUNIT;
		UnitPointer Gargoyle3 = NULLUNIT;
		UnitPointer Gargoyle4 = NULLUNIT;
		UnitPointer Gargoyle5 = NULLUNIT;
	//	UnitPointer Gargoyle6 = NULLUNIT;
	//	UnitPointer Gargoyle7 = NULLUNIT;
	//	UnitPointer Gargoyle8 = NULLUNIT;
		UnitPointer FrostWyrm1 = NULLUNIT;
		FrostWyrm1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FWYRM, PosXa, PosYa, PosZa, PosOa, false, false, 0, 0);
		Gargoyle1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, (PosXa + 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		Gargoyle2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, (PosXa - 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		Gargoyle3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, (PosXa + 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		Gargoyle4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, (PosXa - 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		Gargoyle5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, PosXa, (PosYa + 2.0f), PosZa, PosOa, false, false, 0, 0);
	//	Gargoyle6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, PosXa, (PosYa - 2.0f), PosZa, PosOa, false, false, 0, 0);
	//	Gargoyle7 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, PosXa, (PosYa + 4.0f), PosZa, PosOa, false, false, 0, 0);
	//	Gargoyle8 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, PosXa, (PosYa - 4.0f), PosZa, PosOa, false, false, 0, 0);
		Gargoyle1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Gargoyle2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Gargoyle3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Gargoyle4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Gargoyle5->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Gargoyle6->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Gargoyle7->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Gargoyle8->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		FrostWyrm1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	}
	void Wave7c()
	{
		UnitPointer Abomination1 = NULLUNIT;
	//	UnitPointer Abomination2 = NULLUNIT;
	//	UnitPointer Abomination3 = NULLUNIT;
	//	UnitPointer Abomination4 = NULLUNIT;
		UnitPointer FrostWyrm1 = NULLUNIT;
		UnitPointer Ghoul1 = NULLUNIT;
		UnitPointer Ghoul2 = NULLUNIT;
		UnitPointer Ghoul3 = NULLUNIT;
	//	UnitPointer Ghoul4 = NULLUNIT;
	//	UnitPointer Ghoul5 = NULLUNIT;
	//	UnitPointer Ghoul6 = NULLUNIT;
		Ghoul1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, PosYa, PosZa, PosOa, false, false, 0, 0);
		Ghoul2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa + 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		Ghoul3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa - 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Ghoul4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa + 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Ghoul5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa - 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Ghoul6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa + 2.0f), PosZa, PosOa, false, false, 0, 0);
		FrostWyrm1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FWYRM, PosXa, (PosYa - 2.0f), PosZa, PosOa, false, false, 0, 0);
		Abomination1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosXa, (PosYa + 4.0f), PosZa, PosOa, false, false, 0, 0);
	//	Abomination2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosXa, (PosYa - 4.0f), PosZa, PosOa, false, false, 0, 0);
	//	Abomination3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosXa + 3.0f), (PosYa + 3.0f), PosZa, PosOa, false, false, 0, 0);
	//	Abomination4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosXa - 3.0f), (PosYa - 3.0f), PosZa, PosOa, false, false, 0, 0);
		Abomination1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Abomination2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Abomination3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Abomination4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		FrostWyrm1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Ghoul1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Ghoul2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Ghoul3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul5->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul6->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	}
	void Wave8c()
	{
		UnitPointer Abomination1 = NULLUNIT;
		UnitPointer Abomination2 = NULLUNIT;
	//	UnitPointer Abomination3 = NULLUNIT;
	//	UnitPointer Abomination4 = NULLUNIT;
		UnitPointer Necromancer1 = NULLUNIT;
	//	UnitPointer Necromancer2 = NULLUNIT;
	//	UnitPointer CryptFiend1 = NULLUNIT;
	//	UnitPointer CryptFiend2 = NULLUNIT;
	//	UnitPointer Banshee1 = NULLUNIT;
	//	UnitPointer Banshee2 = NULLUNIT;
		UnitPointer Ghoul1 = NULLUNIT;
		UnitPointer Ghoul2 = NULLUNIT;
	//	UnitPointer Ghoul3 = NULLUNIT;
	//	UnitPointer Ghoul4 = NULLUNIT;
	//	UnitPointer Ghoul5 = NULLUNIT;
	//	UnitPointer Ghoul6 = NULLUNIT;
		Ghoul1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, PosYa, PosZa, PosOa, false, false, 0, 0);
		Ghoul2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa + 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Ghoul3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa - 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Ghoul4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa + 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Ghoul5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa - 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Ghoul6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa + 2.0f), PosZa, PosOa, false, false, 0, 0);
	//	Banshee1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, PosXa, (PosYa - 2.0f), PosZa, PosOa, false, false, 0, 0);
	//	Banshee2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, PosXa, (PosYa + 4.0f), PosZa, PosOa, false, false, 0, 0);
	//	CryptFiend1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosXa, (PosYa - 4.0f), PosZa, PosOa, false, false, 0, 0);
	//	CryptFiend2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosXa + 3.0f), (PosYa + 3.0f), PosZa, PosOa, false, false, 0, 0);
		Abomination1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosXa - 3.0f), (PosYa - 3.0f), PosZa, PosOa, false, false, 0, 0);
		Abomination2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosXa + 5.0f), (PosYa + 5.0f), PosZa, PosOa, false, false, 0, 0);
	//	Abomination3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosXa - 5.0f), (PosYa - 5.0f), PosZa, PosOa, false, false, 0, 0);
	//	Abomination4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosXa, (PosYa + 7.0f), PosZa, PosOa, false, false, 0, 0);
		Necromancer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa - 7.0f), PosZa, PosOa, false, false, 0, 0);
	//	Necromancer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa + 9.0f), PosZa, PosOa, false, false, 0, 0);
		Abomination1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Abomination2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Abomination3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Abomination4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Necromancer1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Necromancer2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	CryptFiend1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	CryptFiend2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Banshee1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Banshee2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Ghoul1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Ghoul2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul5->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul6->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	}
	void Wave1d()
	{
		UnitPointer Necromancer1 = NULLUNIT;
		UnitPointer Necromancer2 = NULLUNIT;
		UnitPointer Necromancer3 = NULLUNIT;
	//	UnitPointer Necromancer4 = NULLUNIT;
	//	UnitPointer Necromancer5 = NULLUNIT;
	//	UnitPointer Necromancer6 = NULLUNIT;
		UnitPointer Abomination1 = NULLUNIT;
		UnitPointer Abomination2 = NULLUNIT;
		UnitPointer Abomination3 = NULLUNIT;
	//	UnitPointer Abomination4 = NULLUNIT;
	//	UnitPointer Abomination5 = NULLUNIT;
	//	UnitPointer Abomination6 = NULLUNIT;
		Abomination1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosXa, PosYa, PosZa, PosOa, false, false, 0, 0);
		Abomination2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosXa + 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		Abomination3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosXa - 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Abomination4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosXa + 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Abomination5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosXa - 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Abomination6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosXa, (PosYa + 2.0f), PosZa, PosOa, false, false, 0, 0);
		Necromancer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa - 2.0f), PosZa, PosOa, false, false, 0, 0);
		Necromancer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa + 4.0f), PosZa, PosOa, false, false, 0, 0);
		Necromancer3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa - 4.0f), PosZa, PosOa, false, false, 0, 0);
	//	Necromancer4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa + 3.0f), (PosYa + 3.0f), PosZa, PosOa, false, false, 0, 0);
	//	Necromancer5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa - 3.0f), (PosYa - 3.0f), PosZa, PosOa, false, false, 0, 0);
	//	Necromancer6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa + 5.0f), (PosYa + 5.0f), PosZa, PosOa, false, false, 0, 0);
		Necromancer1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Necromancer2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Necromancer3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Necromancer4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Necromancer5->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Necromancer6->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Abomination1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Abomination2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Abomination3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Abomination4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Abomination5->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Abomination6->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	}
	void Wave2d()
	{
		UnitPointer Ghoul1 = NULLUNIT;
		UnitPointer Ghoul2 = NULLUNIT;
	//	UnitPointer Ghoul3 = NULLUNIT;
	//	UnitPointer Ghoul4 = NULLUNIT;
	//	UnitPointer Ghoul5 = NULLUNIT;
		UnitPointer Gargoyle1 = NULLUNIT;
		UnitPointer Gargoyle2 = NULLUNIT;
		UnitPointer Gargoyle3 = NULLUNIT;
	//	UnitPointer Gargoyle4 = NULLUNIT;
	//	UnitPointer Gargoyle5 = NULLUNIT;
	//	UnitPointer Gargoyle6 = NULLUNIT;
	//	UnitPointer Gargoyle7 = NULLUNIT;
	//	UnitPointer Gargoyle8 = NULLUNIT;
		UnitPointer FrostWyrm1 = NULLUNIT;
		FrostWyrm1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FWYRM, PosXa, PosYa, PosZa, PosOa, false, false, 0, 0);
		Gargoyle1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, (PosXa + 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		Gargoyle2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, (PosXa - 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		Gargoyle3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, (PosXa + 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Gargoyle4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, (PosXa - 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Gargoyle5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, PosXa, (PosYa + 2.0f), PosZa, PosOa, false, false, 0, 0);
	//	Gargoyle6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, PosXa, (PosYa - 2.0f), PosZa, PosOa, false, false, 0, 0);
	//	Gargoyle7 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, PosXa, (PosYa + 4.0f), PosZa, PosOa, false, false, 0, 0);
	//	Gargoyle8 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GARGO, PosXa, (PosYa - 4.0f), PosZa, PosOa, false, false, 0, 0);
		Ghoul1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa + 3.0f), (PosYa + 3.0f), PosZa, PosOa, false, false, 0, 0);
		Ghoul2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa - 3.0f), (PosYa - 3.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa - 3.0f), (PosYa - 3.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa + 5.0f), (PosYa + 5.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa - 5.0f), (PosYa - 5.0f), PosZa, PosOa, false, false, 0, 0);
		Ghoul1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Ghoul2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul5->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Gargoyle1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Gargoyle2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Gargoyle3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Gargoyle4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//  Gargoyle5->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Gargoyle6->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Gargoyle7->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Gargoyle8->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		FrostWyrm1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	}
	void Wave3d()
	{
	//	UnitPointer Ghoul1 = NULLUNIT;
	//	UnitPointer Ghoul2 = NULLUNIT;
	//	UnitPointer Ghoul3 = NULLUNIT;
	//	UnitPointer Ghoul4 = NULLUNIT;
	//	UnitPointer Ghoul5 = NULLUNIT;
	//	UnitPointer Ghoul6 = NULLUNIT;
	//	UnitPointer GiantInfernal1 = NULLUNIT;
	//	UnitPointer GiantInfernal2 = NULLUNIT;
		UnitPointer GiantInfernal3 = NULLUNIT;
		UnitPointer GiantInfernal4 = NULLUNIT;
		UnitPointer GiantInfernal5 = NULLUNIT;
		UnitPointer GiantInfernal6 = NULLUNIT;
		UnitPointer GiantInfernal7 = NULLUNIT;
		UnitPointer GiantInfernal8 = NULLUNIT;
	//	GiantInfernal1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, PosXa, PosYa, PosZa, PosOa, false, false, 0, 0);
	//	GiantInfernal2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, (PosXa + 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		GiantInfernal3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, (PosXa - 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		GiantInfernal4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, (PosXa + 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		GiantInfernal5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, (PosXa - 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		GiantInfernal6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, PosXa, (PosYa + 2.0f), PosZa, PosOa, false, false, 0, 0);
		GiantInfernal7 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, PosXa, (PosYa - 2.0f), PosZa, PosOa, false, false, 0, 0);
		GiantInfernal8 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, PosXa, (PosYa + 4.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa - 4.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa + 3.0f), (PosYa + 3.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa - 3.0f), (PosYa - 3.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa + 5.0f), (PosYa + 5.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa - 5.0f), (PosYa - 5.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa + 7.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	///	Ghoul2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	///	Ghoul4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul5->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul6->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	GiantInfernal1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	GiantInfernal2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		GiantInfernal3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		GiantInfernal4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		GiantInfernal5->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		GiantInfernal6->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		GiantInfernal7->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		GiantInfernal8->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	}
	void Wave4d()
	{
		UnitPointer FelStalker1 = NULLUNIT;
		UnitPointer FelStalker2 = NULLUNIT;
		UnitPointer FelStalker3 = NULLUNIT;
	/*	UnitPointer FelStalker4 = NULLUNIT;
		UnitPointer FelStalker5 = NULLUNIT;
		UnitPointer FelStalker6 = NULLUNIT;*/
		UnitPointer GiantInfernal1 = NULLUNIT;
		UnitPointer GiantInfernal2 = NULLUNIT;
	/*	UnitPointer GiantInfernal3 = NULLUNIT;
		UnitPointer GiantInfernal4 = NULLUNIT;
		UnitPointer GiantInfernal5 = NULLUNIT;
		UnitPointer GiantInfernal6 = NULLUNIT;
		UnitPointer GiantInfernal7 = NULLUNIT;
		UnitPointer GiantInfernal8 = NULLUNIT;*/ 
		GiantInfernal1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, PosXa, PosYa, PosZa, PosOa, false, false, 0, 0);
		GiantInfernal2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, (PosXa + 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	/*	GiantInfernal3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, (PosXa - 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		GiantInfernal4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, (PosXa + 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		GiantInfernal5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, (PosXa - 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		GiantInfernal6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, PosXa, (PosYa + 2.0f), PosZa, PosOa, false, false, 0, 0);
		GiantInfernal7 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, PosXa, (PosYa - 2.0f), PosZa, PosOa, false, false, 0, 0);
		GiantInfernal8 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, PosXa, (PosYa + 4.0f), PosZa, PosOa, false, false, 0, 0);*/
		FelStalker1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FSTAL, PosXa, (PosYa - 4.0f), PosZa, PosOa, false, false, 0, 0);
		FelStalker2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FSTAL, (PosXa + 3.0f), (PosYa + 3.0f), PosZa, PosOa, false, false, 0, 0);
		FelStalker3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FSTAL, (PosXa - 3.0f), (PosYa - 3.0f), PosZa, PosOa, false, false, 0, 0);
	//	FelStalker4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FSTAL, (PosXa + 5.0f), (PosYa + 5.0f), PosZa, PosOa, false, false, 0, 0);
	//	FelStalker5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FSTAL, (PosXa - 5.0f), (PosYa - 5.0f), PosZa, PosOa, false, false, 0, 0);
	//	FelStalker6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FSTAL, PosXa, (PosYa + 7.0f), PosZa, PosOa, false, false, 0, 0);
		FelStalker1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		FelStalker2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		FelStalker3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	FelStalker4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	FelStalker5->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	FelStalker6->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		GiantInfernal1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		GiantInfernal2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	GiantInfernal3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	GiantInfernal4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	GiantInfernal5->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	GiantInfernal6->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	GiantInfernal7->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	GiantInfernal8->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	}
	void Wave5d()
	{
		UnitPointer Necromancer1 = NULLUNIT;
	//	UnitPointer Necromancer2 = NULLUNIT;
	//	UnitPointer Necromancer3 = NULLUNIT;
	//	UnitPointer Necromancer4 = NULLUNIT;
		UnitPointer Abomination1 = NULLUNIT;
		UnitPointer Abomination2 = NULLUNIT;
	//	UnitPointer Abomination3 = NULLUNIT;
	//	UnitPointer Abomination4 = NULLUNIT;
		UnitPointer FelStalker1 = NULLUNIT;
		UnitPointer FelStalker2 = NULLUNIT;
	//	UnitPointer FelStalker3 = NULLUNIT;
	//	UnitPointer FelStalker4 = NULLUNIT;
	//	UnitPointer FelStalker5 = NULLUNIT;
	//	UnitPointer FelStalker6 = NULLUNIT;
		FelStalker1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FSTAL, PosXa, PosYa, PosZa, PosOa, false, false, 0, 0);
		FelStalker2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FSTAL, (PosXa + 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	FelStalker3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FSTAL, (PosXa - 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	FelStalker4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FSTAL, (PosXa + 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	FelStalker5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FSTAL, (PosXa - 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	FelStalker6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FSTAL, PosXa, (PosYa + 2.0f), PosZa, PosOa, false, false, 0, 0);
		Abomination1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosXa, (PosYa - 2.0f), PosZa, PosOa, false, false, 0, 0);
		Abomination2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosXa, (PosYa + 4.0f), PosZa, PosOa, false, false, 0, 0);
	//	Abomination3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosXa, (PosYa - 4.0f), PosZa, PosOa, false, false, 0, 0);
	//	Abomination4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosXa + 3.0f), (PosYa + 3.0f), PosZa, PosOa, false, false, 0, 0);
		Necromancer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa - 3.0f), (PosYa - 3.0f), PosZa, PosOa, false, false, 0, 0);
	//	Necromancer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa + 5.0f), (PosYa + 5.0f), PosZa, PosOa, false, false, 0, 0);
	//	Necromancer3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa - 5.0f), (PosYa - 5.0f), PosZa, PosOa, false, false, 0, 0);
	//	Necromancer4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa + 7.0f), PosZa, PosOa, false, false, 0, 0);
		Necromancer1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Necromancer2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Necromancer3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Necromancer4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Abomination1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Abomination2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Abomination3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Abomination4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		FelStalker1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		FelStalker2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	FelStalker3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	FelStalker4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	FelStalker5->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	FelStalker6->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	}
	void Wave6d()
	{
		UnitPointer Necromancer1 = NULLUNIT;
		UnitPointer Necromancer2 = NULLUNIT;
		UnitPointer Necromancer3 = NULLUNIT;
	//	UnitPointer Necromancer4 = NULLUNIT;
	//	UnitPointer Necromancer5 = NULLUNIT;
	//	UnitPointer Necromancer6 = NULLUNIT;
		UnitPointer Banshee1 = NULLUNIT;
		UnitPointer Banshee2 = NULLUNIT;
		UnitPointer Banshee3 = NULLUNIT;
	//	UnitPointer Banshee4 = NULLUNIT;
	//	UnitPointer Banshee5 = NULLUNIT;
	//	UnitPointer Banshee6 = NULLUNIT;
		Banshee1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, PosXa, PosYa, PosZa, PosOa, false, false, 0, 0);
		Banshee2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, (PosXa + 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		Banshee3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, (PosXa - 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Banshee4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, (PosXa + 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Banshee5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, (PosXa - 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Banshee6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, PosXa, (PosYa + 2.0f), PosZa, PosOa, false, false, 0, 0);
		Necromancer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa - 2.0f), PosZa, PosOa, false, false, 0, 0);
		Necromancer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa + 4.0f), PosZa, PosOa, false, false, 0, 0);
		Necromancer3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa - 4.0f), PosZa, PosOa, false, false, 0, 0);
	//	Necromancer4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa + 3.0f), (PosYa + 3.0f), PosZa, PosOa, false, false, 0, 0);
	//	Necromancer5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa - 3.0f), (PosYa - 3.0f), PosZa, PosOa, false, false, 0, 0);
	//	Necromancer6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa + 5.0f), (PosYa + 5.0f), PosZa, PosOa, false, false, 0, 0);
		Necromancer1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Necromancer2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Necromancer3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Necromancer4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Necromancer5->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Necromancer6->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Banshee1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Banshee2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Banshee3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Banshee4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Banshee5->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Banshee6->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	}
	void Wave7d()
	{
	//	UnitPointer Ghoul1 = NULLUNIT;
	//	UnitPointer Ghoul2 = NULLUNIT;
		UnitPointer FelStalker1 = NULLUNIT;
		UnitPointer FelStalker2 = NULLUNIT;
		UnitPointer CryptFiend1 = NULLUNIT;
		UnitPointer CryptFiend2 = NULLUNIT;
		UnitPointer GiantInfernal1 = NULLUNIT;
		UnitPointer GiantInfernal2 = NULLUNIT;
	//	UnitPointer GiantInfernal3 = NULLUNIT;
	//	UnitPointer GiantInfernal4 = NULLUNIT;
	//	UnitPointer GiantInfernal5 = NULLUNIT;
	//	UnitPointer GiantInfernal6 = NULLUNIT;
	//	UnitPointer GiantInfernal7 = NULLUNIT;
	//	UnitPointer GiantInfernal8 = NULLUNIT;
		GiantInfernal1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, PosXa, PosYa, PosZa, PosOa, false, false, 0, 0);
		GiantInfernal2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, (PosXa + 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	GiantInfernal3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, (PosXa - 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	GiantInfernal4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, (PosXa + 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	GiantInfernal5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, (PosXa - 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	GiantInfernal6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, PosXa, (PosYa + 2.0f), PosZa, PosOa, false, false, 0, 0);
	//	GiantInfernal7 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, PosXa, (PosYa - 2.0f), PosZa, PosOa, false, false, 0, 0);
	//	GiantInfernal8 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GINFE, PosXa, (PosYa + 4.0f), PosZa, PosOa, false, false, 0, 0);
		CryptFiend1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosXa, (PosYa - 4.0f), PosZa, PosOa, false, false, 0, 0);
		CryptFiend2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosXa + 3.0f), (PosYa + 3.0f), PosZa, PosOa, false, false, 0, 0);
		FelStalker1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FSTAL, (PosXa - 3.0f), (PosYa - 3.0f), PosZa, PosOa, false, false, 0, 0);
		FelStalker2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FSTAL, (PosXa + 5.0f), (PosYa + 5.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, (PosXa - 5.0f), (PosYa - 5.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa + 7.0f), PosZa, PosOa, false, false, 0, 0);
	//	Ghoul1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Ghoul2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		FelStalker1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		FelStalker2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		CryptFiend1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		CryptFiend2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		GiantInfernal1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		GiantInfernal2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	GiantInfernal3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	GiantInfernal4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	GiantInfernal5->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	GiantInfernal6->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	GiantInfernal7->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	GiantInfernal8->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	}
	void Wave8d()
	{
	//	UnitPointer Abomination1 = NULLUNIT;
	//	UnitPointer Abomination2 = NULLUNIT;
	//	UnitPointer Abomination3 = NULLUNIT;
	//	UnitPointer Abomination4 = NULLUNIT;
		UnitPointer Necromancer1 = NULLUNIT;
		UnitPointer Necromancer2 = NULLUNIT;
		UnitPointer CryptFiend1 = NULLUNIT;
		UnitPointer CryptFiend2 = NULLUNIT;
	//	UnitPointer CryptFiend3 = NULLUNIT;
	//	UnitPointer CryptFiend4 = NULLUNIT;
	//	UnitPointer Banshee1 = NULLUNIT;
	//	UnitPointer Banshee2 = NULLUNIT;
		UnitPointer Banshee3 = NULLUNIT;
		UnitPointer Banshee4 = NULLUNIT;
		UnitPointer FelStalker1 = NULLUNIT;
		UnitPointer FelStalker2 = NULLUNIT;
		FelStalker1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FSTAL, PosXa, PosYa, PosZa, PosOa, false, false, 0, 0);
		FelStalker2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FSTAL, (PosXa + 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Banshee1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, (PosXa - 2.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
	//	Banshee2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, (PosXa + 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		Banshee3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, (PosXa - 4.0f), PosYa, PosZa, PosOa, false, false, 0, 0);
		Banshee4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BANSH, PosXa, (PosYa + 2.0f), PosZa, PosOa, false, false, 0, 0);
		CryptFiend1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosXa, (PosYa - 2.0f), PosZa, PosOa, false, false, 0, 0);
		CryptFiend2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosXa, (PosYa + 4.0f), PosZa, PosOa, false, false, 0, 0);
	//	CryptFiend3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, PosXa, (PosYa - 4.0f), PosZa, PosOa, false, false, 0, 0);
	//	CryptFiend4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CRYPT, (PosXa + 3.0f), (PosYa + 3.0f), PosZa, PosOa, false, false, 0, 0);
	//	Abomination1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosXa - 3.0f), (PosYa - 3.0f), PosZa, PosOa, false, false, 0, 0);
	//	Abomination2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosXa + 5.0f), (PosYa + 5.0f), PosZa, PosOa, false, false, 0, 0);
	//	Abomination3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, (PosXa - 5.0f), (PosYa - 5.0f), PosZa, PosOa, false, false, 0, 0);
	//	Abomination4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ABOMINATION, PosXa, (PosYa + 7.0f), PosZa, PosOa, false, false, 0, 0);
		Necromancer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa - 7.0f), PosZa, PosOa, false, false, 0, 0);
		Necromancer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GHOUL, PosXa, (PosYa + 9.0f), PosZa, PosOa, false, false, 0, 0);
	//	Abomination1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Abomination2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Abomination3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Abomination4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Necromancer1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Necromancer2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		CryptFiend1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		CryptFiend2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	CryptFiend3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	CryptFiend4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Banshee1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	//	Banshee2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Banshee3->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		Banshee4->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		FelStalker1->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
		FelStalker2->GetAIInterface()->MoveTo(5510.481500f, -2676.711200f, 1480.431400f, 4.662660f);
	}
	void Incoming()
	{
		Jaina->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Stay alert! Another wave approaches.");
		Jaina->PlaySoundToSet(11008);
	}
	void Rally()
	{
		uint32 RandomSpeech = rand()%2;
		switch(RandomSpeech)
		{			
			case 0:
				Jaina->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Don't give up! We must prevail!");
				Jaina->PlaySoundToSet(11006);
				break;
			case 1:
				Jaina->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "We must hold strong!");
				Jaina->PlaySoundToSet(11051);
				break;

		}
	}
	void Incoming2()
	{
		Thrall->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Make ready for another wave! LOK-TAR OGAR!");
		Thrall->PlaySoundToSet(11032);
	}
	void Rally2()
	{
		uint32 RandomSpeech = rand()%3;
		switch(RandomSpeech)
		{			
			case 0:
				Thrall->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Hold them back! Do not falter!");
				Thrall->PlaySoundToSet(11030);
				break;
			case 1:
				Thrall->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Victory or death!");
				Thrall->PlaySoundToSet(11059);
				break;
			case 2:
				Thrall->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Do not give an inch of ground!");
				Thrall->PlaySoundToSet(11060);
				break;

		}
	}
	void Success()
	{
		switch(HyjalPhase[_unit->GetInstanceID()])
		{
			case HYJAL_PHASE_RAGE_WINTERCHILL_COMPLETE:
				if(Jaina && Jaina->isAlive())
				{
				Jaina->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Hold them back as long as possible");
				Jaina->PlaySoundToSet(11050);
				}
				break;
			case HYJAL_PHASE_KAZROGAL_COMPLETE:
				if(Thrall && Thrall->isAlive())
				{
				Thrall->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Do not give an inch of ground!");
				Thrall->PlaySoundToSet(11060);
				}
				break;
		}
	}
	void Reset()
	{		
		_unit->Despawn(1000, 0);
	}
protected:
	uint32 EventTimer;
	UnitPointer Jaina;
	UnitPointer Thrall;
};
//Jaina Proudmoore AI & GS
#define CN_JAINA_PROUDMOORE 17772
#define BLIZZARD			31266
#define PYROBLAST			31263
#define ELEMENTALS			31264

class JainaProudmooreAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(JainaProudmooreAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];
	
    JainaProudmooreAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		// Event related
		if(!Started)
		{
			HyjalPhase[_unit->GetInstanceID()] = HYJAL_PHASE_NOT_STARTED;
		}
		//Spells related
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		}
		spells[0].info = dbcSpell.LookupEntry(BLIZZARD);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(PYROBLAST);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 13.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(ELEMENTALS);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].perctrigger = 10.0f;
		spells[2].attackstoptimer = 1000;

	}
	void OnDamageTaken(UnitPointer mAttacker, float fAmount) 
	{
		if (!OnDamageTimer)
		{
			uint32 RandomSpeech = rand()%2;
			switch(RandomSpeech)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'm in jeopardy, help me if you can!");
				_unit->PlaySoundToSet(11007);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "They've broken through!");
				_unit->PlaySoundToSet(11049);
				break;
			}
			OnDamageTimer = 17;
		}
	}
	void OnCombatStart(UnitPointer mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		TimerSummon = 30;
		OnDamageTimer = 2;
	}

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I did... my best.");
		_unit->PlaySoundToSet(11010);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		TimerSummon--;
		OnDamageTimer--;
		if(!TimerSummon)
		{
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(18001, (_unit->GetPositionX() + 3.0f), _unit->GetPositionY(), _unit->GetPositionZ(), 0, false, false, 0, 0);
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(18001, (_unit->GetPositionX() - 3.0f), _unit->GetPositionY(), _unit->GetPositionZ(), 0, false, false, 0, 0);
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(18001, _unit->GetPositionX(), (_unit->GetPositionY() - 3.0f), _unit->GetPositionZ(), 0, false, false, 0, 0);
			TimerSummon = 250;
		}
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

protected:
	uint32 TimerSummon;
	uint32 OnDamageTimer;
	int nrspells;
};

class SCRIPT_DECL JainaProudmooreGS : public GossipScript
{
public:
	void GossipHello(ObjectPointer pObject, PlayerPointer Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

		switch(HyjalPhase[pObject->GetInstanceID()])
		{
		case HYJAL_PHASE_NOT_STARTED:
			Menu->AddItem(0, "We are ready to defend the Alliance base.", 1);
			break;

		case HYJAL_PHASE_RAGE_WINTERCHILL_COMPLETE:
			Menu->AddItem(0, "The defenses are holding up: we can continue.", 1); 
			break;

		case HYJAL_PHASE_ANETHERON_COMPLETE:
			Menu->AddItem(0, "We can't keep this up. Let's retreat!", 1);
			break;
		}

		if(AutoSend)
			Menu->SendTo(Plr);
	}

	void GossipSelectOption(ObjectPointer pObject, PlayerPointer Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		CreaturePointer creature = TO_CREATURE(pObject);

		switch(HyjalPhase[pObject->GetInstanceID()])
		{
		case HYJAL_PHASE_NOT_STARTED:
			creature->GetMapMgr()->GetInterface()->SpawnCreature(CN_TRIGGER_HYJALA, 5073.207031f, -1790.292969f, 1320.881470f, 0.0f, false, false, 0, 0);
			creature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Hold them back as long as possible");
			creature->PlaySoundToSet(11050);
			Started = true;
			creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			break;
		case HYJAL_PHASE_RAGE_WINTERCHILL_COMPLETE:
			creature->GetMapMgr()->GetInterface()->SpawnCreature(CN_TRIGGER_HYJALA, 5073.207031f, -1790.292969f, 1320.881470f, 0.0f, false, false, 0, 0);
			creature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "We have won valuable time. Now we must pull back!");
			creature->PlaySoundToSet(11011);
			Started = false;
			creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			break;
		case HYJAL_PHASE_ANETHERON_COMPLETE:
			creature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "We are lost. Fall back!");
			creature->PlaySoundToSet(11009);
			creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			break;
		}

		Plr->Gossip_Complete();
	}

	void GossipEnd(ObjectPointer pObject, PlayerPointer Plr) { GossipScript::GossipEnd(pObject, Plr); }
	void Destroy() { delete this; }
};

//Thrall AI & GS
#define CN_THRALL 17852
#define CHAINLIGHT	31330
#define SPELLDW		31331

class ThrallAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ThrallAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];
	
    ThrallAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		//Spells related
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		}
		spells[0].info = dbcSpell.LookupEntry(CHAINLIGHT);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;

		spells[0].info = dbcSpell.LookupEntry(SPELLDW);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;
	
	}
	void OnDamageTaken(UnitPointer mAttacker, float fAmount) 
	{
		if (!OnDamageTimer)
		{
			uint32 RandomSpeech = rand()%2;
			switch(RandomSpeech)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I will lie down for no one!");
				_unit->PlaySoundToSet(11031);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Bring the fight to me and pay with your lives!");
				_unit->PlaySoundToSet(11061);
				break;
			}
			OnDamageTimer = 17;
		}
	}
	void OnCombatStart(UnitPointer mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		TimerSummon = 30;
		OnDamageTimer = 2;
	}

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Uraaa...");
		_unit->PlaySoundToSet(11034);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		/*TimerSummon--;
		if(!TimerSummon)
		{
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(17854, (_unit->GetPositionX() + 3.0f), _unit->GetPositionY(), _unit->GetPositionZ(), 0, false, false, 0, 0);
			TimerSummon = 250;
		}*/
		OnDamageTimer--;
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

protected:
	uint32 TimerSummon;
	uint32 OnDamageTimer;
	int nrspells;
};
class SCRIPT_DECL ThrallGS : public GossipScript
{
public:
	void GossipHello(ObjectPointer pObject, PlayerPointer Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

		switch(HyjalPhase[pObject->GetInstanceID()])
		{
		case HYJAL_PHASE_ANETHERON_COMPLETE:
			Menu->AddItem(0, "We're here to help! The Alliance are overrun.", 1); 
			break;

		case HYJAL_PHASE_KAZROGAL_COMPLETE:
			Menu->AddItem(0, "We're okay so far. Let's do this!", 1); 
			break;
		case HYJAL_PHASE_AZGALOR_COMPLETE:
			Menu->AddItem(0, "Azgalor is death! Archimonde is coming!", 1);
			break;
		}

		if(AutoSend)
			Menu->SendTo(Plr);
	}

	void GossipSelectOption(ObjectPointer pObject, PlayerPointer Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		CreaturePointer creature = TO_CREATURE(pObject);

		switch(HyjalPhase[pObject->GetInstanceID()])
		{
			case HYJAL_PHASE_ANETHERON_COMPLETE:
				creature->GetMapMgr()->GetInterface()->SpawnCreature(CN_TRIGGER_HYJALA, 5501.569336f, -2675.938232f, 1481.075317f, 0.0f, false, false, 0, 0);
				creature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Do not give an inch of ground!");
				creature->PlaySoundToSet(11060);
				creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				break;
			case HYJAL_PHASE_KAZROGAL_COMPLETE:
				creature->GetMapMgr()->GetInterface()->SpawnCreature(CN_TRIGGER_HYJALA, 5501.569336f, -2675.938232f, 1481.075317f, 0.0f, false, false, 0, 0);
				creature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "We have played our part and done well. It is up to the others now.");
				creature->PlaySoundToSet(11035);
				creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				break;
			case HYJAL_PHASE_AZGALOR_COMPLETE:
				creature->GetMapMgr()->GetInterface()->SpawnCreature(CN_TRIGGER_HYJALA, 5501.569336f, -2675.938232f, 1481.075317f, 0.0f, false, false, 0, 0);
				creature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "It is over. Withdraw! We have failed.");
				creature->PlaySoundToSet(11033);
				creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				break;
		}

		Plr->Gossip_Complete();
	}

	void GossipEnd(ObjectPointer pObject, PlayerPointer Plr) { GossipScript::GossipEnd(pObject, Plr); }
	void Destroy() { delete this; }
};

// Rage WinterchillAI
#define CN_RAGE_WINTERCHILL 17767

#define FROSTBOLT 31249			// it's not correct spell for sure (not sure to others too :P)
#define DEATCH_AND_DECAY 39658
#define FROST_NOVA 31250
#define FROST_ARMOR 31256

class RageWinterchillAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(RageWinterchillAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    RageWinterchillAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		}

		spells[0].info = dbcSpell.LookupEntry(FROSTBOLT);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 80.0f;

		spells[1].info = dbcSpell.LookupEntry(DEATCH_AND_DECAY);
		spells[1].targettype = TARGET_RANDOM_DESTINATION;
		spells[1].instant = false;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 30;
		spells[1].mindist2cast = 0.0f;
		spells[1].maxdist2cast = 30.0f;

		spells[2].info = dbcSpell.LookupEntry(FROST_NOVA);
		spells[2].targettype = TARGET_VARIOUS; 
		spells[2].instant = true;
		spells[2].perctrigger = 8.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].cooldown = 15;
		spells[2].mindist2cast = 0.0f;
		spells[2].maxdist2cast = 45.0f;

		spells[3].info = dbcSpell.LookupEntry(FROST_ARMOR);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].perctrigger = 8.0f;
		spells[3].attackstoptimer = 1000;
		spells[3].cooldown = 10;
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The Legion's final conquest has begun! Once again the subjugation of this world is within our grasp. Let none survive!");
		_unit->PlaySoundToSet(11022);

		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
		if (_unit->GetHealthPct() > 0)	
		{
			int RandomSpeach;
			RandomUInt(1000);
			RandomSpeach=rand()%5;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Crumble and rot!");
				_unit->PlaySoundToSet(11023);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ashes to ashes, dust to dust");
				_unit->PlaySoundToSet(11055);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "All life must perish!");
				_unit->PlaySoundToSet(11025);
				break;
			case 3:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your world is ours now!");
				_unit->PlaySoundToSet(11056);
				break;
			case 4:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Victory to the Legion!");
				_unit->PlaySoundToSet(11057);
				break;
			}
		}
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You have won this battle, but not... the...war");
		_unit->PlaySoundToSet(11026);
		HyjalPhase[_unit->GetInstanceID()] = HYJAL_PHASE_RAGE_WINTERCHILL_COMPLETE;
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

protected:

	int nrspells;
};

// AnetheronAI

#define CN_ANETHERON 17808

#define CARRION_SWARM	31306
#define VAMPIRIC_AURA	38196	// 31317
#define INFERNO			31299	// doesn't summon infernal - core bug
#define SLEEP			31292	// 12098
#define BERSERK			26662

class AnetheronAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(AnetheronAI);
	SP_AI_Spell spells[5];
	bool m_spellcheck[5];

    AnetheronAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 

		spells[0].info = dbcSpell.LookupEntry(CARRION_SWARM);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 60.0f;

		spells[1].info = dbcSpell.LookupEntry(VAMPIRIC_AURA);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].perctrigger = 12.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 8;

		spells[2].info = dbcSpell.LookupEntry(INFERNO);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = false;
		spells[2].perctrigger = 6.0f;
		spells[2].attackstoptimer = 3000;
		spells[2].cooldown = 30;
		spells[2].mindist2cast = 0.0f;
		spells[2].maxdist2cast = 60.0f;
		spells[2].speech = "Hit he, no time for a slow death";
		spells[2].soundid = 11039;
		
		spells[3].info = dbcSpell.LookupEntry(SLEEP);
		spells[3].targettype = TARGET_RANDOM_SINGLE;
		spells[3].instant = true;
		spells[3].perctrigger = 5.0f;
		spells[3].attackstoptimer = 3000;
		spells[3].cooldown = 7;
		spells[3].mindist2cast = 0.0f;
		spells[3].maxdist2cast = 30.0f;

		spells[4].info = dbcSpell.LookupEntry(BERSERK);
		spells[4].targettype = TARGET_SELF;
		spells[4].instant = true;
		spells[4].cooldown = 600;
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You are defenders of a doomed world. Flee here and perhaps you will prolong your pathetic lives!");
		_unit->PlaySoundToSet(10977);

		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;

		uint32 t = (uint32)time(NULL);
		spells[4].casttime = t + spells[4].cooldown;

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
		if (_unit->GetHealthPct() > 0)	
		{
			int RandomSpeach=rand()%3;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your hopes are lost.");
				_unit->PlaySoundToSet(10981);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Scream for me.");
				_unit->PlaySoundToSet(11038);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You look tired");
				_unit->PlaySoundToSet(10978);
				break;
			}
		}
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The clock... is still...ticking.");
		_unit->PlaySoundToSet(10982);
		HyjalPhase[_unit->GetInstanceID()] = HYJAL_PHASE_ANETHERON_COMPLETE;
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		uint32 t = (uint32)time(NULL);
		if (t > spells[4].casttime)
		{
			_unit->CastSpell(_unit, spells[4].info, spells[4].instant);

			spells[4].casttime = t + spells[4].cooldown;
			return;
		}

		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					if (i == 1)
					{
						AuraPointer aura = AuraPointer(new Aura(spells[1].info, 5000, _unit, _unit));
						if (aura != NULL)
						_unit->AddAura(aura,NULLAURA);
					}

					else
					{
						target = _unit->GetAIInterface()->GetNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
							case TARGET_RANDOM_FRIEND:
							case TARGET_RANDOM_SINGLE:
							case TARGET_RANDOM_DESTINATION:
								CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
						}
					}

					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

protected:

	int nrspells;
};

// KazrogalAI

#define CN_KAZROGAL 17888

#define K_CLEAVE			31436
#define WAR_STOMP			31480
#define MARK_OF_KAZROGAL	31447
#define MARK_OF_KAZROGAL2	31463	// should it be scripted to attack friends?

class KazrogalAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(KazrogalAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    KazrogalAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		}

		spells[0].info = dbcSpell.LookupEntry(K_CLEAVE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 5;

		spells[1].info = dbcSpell.LookupEntry(WAR_STOMP);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 6.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 10;

		spells[2].info = dbcSpell.LookupEntry(MARK_OF_KAZROGAL);
		spells[2].targettype = TARGET_VARIOUS; 
		spells[2].instant = false;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 2000;
		spells[2].cooldown = 1;

		spells[3].info = dbcSpell.LookupEntry(MARK_OF_KAZROGAL2);
		spells[3].targettype = TARGET_VARIOUS; 
		spells[3].instant = true;

		MarkDeto = 0;
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Cry for mercy! Your meaningless lives will soon be forfeit.");
		_unit->PlaySoundToSet(11015);

		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		TimerMark = 60;
    }

	void OnTargetDied(UnitPointer mTarget)
    {
		if (_unit->GetHealthPct() > 0)	
		{
			int RandomSpeech=rand()%2;
			switch (RandomSpeech)
			{
			/*case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your death will be a painful one.");
				_unit->PlaySoundToSet(11016);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You... are marked");
				_unit->PlaySoundToSet(11052);
				break;*/
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You... are nothing!");
				_unit->PlaySoundToSet(11053);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Miserable nuisance!");
				_unit->PlaySoundToSet(11054);
				break;
			}
		}
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Hahahaha Hahaha");
		_unit->PlaySoundToSet(11018);
		HyjalPhase[_unit->GetInstanceID()] = HYJAL_PHASE_KAZROGAL_COMPLETE;
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		TimerMark--;
		if(!TimerMark && MarkDeto < 6)
		{
			_unit->GetAIInterface()->CastSpell(_unit,dbcSpell.LookupEntry(MARK_OF_KAZROGAL), TARGET_VARIOUS);
			TimerMark = (60 - MarkDeto*10);
			MarkDeto++;
			MarkCast();
		}
		if(!TimerMark && MarkDeto >=6)
		{
			_unit->GetAIInterface()->CastSpell(_unit,dbcSpell.LookupEntry(MARK_OF_KAZROGAL), TARGET_VARIOUS);
			TimerMark = 5;
			MarkDeto++;
			MarkCast();
		}
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}

					if (i == 2)
						MarkCast();

					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void MarkCast()
	{
		for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
		{ 
			if (isHostile(_unit, (*itr)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID())
			{
				UnitPointer Target = NULLUNIT;
				Target = TO_UNIT(*itr);

				if (Target->isAlive() && Target->FindAura(MARK_OF_KAZROGAL) && Target->GetPowerType() == POWER_TYPE_MANA && Target->GetUInt32Value(UNIT_FIELD_POWER2) < 600)
				{
					Target->CastSpell(Target, spells[3].info, spells[3].instant);
				} 
			} 
		}
	}

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

protected:

	uint32 MarkDeto;
	uint32 TimerMark;
	int nrspells;
};

// AzgalorAI

#define CN_AZGALOR 17842
#define CN_DEATHTRIGGER				90001

#define CLEAVE			31345
#define RAIN_OF_FIRE	31340 
#define HOWL_OF_AZGALOR	31344
#define DOOM			31347	// it's applied, but doesn't do anything more - should be scripted?

class AzgalorAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(AzgalorAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    AzgalorAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		}

		spells[0].info = dbcSpell.LookupEntry(CLEAVE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 3000;
		spells[0].cooldown = 10;

		spells[1].info = dbcSpell.LookupEntry(RAIN_OF_FIRE);
		spells[1].targettype = TARGET_RANDOM_DESTINATION;
		spells[1].instant = true;
		spells[1].perctrigger = 7.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 15;
		spells[1].mindist2cast = 0.0f;
		spells[1].maxdist2cast = 40.0f;

		spells[2].info = dbcSpell.LookupEntry(HOWL_OF_AZGALOR);
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;
		spells[2].perctrigger = 9.0f;
		spells[2].attackstoptimer = 2000;
		spells[2].cooldown = 15;

		/*spells[3].info = dbcSpell.LookupEntry(DOOM);
		spells[3].targettype = TARGET_RANDOM_SINGLE; 
		spells[3].instant = true;
		spells[3].cooldown = 45;
		spells[3].mindist2cast = 0.0f;
		spells[3].maxdist2cast = 50.0f;*/
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);

		for (int i = 0; i < 3; i++)
			spells[i].casttime = 0;

		/*uint32 t = (uint32)time(NULL);
		spells[3].casttime = t + spells[3].cooldown;*/

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
		if (_unit->GetHealthPct() > 0)	
		{
			int RandomSpeach;
			RandomUInt(1000);
			RandomSpeach=rand()%3;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Reesh, hokta!");
				_unit->PlaySoundToSet(11001);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "No one is going to save you!");
				_unit->PlaySoundToSet(11047);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Don't fight it");
				_unit->PlaySoundToSet(11048);
				break;
			}
		}
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your time is almost... up!");
		_unit->PlaySoundToSet(11002);
		HyjalPhase[_unit->GetInstanceID()] = HYJAL_PHASE_AZGALOR_COMPLETE;
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		DoomTimer = 20;
		/*uint32 t = (uint32)time(NULL);
		if (t > spells[3].casttime)
		{
			CastSpellOnRandomTarget(3, spells[3].mindist2cast, spells[3].maxdist2cast, 0, 100);
			spells[3].casttime = t + spells[3].cooldown;
			return;
		}*/
		DoomTimer--;
		if(!DoomTimer)
		{
			UnitPointer RandomTarget = NULLUNIT;
			std::vector<UnitPointer > TargetTable;
			for(unordered_set<PlayerPointer>::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr) 
			{ 
				if ((*itr)->GetTypeId() == TYPEID_PLAYER && (*itr)->GetInstanceID() == _unit->GetInstanceID() && (*itr)->GetDistance2dSq(_unit) > 225) //15 yards
				{
					RandomTarget = TO_UNIT(*itr);
					if (RandomTarget && RandomTarget->isAlive())
						TargetTable.push_back(RandomTarget);
				}
			}
			RandomTarget = *(TargetTable.begin()+rand()%TargetTable.size());
			if(RandomTarget)
			{
				_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_DEATHTRIGGER, RandomTarget->GetPositionX(), RandomTarget->GetPositionY(), RandomTarget->GetPositionZ(), 0, false, false, 0, 0);
			}
			DoomTimer = 20;
		}		

		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime && (i != 1 || (i == 1 && _unit->GetHealthPct() >= 20)))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			if (i == 3)
			{
				AuraPointer aura = AuraPointer(new Aura(spells[3].info, 20000, _unit, RTarget));
				if (aura != NULL)
					RTarget->AddAura(aura,NULLAURA);

				TargetTable.clear();
				return;
			}

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

protected:

	int nrspells;
	uint32 DoomTimer;
};

// Archimonde Channel TriggerAI

#define CN_ARCHIMONDE_CHANNEL_TRIGGER 30004

// Additional
#define DRAIN_WORLD_TREE_VISUAL		39140
#define DRAIN_WORLD_TREE_VISUAL2	39141

class ArchimondeTriggerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ArchimondeTriggerAI);

    ArchimondeTriggerAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->m_noRespawn = true;

		UnitPointer Archimonde = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(5598.629883f, -3447.719971f, 1576.650024f, 17968);
		if (Archimonde)
		{
			_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, Archimonde->GetGUID());
			_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, DRAIN_WORLD_TREE_VISUAL2);
		}
	}
};

// DoomfireAI

#define CN_DOOMFIRE 18095
#define DOOMFIRE	31945 // 32492

class DoomfireAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(DoomfireAI);

    DoomfireAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		/*_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->m_noRespawn = true;
		sEventMgr.AddEvent(((UnitPointer )_unit), &Unit::EventCastSpell, ((UnitPointer )_unit), dbcSpell.LookupEntry(DOOMFIRE), EVENT_UNK, 1000, 30, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		*/
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(DOOMFIRE), true);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->m_noRespawn = true;
		RegisterAIUpdateEvent(1000);

		DespawnTimer = 0;
		DirChange = 0;
	}

	void AIUpdate()
	{
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(32492), true);
		DespawnTimer++;
		if (DespawnTimer >= 27)
		{
			_unit->Despawn(1,0);
			DespawnTimer = 0;
		}
		// After 4 sec of last direction change, doomfire has 33% chance to change direction
		DirChange++;
		if ((DirChange == 4 && RandomUInt(3) == 1) || DirChange >= 5)
		{
			if (_unit->GetAIInterface()->getUnitToFollow())
			{
				if (RandomUInt(3) == 1 || _unit->GetDistance2dSq(_unit->GetAIInterface()->getUnitToFollow()) <= 2.0f)
				{
					_unit->GetAIInterface()->SetUnitToFollow(NULLUNIT);
					_unit->GetAIInterface()->SetUnitToFollowAngle(0.0f);
				}
			}

			if (!_unit->GetAIInterface()->getUnitToFollow())
			{
				if (RandomUInt(3) == 1)
				{
					UnitPointer NewTarget = NULLUNIT;
					NewTarget = FindTarget();
					if (NewTarget)
					{
						_unit->GetAIInterface()->SetUnitToFollow(NewTarget);
						_unit->GetAIInterface()->SetUnitToFollowAngle(2.0f);
					}
				}

				if (!_unit->GetAIInterface()->getUnitToFollow())
				{
					float movedist = 10.0f;
					float x = 0.0f;
					float y = 0.0f;

					float xchange = (float)RandomFloat(movedist);
					float ychange = sqrt(movedist*movedist - xchange*xchange);

					if (RandomUInt(2) == 1)
						xchange *= -1;
					if (RandomUInt(2) == 1)
						ychange *= -1;

					x = _unit->GetPositionX() + xchange;
					y = _unit->GetPositionY() + ychange;

					_unit->GetAIInterface()->MoveTo(x, y, _unit->GetPositionZ(), _unit->GetOrientation());
				}
			}

			DirChange = 0;
		}
	}
	// A bit rewritten FindTarget function
	UnitPointer FindTarget()
	{
		UnitPointer target = NULLUNIT;
		float distance = 15.0f;
		float z_diff;

		UnitPointer pUnit;
		float dist;

		for (unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeOppFactsSetBegin(); itr != _unit->GetInRangeOppFactsSetEnd(); itr++)
		{
			if((*itr)->GetTypeId() != TYPEID_UNIT && (*itr)->GetTypeId() != TYPEID_PLAYER)
				continue;

			pUnit = TO_UNIT((*itr));

			if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH))
				continue;

			z_diff = fabs(_unit->GetPositionZ() - pUnit->GetPositionZ());
			if(z_diff > 2.5f)
				continue;

			if(pUnit->m_invisible)
				continue;
			
			if(!pUnit->isAlive() || _unit == pUnit)
				continue;

			dist = _unit->GetDistance2dSq(pUnit);

			if(dist > distance*distance)
				continue;

			if (dist < 3.0f)
				continue;

			distance = dist;
			target = pUnit;
		}

		return target;
	}

protected:

	uint32 DespawnTimer;
	uint32 DirChange;
};

/* * Doomfire - Leaves a trail of fire on the ground, which does 2400 fire 
		damage per second (occasionally feared people run into these and die) */
// DeathTrigger (Doom effect)
#define CN_LESSER_DOOMGUARD	17864
class DeathTriggerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(DeathTriggerAI);

    DeathTriggerAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		_unit->GetAIInterface()->m_canMove = false;
		_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		sEventMgr.AddEvent(((UnitPointer )_unit), &Unit::EventCastSpell, ((UnitPointer )_unit), dbcSpell.LookupEntry(5), EVENT_UNK, 500, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_LESSER_DOOMGUARD,_unit->GetPositionX(),_unit->GetPositionY(),_unit->GetPositionZ(),0,false,false,0,0);
		_unit->Despawn(1000, 0);
	}
};

// Lesser Doomguard
#define CRIPPLE 			31406			
#define WARSTOMP		 	31408

class LesserDoomGuardAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(LesserDoomGuardAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    LesserDoomGuardAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(CRIPPLE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 80.0f;

		spells[1].info = dbcSpell.LookupEntry(WARSTOMP);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 10;
		spells[1].mindist2cast = 0.0f;
		spells[1].maxdist2cast = 30.0f;

    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
	
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
protected:

	int nrspells;
};
// Towering Infernal
#define CN_TOWERING_INFERNAL	17818
#define INFERNOE				31302
#define IMMOLATION		 		31303

class ToweringInfernalAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ToweringInfernalAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    ToweringInfernalAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(INFERNOE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 80.0f;

		spells[1].info = dbcSpell.LookupEntry(IMMOLATION);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 10;
		spells[1].mindist2cast = 0.0f;
		spells[1].maxdist2cast = 30.0f;

    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
protected:

	int nrspells;
};

// ArchimondeAI

#define CN_ARCHIMONDE 17968
#define CN_ANCIENT_WISP 17946

#define FEAR						33547
#define AIR_BURST					32014 
#define GRIP_OF_THE_LEGION			31972
#define DOOMFIRE_STRIKE				31903
#define FINGER_OF_DEATH				31984	// should be casted only when no one in melee range
#define HAND_OF_DEATH				35354	// used if too close to Well of Eternity or if after 10 min caster has more than 10% hp
#define SOUL_CHARGER				32053	// If player dies whole raid gets one of those 3 buffs
#define SOUL_CHARGEO				32054
#define SOUL_CHARGEG				32057
#define PROTECTION_OF_ELUNE			38528

class ArchimondeAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ArchimondeAI);
	SP_AI_Spell spells[7];
	bool m_spellcheck[7];

    ArchimondeAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(AIR_BURST);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 25;
		spells[0].mindist2cast = 30.0f;
		spells[0].maxdist2cast = 200.0f;

		spells[1].info = dbcSpell.LookupEntry(GRIP_OF_THE_LEGION);
		spells[1].targettype = TARGET_RANDOM_SINGLE; 
		spells[1].instant = true;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 10;
		spells[1].mindist2cast = 0.0f;
		spells[1].maxdist2cast = 60.0f;

		spells[2].info = dbcSpell.LookupEntry(DOOMFIRE_STRIKE);
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 2000;

		spells[3].info = dbcSpell.LookupEntry(FEAR);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = true;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;
		spells[3].cooldown = 30;

		spells[4].info = dbcSpell.LookupEntry(FINGER_OF_DEATH);
		spells[4].targettype = TARGET_RANDOM_SINGLE; 
		spells[4].instant = true;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 1000;
		spells[4].mindist2cast = 30.0f;
		spells[4].maxdist2cast = 100.0f;
		spells[4].cooldown = 5;

		spells[5].info = dbcSpell.LookupEntry(HAND_OF_DEATH);
		spells[5].targettype = TARGET_VARIOUS;
		spells[5].instant = true;
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 1000;
		spells[5].cooldown = 600;

		spells[6].instant = true;
		spells[6].cooldown = 10;

		Trigger = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ARCHIMONDE_CHANNEL_TRIGGER, 5501.476563f, -3524.868408f, 1604.188965f, 0.393633f, false, false, 0, 0);

		if (Trigger && Trigger->IsInWorld())
		{
			Trigger->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			Trigger->GetAIInterface()->SetAllowedToEnterCombat(false);
			Trigger->m_noRespawn = true;

			Trigger->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, _unit->GetGUID());
			Trigger->SetUInt32Value(UNIT_CHANNEL_SPELL, DRAIN_WORLD_TREE_VISUAL2);

			_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, Trigger->GetGUID());
			_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, DRAIN_WORLD_TREE_VISUAL);
		}
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your resistance is insignificant.");
		_unit->PlaySoundToSet(10987);

		_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
		_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);

		if (Trigger && Trigger->IsInWorld())
		{
			Trigger->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
			Trigger->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
		}

		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;

		uint32 t = (uint32)time(NULL);
		spells[3].cooldown= 30;
		spells[4].cooldown = 4;
		spells[5].cooldown = 600;
		spells[6].casttime = 0;
		spells[0].cooldown = 25;
		spells[2].perctrigger = 7.0f;

		phase = 1;
		enrage_timer = 0;

		RegisterAIUpdateEvent( 1000 );
    }

	void OnTargetDied(UnitPointer mTarget)
    {
		if (_unit->GetHealthPct() > 0 && phase == 1)	// Hack to prevent double yelling (OnDied and OnTargetDied when creature is dying)
		{
			int RandomSpeach=rand()%3;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your soul will languish for eternity.");
				_unit->PlaySoundToSet(10993);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "All creation will be devoured!");
				_unit->PlaySoundToSet(10994);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I am the coming of the end!");
				_unit->PlaySoundToSet(10995);
				break;
			}

			if( mTarget->GetTypeId() == TYPEID_PLAYER && _unit->isAlive() )
			{
				uint32 SpellID = 0;
				if (mTarget->getClass() == WARRIOR || mTarget->getClass() == ROGUE || mTarget->getClass() == MAGE)
					SpellID = SOUL_CHARGEO;
				if (mTarget->getClass() == PRIEST || mTarget->getClass() == PALADIN || mTarget->getClass() == WARLOCK)
					SpellID = SOUL_CHARGER;
				else
					SpellID = SOUL_CHARGEG;
				
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SpellID), spells[6].instant);
				
				int RandomSpeech2=rand()%2;
				switch (RandomSpeech2)
				{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "This world will burn!");
					_unit->PlaySoundToSet(10990);
					break;
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Manach sheek-thrish!");
					_unit->PlaySoundToSet(11041);
					break;
				} 
	 		}
		}
    }

    void OnCombatStop(UnitPointer mTarget)
    {
		if( _unit->HasAura(GENERIC_ENRAGE) )
			_unit->RemoveAura(GENERIC_ENRAGE);

		_unit->GetAIInterface()->disable_melee = false;
		_unit->GetAIInterface()->m_canMove = true;

        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

		if (Trigger && Trigger->IsInWorld() && _unit->isAlive())
		{
			Trigger->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, _unit->GetGUID());
			Trigger->SetUInt32Value(UNIT_CHANNEL_SPELL, DRAIN_WORLD_TREE_VISUAL2);

			_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, Trigger->GetGUID());
			_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, DRAIN_WORLD_TREE_VISUAL);
		}

        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "No, it cannot be! Nooo!");
		_unit->PlaySoundToSet(10992);

       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if( enrage_timer == 600 && phase != 2 )
		{   
			AuraPointer aura = AuraPointer(new Aura(dbcSpell.LookupEntry(GENERIC_ENRAGE), 300000, _unit, _unit));
			if (aura)
               _unit->AddAura(aura,NULLAURA);

		/*	AuraPointer aura = new Aura(dbcSpell.LookupEntry(GENERIC_ENRAGE), 300000, _unit, _unit);
			aura->m_areaAura = false;
			aura->AddMod(13, 500, 127, 0);
			aura->AddMod(138, 50, 0, 1);
			_unit->AddAura(aura);*/ 
		}

		//Distance to Well of Eternity Check
		GameObjectPointer well_of_eternity = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(5476.571289f, -3533.433838f, 1552.217407f, 21583);
		if( well_of_eternity != NULL )
		{
			float dist = _unit->CalcDistance( static_cast< ObjectPointer >( well_of_eternity ) );
			if( dist < 80.0f )
				_unit->CastSpell(_unit, HAND_OF_DEATH, true);

		}

		if( phase == 2 || _unit->GetHealthPct() <= 10 )
		{
			
			if( phase == 1 )
			{
				if( enrage_timer < 600 )
					enrage_timer -= (600-enrage_timer)*3;

				RemoveAIUpdateEvent();
				RegisterAIUpdateEvent( 333 );
				phase = 2;
                _unit->GetAIInterface()->disable_melee = true;
				_unit->GetAIInterface()->m_canMove = false;
				spells[3].cooldown = 600;
				spells[2].perctrigger = 0.0f;

				std::vector<UnitPointer > TargetTable;		
				for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
				{ 
					if ( (*itr) && (*itr)->IsPlayer() )
					{
						UnitPointer tg = TO_UNIT(*itr);
						if( tg && tg->isAlive() )
						{   
							AuraPointer aura = AuraPointer(new Aura(dbcSpell.LookupEntry(PROTECTION_OF_ELUNE), 60000, tg, tg));
			                if (aura)
				              tg->AddAura(aura,NULLAURA);
						/*	AuraPointer aura = new Aura(dbcSpell.LookupEntry(PROTECTION_OF_ELUNE), 60000, tg, tg);
							aura->m_areaAura = false;
							aura->AddMod(4, -99, 127, 0);
							tg->AddAura(aura);*/ 
						}
					}
				}
			}

			for(uint8 i = 0; i<RandomUInt(1); i++)
			{
				CreaturePointer cre = NULLCREATURE;
				cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ANCIENT_WISP, 
					(_unit->GetPositionX()+RandomFloat(100)-50), (_unit->GetPositionY()+RandomFloat(100)-50),
					_unit->GetPositionZ()+5, _unit->GetOrientation(),
					true, false, 0, 0);
				if( cre )
				{
					cre->GetAIInterface()->SetUnitToFollow(_unit);
					cre->GetAIInterface()->SetUnitToFollowAngle(0.0f);
					cre->Despawn(60000,0);
					cre->m_noRespawn = true;
					cre->GetAIInterface()->AttackReaction(_unit, 50000, 0);
				}
			}

			UnitPointer wisp = NULLUNIT;
			wisp = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX()+RandomFloat(50)-25, _unit->GetPositionY()+RandomFloat(50)-25, _unit->GetPositionZ(), CN_ANCIENT_WISP);
			if( wisp != NULL && wisp->isAlive() )
				_unit->CastSpell(wisp,FINGER_OF_DEATH,true);
		}

		if( spells[3].cooldown == 0 && phase == 1 )
		{
			_unit->CastSpell(_unit, spells[3].info, spells[3].instant);

			spells[3].cooldown = 30;
		}

		if ( spells[5].cooldown == 0 )
		{
			_unit->CastSpell(_unit, spells[5].info, spells[5].instant);

			spells[5].cooldown = 600;
		}
		
		if( spells[4].cooldown == 0 )
		{
			uint32 counter = 0;
			std::vector<UnitPointer > TargetTable;		
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if ( (*itr) && (*itr)->IsPlayer() )
				{
					UnitPointer tg = TO_UNIT(*itr);
					float dist = _unit->GetDistance2dSq( static_cast< ObjectPointer >( tg ) );
					if( tg->isAlive() && dist <= 400.0f )
						counter++;
				}
			}

			if( counter == 0 )
				CastSpellOnRandomTarget(4, 10, 100, 0, 100);
	
			spells[4].cooldown = 4;
		}

		if( spells[0].cooldown == 0 && phase == 1 )
		{
			spells[0].cooldown = 25;
			CastSpellOnRandomTarget(0, spells[0].mindist2cast, spells[0].maxdist2cast, 0, 100);
		}

		spells[3].cooldown--;
		spells[4].cooldown--;
		spells[5].cooldown--;
		spells[0].cooldown--;

		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}

					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

protected:
	CreaturePointer Trigger;
	int nrspells;
	int enrage_timer;
	int phase;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define STARFALL		37124
#define	SEARINGA		20688

class HTyrandeAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HTyrandeAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    HTyrandeAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(STARFALL);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 15.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = -1;
		
		spells[1].info = dbcSpell.LookupEntry(SEARINGA);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 15.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = -1;
		
    }
    
    void OnDamageTaken(UnitPointer mAttacker, float fAmount) 
	{
		if (!OnDamageTimer)
		{
			uint32 RandomSpeech = rand()%2;
			switch(RandomSpeech)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I cannot hold them off for long!");
				_unit->PlaySoundToSet(11065);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'll fight them as long as I can!");
				_unit->PlaySoundToSet(11066);
				break;
			}
			OnDamageTimer = 17;
		}
	}
	void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		RallyTimer = 20;
		OnDamageTimer = 15;
		
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "NO! All... is... lost.");
		_unit->PlaySoundToSet(11068);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		OnDamageTimer--;
		RallyTimer--;
		if(!RallyTimer)
		{
			uint32 RallySpeech = rand()%3;
			switch(RallySpeech)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "This is our final stand! Make every blow count!");
					_unit->PlaySoundToSet(11062);
					break;
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Fight! Fight to your dying breath!");
					_unit->PlaySoundToSet(11063);
					break;
				case 2:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Give every ounce of strength! Do not despair!");
					_unit->PlaySoundToSet(11064);
					break;
			}
			RallyTimer = 60;
		}
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
protected:
	uint32 OnDamageTimer;
	int nrspells;
	uint32 RallyTimer;
};

// Mobs:
// Bad guys >D
// Abomination

#define DISEASECLOUD		31607
#define KNOCKDOWN			31610

class AbominationAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(AbominationAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    AbominationAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(DISEASECLOUD);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
		
		spells[1].info = dbcSpell.LookupEntry(KNOCKDOWN);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 10;
		spells[1].mindist2cast = 0.0f;
		spells[1].maxdist2cast = 5.0f;

    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
protected:
	int nrspells;
};

// Banshee

#define ANTIMAGIC			31662  
#define BANSHEECURSE		31651
#define BANSHEEWAIL			38183

class BansheeAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BansheeAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    BansheeAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(ANTIMAGIC);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
		
		spells[1].info = dbcSpell.LookupEntry(BANSHEECURSE);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = false;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 10;
		spells[1].mindist2cast = 0.0f;
		spells[1].maxdist2cast = 30.0f;
		
		spells[2].info = dbcSpell.LookupEntry(BANSHEEWAIL);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = false;
		spells[2].perctrigger = 8.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].cooldown = 10;
		spells[2].mindist2cast = 0.0f;
		spells[2].maxdist2cast = 30.0f;

    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	protected:
	int nrspells;
};

// Crypt Fiend

#define POISONSPRAY			30043

class CfiendAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(CfiendAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    CfiendAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(POISONSPRAY);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 15;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 40.0f;
		
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	
	protected:
	int nrspells;
};

// Ghoul
#define CANIBALIZE			31537
#define FRENZY				31540

class GhoulAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(GhoulAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    GhoulAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(CANIBALIZE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
		
		spells[1].info = dbcSpell.LookupEntry(FRENZY);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 10;

    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
protected:
	int nrspells;
};

// Shadowy Necromancer
#define SHADOWBOLTSN			31627/*
#define SUMMONMAGE				31625
#define SUMMONINVADER			31624
#define SUMMONBOTH				31617*/ 

class SNecromancerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SNecromancerAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    SNecromancerAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(SHADOWBOLTSN);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 15.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 40.0f;
/*
		spells[1].info = dbcSpell.LookupEntry(SUMMONMAGE);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 10;

		spells[2].info = dbcSpell.LookupEntry(SUMMONINVADER);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].cooldown = 10;

		spells[3].info = dbcSpell.LookupEntry(SUMMONBOTH);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].perctrigger = 5.0f;
		spells[3].attackstoptimer = 1000;
		spells[3].cooldown = 10;
		
*/
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
protected:
	int nrspells;
};

// Gargoyle

#define GARGOYLESTRIKE			31664

class HGargoyleAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HGargoyleAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    HGargoyleAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		_unit->GetAIInterface()->setMoveType(768);
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(GARGOYLESTRIKE);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 40.0f;
		

    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	protected:
	int nrspells;
};

// Fel Stalker
#define MANABURN			31729

class HFelStalkerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HFelStalkerAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    HFelStalkerAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(MANABURN);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 40.0f;
		
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	protected:
	int nrspells;
};

// Frost Wyrm
#define FROSTBREATH			31688

class HFwyrmAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HFwyrmAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    HFwyrmAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		_unit->GetAIInterface()->setMoveType(768);
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(FROSTBREATH);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 40.0f;
		

    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	protected:
	int nrspells;
};

// Giant Infernal // No Info --> Abomination KNOCKBACK

class GiantInfAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(GiantInfAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    GiantInfAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		_unit->m_noRespawn = true;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(KNOCKDOWN);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 15;
		
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	
	protected:
	int nrspells;
};

// Skeleton Invader // No Info T_T
#define CN_HSKI					17902
// Skeleton Mage
#define CN_HSKM					17903
#define FIREBALLSK				31620
#define FROSTBALLSK				31622
#define SHADOWBOLTSK			31618

class HSkeletonMageAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HSkeletonMageAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    HSkeletonMageAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		TimerDespawn = 60;
		_unit->m_noRespawn = true;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(FIREBALLSK);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 40.0f;
		
		spells[1].info = dbcSpell.LookupEntry(SHADOWBOLTSK);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = false;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 10;
		spells[1].mindist2cast = 0.0f;
		spells[1].maxdist2cast = 40.0f;
		
		spells[2].info = dbcSpell.LookupEntry(FROSTBALLSK);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = false;
		spells[2].perctrigger = 8.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].cooldown = 10;
		spells[2].mindist2cast = 0.0f;
		spells[2].maxdist2cast = 40.0f;

    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
		_unit->Despawn(1, 0);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		TimerDespawn--;
			if(!TimerDespawn)
				_unit->Despawn(1, 0);
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	protected:
	int nrspells;
	uint32 TimerDespawn;
};


// GOOD GUYS :D

// Alliance Footman

#define CN_FOOTMAN			17919
#define SHIELDWALL			31731

class HAFootmanAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HAFootmanAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    HAFootmanAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(SHIELDWALL);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
		
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
protected:
	int nrspells;
};

// Alliance Knight

#define CN_HAKNIGHT			17920
#define RALLYINGCRY			31732

class HAKnightAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HAKnightAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    HAKnightAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(RALLYINGCRY);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;		
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
protected:
	int nrspells;
};

// Alliance Peasant //Just pee.
// Alliance Priest

#define CN_PRIEST		17928
#define HEAL			31739 //46564 //NOt right one, but heals raid.
#define SMITE			31740

class HAPriestAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HAPriestAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    HAPriestAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		spells[0].info = dbcSpell.LookupEntry(HEAL);
		spells[0].targettype = TARGET_RANDOM_FRIEND;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 40.0f;
		
		spells[1].info = dbcSpell.LookupEntry(SMITE);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = true;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 10;
		spells[1].mindist2cast = 0.0f;
		spells[1].maxdist2cast = 40.0f;
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	protected:
	int nrspells;
};

// Alliance Rifleman

#define CN_HARIFLE			17921
#define EXPLODINGSHOOT		7896 //NOt right one, but heals raid.
#define SHOOT				32103

class HARiflemanAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HARiflemanAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    HARiflemanAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		spells[0].info = dbcSpell.LookupEntry(EXPLODINGSHOOT);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 30.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 40.0f;
		
		spells[1].info = dbcSpell.LookupEntry(SHOOT);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = true;
		spells[1].perctrigger = 30.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 10;
		spells[1].mindist2cast = 0.0f;
		spells[1].maxdist2cast = 40.0f;
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	protected:
	int nrspells;
};

// Alliance Sorceress

#define CN_ASORCERESS	17922
#define	SLOW			31741
#define	ARCANE_MISSILE	31743

class HASorceressAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HASorceressAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    HASorceressAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		spells[0].info = dbcSpell.LookupEntry(SLOW);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 40.0f;
		
		spells[1].info = dbcSpell.LookupEntry(ARCANE_MISSILE);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = true;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 10;
		spells[1].mindist2cast = 0.0f;
		spells[1].maxdist2cast = 40.0f;
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	protected:
	int nrspells;
};


// Ancient Wisp

#define CN_ANCIENTW				17946
#define ANCIENTSPARK			39349

class HAAncientWAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HAAncientWAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    HAAncientWAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		_unit->GetAIInterface()->setMoveType(768);
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(ANCIENTSPARK);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 80.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = -1;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 100.0f;
		
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		TimerDespawn--;
		if(!TimerDespawn)
		{
			_unit->Despawn(1,0);
		}
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
protected:
	int nrspells;
	uint32 TimerDespawn;
};

// Dire Wolf
#define CN_DIRE_WOLF	17854
#define GROWL			31334

class HHDireWolfAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HHDireWolfAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    HHDireWolfAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		_unit->m_noRespawn = true;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(GROWL);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
		
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
protected:
	int nrspells;
};

// Druid of the Claw //NO INFO T_T
// Druid of the Talon //NO INFO T_T
// Dryad //NO INFO T_T
// Guardian Water Elemental //NO INFO T_T

#define CN_WATERELE		18001	
#define FROSTBOLTWE		31622

class HWaterEleAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HWaterEleAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    HWaterEleAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		_unit->m_noRespawn = true;
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(FROSTBOLTWE);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 40.0f;
		
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
protected:
	int nrspells;
};

// Horde Grunt

#define CN_HGRUNT		17932
#define STRIKE			31754

class HHGruntAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HHGruntAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    HHGruntAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(STRIKE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
protected:
	int nrspells;
};

// Horde Headhunter

#define CN_HEADH		17934
#define SPEART			31758

class HHHeadHunterAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HHHeadHunterAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    HHHeadHunterAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(SPEART);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
protected:
	int nrspells;
};

// Horde Healing Ward
#define CN_HHWARD			18036

class HHHWardAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HHHWardAI);
	
    HHHWardAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		_unit->GetAIInterface()->m_canMove = false;
		sEventMgr.AddEvent(((UnitPointer )_unit), &Unit::EventCastSpell, ((UnitPointer )_unit), dbcSpell.LookupEntry(HEAL), EVENT_UNK, 5000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		_unit->Despawn(30000, 0);
		
    }
};

// Horde Peon // Just Pee.
// Horde Shaman

#define CN_HASHAMAN			17936
#define BLOODLUST			6742
#define LSHIELD				31765
#define LBOLT				31764

class HHShamanAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HHShamanAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    HHShamanAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(BLOODLUST);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
		
		spells[1].info = dbcSpell.LookupEntry(LSHIELD);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 10;
		
		spells[2].info = dbcSpell.LookupEntry(LBOLT);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = false;
		spells[2].perctrigger = 8.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].cooldown = 10;
		spells[2].mindist2cast = 0.0f;
		spells[2].maxdist2cast = 40.0f;
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	protected:
	int nrspells;
};

// Horde Witch Doctor

#define CN_HWITCHDR			17935
#define HOLYBOLT			31759
#define SUMMONWARD			31760

class HHWitchDrAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HHWitchDrAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    HHWitchDrAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(HOLYBOLT);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 40.0f;

		spells[1].info = dbcSpell.LookupEntry(SUMMONWARD);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 10;
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		TimerSummon = 15;
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		/*TimerSummon--;
			if(!TimerSummon)
			{
				_unit->GetMapMgr()->GetInterface()->SpawnCreature(18036, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0, false, false, 0, 0);
			}*/
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	protected:
	int nrspells;
	uint32 TimerSummon;
};


// Night Elf Ancient Protector // No Info --> Ancient Whisp
#define CN_NEAPROTE			18487
// Night Elf Ancient of Lore // No Info --> Ancient Whisp
#define CN_NEALORE			18486
// Night Elf Ancient of War // No Info --> Ancient Whisp
#define CN_NEAWAR			18485
// Night Elf Archer // NO Info --> Rifleman
#define CN_NEARCHER			17943
// Night Elf Huntress // NO Info --> HeadHunter
#define CN_NEHUNT			17945
// Night Elf Wisp // No Info --> Ancient Whisp
#define CN_NEWHISP			18502
// Tauren Warrior
#define CN_TWARRIOR			17933
#define WARSTOMP2			31755

class HHTaurenWarriorAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HHTaurenWarriorAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    HHTaurenWarriorAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(WARSTOMP2);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;		
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}
protected:
	int nrspells;
};
// Puerta Keeper
#define CN_PUERTAKEEPER		90003
class PuertasAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(PuertasAI);
	
    PuertasAI(CreaturePointer pCreature) : CreatureAIScript(pCreature)
    {
		GateHordaOn = false;
		GateElfaOn = false;
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->GetAIInterface()->m_canMove = false;
		RegisterAIUpdateEvent(1000);
	}
	void AIUpdate()
	{
		GateHorda = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(4271.63f, -4143.69f, 869.941f, 182060);
		GateElfa = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(4204.94f, -4113.65f, 877.722f, 182061);

		if(GateHorda != NULL)
		{
			GateHorda->SetUInt32Value(GAMEOBJECT_FLAGS, 1);
		}
		if(GateElfa != NULL)
		{
			GateElfa->SetUInt32Value(GAMEOBJECT_FLAGS, 1);
		}
		if(GateHorda != NULL && BossHasBeenKilled(17808) && !GateHordaOn)
		{
			HyjalPhase[_unit->GetInstanceID()] = HYJAL_PHASE_ANETHERON_COMPLETE;
			GateHorda->SetByte(GAMEOBJECT_BYTES_1, 0, 0);
			GateHordaOn = true;
		}
		if(GateElfa != NULL && BossHasBeenKilled(17842) && !GateElfaOn)
		{
			HyjalPhase[_unit->GetInstanceID()] = HYJAL_PHASE_AZGALOR_COMPLETE;
			GateElfa->SetByte(GAMEOBJECT_BYTES_1, 0, 0);
			GateElfaOn = true;
		}
	}
	bool BossHasBeenKilled(uint32 creatureId)
		{
			return _unit->GetMapMgr()->pInstance->m_killedNpcs.find(creatureId) != _unit->GetMapMgr()->pInstance->m_killedNpcs.end();
		}
protected:
	GameObjectPointer GateHorda;
	GameObjectPointer GateElfa;
	bool GateHordaOn;
	bool GateElfaOn;
	
};

void SetupBattleOfMountHyjal(ScriptMgr * mgr)
{
	// Leaders AI & Gossip
	GossipScript * jainaGS = (GossipScript*) new JainaProudmooreGS;
	mgr->register_gossip_script(CN_JAINA_PROUDMOORE, jainaGS);
	mgr->register_creature_script(CN_JAINA_PROUDMOORE, &JainaProudmooreAI::Create);

	GossipScript * thrallGS = (GossipScript*) new ThrallGS;
	mgr->register_gossip_script(CN_THRALL, thrallGS);
	mgr->register_creature_script(CN_THRALL, &ThrallAI::Create);
	// Bosses & Triggers
	mgr->register_creature_script(CN_RAGE_WINTERCHILL, &RageWinterchillAI::Create);
	mgr->register_creature_script(CN_ANETHERON, &AnetheronAI::Create);
	mgr->register_creature_script(CN_KAZROGAL, &KazrogalAI::Create);
	mgr->register_creature_script(CN_AZGALOR, &AzgalorAI::Create);
	mgr->register_creature_script(CN_ARCHIMONDE_CHANNEL_TRIGGER, &ArchimondeTriggerAI::Create);
	mgr->register_creature_script(CN_DOOMFIRE, &DoomfireAI::Create);
    mgr->register_creature_script(CN_ARCHIMONDE, &ArchimondeAI::Create);
	mgr->register_creature_script(CN_DEATHTRIGGER, &DeathTriggerAI::Create);
	mgr->register_creature_script(CN_LESSER_DOOMGUARD, &LesserDoomGuardAI::Create);
	mgr->register_creature_script(CN_TOWERING_INFERNAL, &ToweringInfernalAI::Create);
	mgr->register_creature_script(CN_TRIGGER_HYJALA, &TriggerHyjalaAI::Create);
	// Mobs
	//Bad Guys >D 13 NPCS
	mgr->register_creature_script(CN_ABOMINATION, &AbominationAI::Create);
	mgr->register_creature_script(CN_BANSH, &BansheeAI::Create);
	mgr->register_creature_script(CN_CRYPT, &CfiendAI::Create);
	mgr->register_creature_script(CN_GHOUL, &GhoulAI::Create);
	//mgr->register_creature_script(CN_NECROMANCER, &SNecromancerAI::Create);
	mgr->register_creature_script(CN_GARGO, &HGargoyleAI::Create);
	mgr->register_creature_script(CN_FWYRM, &HFwyrmAI::Create);
	mgr->register_creature_script(CN_FSTAL, &HFelStalkerAI::Create);
	mgr->register_creature_script(CN_GINFE, &GiantInfAI::Create);
	mgr->register_creature_script(CN_HSKM, &HSkeletonMageAI::Create);
	//Good Guys :D 20 NPCS
	mgr->register_creature_script(CN_FOOTMAN, &HAFootmanAI::Create);
	mgr->register_creature_script(CN_HAKNIGHT, &HAKnightAI::Create);
	mgr->register_creature_script(CN_PRIEST, &HAPriestAI::Create);
	mgr->register_creature_script(CN_HARIFLE, &HARiflemanAI::Create);
	mgr->register_creature_script(CN_ASORCERESS, &HASorceressAI::Create);
	mgr->register_creature_script(CN_DIRE_WOLF, &HHDireWolfAI::Create);
	mgr->register_creature_script(CN_WATERELE, &HWaterEleAI::Create);
	mgr->register_creature_script(CN_HGRUNT, &HHGruntAI::Create);
	mgr->register_creature_script(CN_HHWARD, &HHHWardAI::Create);
	mgr->register_creature_script(CN_HASHAMAN, &HHShamanAI::Create);
	mgr->register_creature_script(CN_HEADH, &HHHeadHunterAI::Create);
	mgr->register_creature_script(CN_HWITCHDR, &HHWitchDrAI::Create);
	mgr->register_creature_script(CN_ANCIENTW, &HAAncientWAI::Create);
	mgr->register_creature_script(CN_NEWHISP, &HAAncientWAI::Create);
	mgr->register_creature_script(CN_NEHUNT, &HHHeadHunterAI::Create);
	mgr->register_creature_script(CN_NEARCHER, &HARiflemanAI::Create);
	mgr->register_creature_script(CN_NEAWAR, &HAAncientWAI::Create);
	mgr->register_creature_script(CN_NEALORE, &HAAncientWAI::Create);
	mgr->register_creature_script(CN_NEAPROTE, &HAAncientWAI::Create);
	mgr->register_creature_script(CN_TWARRIOR, &HHTaurenWarriorAI::Create);
	mgr->register_creature_script(CN_PUERTAKEEPER, &PuertasAI::Create);
}
