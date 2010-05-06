/*
 *    Raid_TheObdsidianSanctum.cpp
 *
 *    WoW Legacy Scripts
 *    Copyrights 2009 WoWLegacy
 *    www.wowlegacy.net
 *    By Azolex 2009
 */
 
#include "StdAfx.h"
#include "Setup.h"
#include "Base.h"

// Sartharion *The Onyx Guardian*
#define BOSS_SARTHARION 28860
#define FLAME_TSUNAMI 30616 // Thanks wow Cache!
#define ADD_LAVA_BLAZE 30643
#define TORNADO 30648
#define CN_TENEBRON 30452
#define CN_VESPERON 30449
#define CN_SHADRON 30451

// Spells
#define SARTHARION_CLEAVE 56909
#define SARTHARION_ENRAGE 61632
#define SARTHARION_AURA 61254

// Normal mode spells
#define SARTHARION_FLAME_BREATH_NM 56908
#define SARTHARION_TAIL_LASH_NM 56910

// Heroic mode spells
#define SARTHARION_FLAME_BREATH_HC 58956
#define SARTHARION_TAIL_LASH_HC 58957

// Tsunami spells
#define TSUNAMI 57492
#define TSUNAMI_VISUAL 57494
#define TORNADO_SPELL 57560


class SartharionAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SartharionAI, MoonScriptBossAI);
	SartharionAI(CreaturePointer pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(SARTHARION_CLEAVE, Target_Current, 24, 0, 8 );
			
		//mEnrage = AddSpell(SARTHARION_ENRAGE, Target_Current, 0, 0, 0); //Need drake support :)
		//14103
		if( IsHeroic() )
		{
			mFlame = AddSpell( SARTHARION_FLAME_BREATH_HC, Target_Self, 18, 2, 16); 
			mFlame->AddEmote("Burn, you miserable wretches!", Text_Yell, 14098);
			AddSpell( SARTHARION_TAIL_LASH_HC, Target_Self, 40, 0, 12);
		}
		else
		{
			mFlame = AddSpell( SARTHARION_FLAME_BREATH_NM, Target_Self, 18, 2, 16); 
			mFlame->AddEmote("Burn, you miserable wretches!", Text_Yell, 14098);
			AddSpell( SARTHARION_TAIL_LASH_NM, Target_Self, 40, 0, 12);
		};
			

		AddEmote(Event_OnTargetDied, "You will make a fine meal for the hatchlings.", Text_Yell, 14094);
		AddEmote(Event_OnTargetDied, "You are at a grave disadvantage!", Text_Yell, 14096);
		AddEmote(Event_OnTargetDied, "This is why we call you lesser beings.", Text_Yell, 14097);
			
		AddEmote(Event_OnCombatStart, "It is my charge to watch over these eggs. I will see you burn before any harm comes to them!", Text_Yell, 14093);

		SetAIUpdateFreq(1000);
		
		pTsunamiOne = NULLCREATURE;
		pTsunamiTwo = NULLCREATURE;
		pTsunamiThree = NULLCREATURE;

		CanMove = false;
	};

	void OnCombatStart(UnitPointer pTarget)
	{
		CheckDrakes();
		
		if(aTenebron == true || aVesperon == true || aShadron == true)
		{
			sLog.outDebug("Sath with 1 or more drakes");
			ApplyAura(SARTHARION_AURA);
			mDrakeTimer = AddTimer(27000);
		}

		pTsunamiOne = NULLCREATURE;
		pTsunamiTwo = NULLCREATURE;
		pTsunamiThree = NULLCREATURE;

		mLavaTimer = AddTimer(28000);
		mAddTimer = AddTimer(6000);

		ParentClass::OnCombatStart(pTarget);
	};

	void CheckDrakes()
	{
		sLog.outDebug("Checking drakes!");
		CreaturePointer Tenebron = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( 3239.07f, 657.235f, 86.9775f, CN_TENEBRON );
		CreaturePointer Vesperon = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( 3363.06f, 525.28f, 98.462f, CN_VESPERON );
		CreaturePointer Shadron = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( 3145.68f, 520.71f, 89.8f, CN_SHADRON );
	
		if(Tenebron != NULL && Tenebron->isAlive()) 
				aTenebron = true;

		if(Vesperon != NULL && Vesperon->isAlive())
				aVesperon = true;

		if(Shadron != NULL && Shadron->isAlive())
				aShadron = true;
	};

	void AIUpdate()
	{
		if(IsInCombat()) //if encounter is in progress, else we dont need this
		{

			if(IsTimerFinished(mLavaTimer))
			{
				_unit->SendChatMessage(CHAT_MSG_RAID_BOSS_EMOTE, LANG_UNIVERSAL, "The lava surrounding Sartharion churns!");

				uint32 RandomSpeach = rand()%4;
				switch (RandomSpeach)		
				{
				case 0:
					Emote( "Such flammable little insects....", Text_Yell, 14100);	
					break;
				case 1:
					Emote( "Your charred bones will litter the floor!", Text_Yell, 14101);
					break;
				case 2:
					Emote( "How much heat can you take?", Text_Yell, 14102);
					break;
				case 3:
					Emote( "All will be reduced to ash!", Text_Yell, 14103);
					break;				
				}

				pTsunamiOne = this->GetUnit()->GetMapMgr()->GetInterface()->SpawnCreature(FLAME_TSUNAMI, 3283.215820f, 511.234100f, 59.288776f, 3.148659f, true, true, NULL, NULL);  
				pTsunamiTwo = this->GetUnit()->GetMapMgr()->GetInterface()->SpawnCreature(FLAME_TSUNAMI, 3286.661133f, 533.317261f, 59.366989f, 3.156505f, true, true, NULL, NULL);  
				pTsunamiThree = this->GetUnit()->GetMapMgr()->GetInterface()->SpawnCreature(FLAME_TSUNAMI, 3283.311035f, 556.839611f, 59.397129f, 3.105450f, true, true, NULL, NULL);   
			
				CanMove = true;

				ResetTimer(mLavaTimer, 35000);
			}

			if(CanMove == true)
			{
				if(pTsunamiOne != NULL)
				{
					pTsunamiOne->GetAIInterface()->MoveTo(3211.564697f, 505.982727f, 59.556610f, true);
					sLog.outDebug("Moving 1st tsunami!");
				}
				if(pTsunamiTwo!= NULL)
				{
					pTsunamiTwo->GetAIInterface()->MoveTo(3214.280029f, 531.491089f, 59.168331f, true);
					sLog.outDebug("Moving 2nd tsunami!");
				}
				if(pTsunamiThree != NULL)
				{
					pTsunamiThree->GetAIInterface()->MoveTo(3211.609131f, 560.359375f, 59.420803f, true);
					sLog.outDebug("Moving 3rd tsunami!");
				}
				else
				{
					sLog.outDebug("pTsunami == NULL;");
				}

				CanMove = false;
			};

			if(IsTimerFinished(mAddTimer))
			{			
				for(uint32 i = 0; i < 2; ++i)
					SpawnCreature(ADD_LAVA_BLAZE, _unit->GetPositionX() + 6, _unit->GetPositionY() + i, _unit->GetPositionZ(), _unit->GetOrientation(), true);

				ResetTimer(mAddTimer, 15000);
			};
		
			if(_unit->GetHealthPct() <= 10)
			{
				for(uint32 i = 0; i < 6; ++i)
					SpawnCreature(ADD_LAVA_BLAZE, _unit->GetPositionX() + 6, _unit->GetPositionY() + i, _unit->GetPositionZ(), _unit->GetOrientation(), true);
			
				RemoveTimer(mAddTimer);
			};

			// Drake part
			if(IsTimerFinished( mDrakeTimer ) && aTenebron == true )
			{
				CreaturePointer Tenebron = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( 3239.07f, 657.235f, 86.9775f, CN_TENEBRON);
			
				if(Tenebron != NULL)
				{
					Emote( "Tenebron! The eggs are yours to protect as well!", Text_Yell, 14106);
					Tenebron->GetAIInterface()->MoveTo( _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation());
					Tenebron->CastSpell( Tenebron, 61248, true);
					sLog.outDebug("Calling drake 1");
				};

				ResetTimer( mDrakeTimer, 45000);
			}
			else
			{
				ResetTimer( mDrakeTimer, 45000);
			};

			if(IsTimerFinished( mDrakeTimer ) && aShadron == true)
			{
				CreaturePointer Shadron = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( 3145.68f, 520.71f, 89.8f, CN_SHADRON); 
			
				if(Shadron != NULL)
				{
					Emote( "Vesperon! The clutch is in danger! Assist me!", Text_Yell, 14104);
					Shadron->GetAIInterface()->MoveTo( _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation());
					Shadron->CastSpell( Shadron, 58105, true);
					sLog.outDebug("Calling drake 2");
				};

				ResetTimer( mDrakeTimer, 45000);
			}
			else
			{
				ResetTimer( mDrakeTimer, 45000);
			};

			if(IsTimerFinished( mDrakeTimer ) && aVesperon == true)
			{
				CreaturePointer Vesperon = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( 3363.06f, 525.28f, 98.462f, CN_VESPERON);
			
				if(Vesperon != NULL)
				{
					Emote( "Shadron! Come to me, all is at risk!", Text_Yell, 14105);
					Vesperon->GetAIInterface()->MoveTo( _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation());
					Vesperon->CastSpell(Vesperon, 61251, true);
					sLog.outDebug("Calling drake 3");
				};

				RemoveTimer( mDrakeTimer );
			}
			else
			{
				RemoveTimer( mDrakeTimer );
			}

			
		};

		ParentClass::AIUpdate();		
	};
	
	void OnDied(UnitPointer pKiller)
	{
		Emote( "Such is the price... of failure...", Text_Yell, 14107);
		
		RemoveAIUpdateEvent();
		ParentClass::OnDied(pKiller);
	};

	void OnCombatStop(UnitPointer pTarget)
	{
		CreaturePointer Tenebron = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), CN_TENEBRON);
		CreaturePointer Vesperon = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), CN_VESPERON);
		CreaturePointer Shadron = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), CN_SHADRON);
		
		if(aTenebron == true && Tenebron != NULL && Tenebron->isAlive())
		{
			Tenebron->GetAIInterface()->MoveTo( Tenebron ->GetSpawnX(), Tenebron ->GetSpawnY(), Tenebron->GetSpawnZ(), Tenebron->GetSpawnO()); 
		}
		else if(aTenebron == true && Tenebron != NULL && !Tenebron->isAlive())
		{
			Tenebron->SetUInt64Value(UNIT_FIELD_HEALTH,_unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
			Tenebron->setDeathState(ALIVE);
			Tenebron->GetAIInterface()->WipeTargetList();
			Tenebron->GetAIInterface()->WipeHateList();
			Tenebron->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			Tenebron->GetAIInterface()->m_canMove = true;
			Tenebron->GetAIInterface()->MoveTo( Tenebron ->GetSpawnX(), Tenebron ->GetSpawnY(), Tenebron->GetSpawnZ(), Tenebron->GetSpawnO()); 
		};

		if(aVesperon == true && Vesperon != NULL && Vesperon->isAlive())
		{
			Vesperon->GetAIInterface()->MoveTo( Vesperon->GetSpawnX(), Vesperon->GetSpawnY(), Vesperon->GetSpawnZ(), Vesperon->GetSpawnO()); 
		}
		else if(aVesperon == true && Vesperon != NULL && !Vesperon->isAlive()) 
		{
			Vesperon->SetUInt64Value(UNIT_FIELD_HEALTH,_unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
			Vesperon->setDeathState(ALIVE);
			Vesperon->GetAIInterface()->WipeTargetList();
			Vesperon->GetAIInterface()->WipeHateList();
			Vesperon->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			Vesperon->GetAIInterface()->m_canMove = true;
			Vesperon->GetAIInterface()->MoveTo( Vesperon->GetSpawnX(), Vesperon->GetSpawnY(), Vesperon->GetSpawnZ(), Vesperon->GetSpawnO()); 
		};
		
		if(aShadron == true && Shadron != NULL && Shadron->isAlive())
		{
			Shadron->GetAIInterface()->MoveTo( Shadron->GetSpawnX(), Shadron->GetSpawnY(), Shadron->GetSpawnZ(), Shadron->GetSpawnO()); 
		}
		else if(aShadron == true && Shadron != NULL && Shadron->isAlive())
		{
			Shadron->SetUInt64Value(UNIT_FIELD_HEALTH,_unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
			Shadron->setDeathState(ALIVE);
			Shadron->GetAIInterface()->WipeTargetList();
			Shadron->GetAIInterface()->WipeHateList();
			Shadron->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			Shadron->GetAIInterface()->m_canMove = true;
			Shadron->GetAIInterface()->MoveTo( Shadron->GetSpawnX(), Shadron->GetSpawnY(), Shadron->GetSpawnZ(), Shadron->GetSpawnO()); 
		}
		
		if((aTenebron == true || aVesperon == true || aShadron == true) && mDrakeTimer != INVALIDATE_TIMER)
			RemoveTimer(mDrakeTimer);

		ParentClass::OnCombatStop( pTarget );
    };

	void Destroy()
	{
		delete this;
	};

	int32 mLavaTimer, mAddTimer, mDrakeTimer;
	bool aTenebron, aVesperon, aShadron, CanMove;
	CreaturePointer pTsunamiOne, pTsunamiTwo, pTsunamiThree;

	SpellDesc* mFlame;
};	

/*class TenebronAI : public MoonScriptBossAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(TenebronAI, MoonScriptBossAI);
	TenebronAI(CreaturePointer pCreature) : MoonScriptBossAI(pCreature)
	{
		
	}

	void OnDied(UnitPointer pKiller)
	{
		Emote( "Such is the price... of failure...", Text_Yell, 14107);
		
		RemoveAIUpdateEvent();
		ParentClass::OnDied(pKiller);
	}
};

class VesperonAI : public MoonScriptBossAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(VesperonAI, MoonScriptBossAI);
	VesperonAI(CreaturePointer pCreature) : MoonScriptBossAI(pCreature)
	{
	}

	void OnDied(UnitPointer pKiller)
	{
		Emote( "Such is the price... of failure...", Text_Yell, 14107);
		
		RemoveAIUpdateEvent();
		ParentClass::OnDied(pKiller);
	}
};

class ShadronAI : public MoonScriptBossAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(ShadronAI, MoonScriptBossAI);
	ShadronAI(CreaturePointer pCreature) : MoonScriptBossAI(pCreature)
	{
	}

	void OnDied(UnitPointer pKiller)
	{
		Emote( "Such is the price... of failure...", Text_Yell, 14107);
		
		RemoveAIUpdateEvent();
		ParentClass::OnDied(pKiller);
	}
};*/

class TsunamiAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(TsunamiAI, MoonScriptBossAI);
	TsunamiAI(CreaturePointer pCreature) : MoonScriptBossAI(pCreature)
	{
		SetAIUpdateFreq(1000);
	}
	
	void AIUpdate()
	{
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(TSUNAMI), true);
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(TSUNAMI_VISUAL), true);

		ParentClass::AIUpdate();
	}
	
	void OnLoad()
	{
		SetFlyMode(true);
		SetCanEnterCombat(false);
		
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		
		ParentClass::OnLoad();
	}
};	

class TornadoAI : public MoonScriptBossAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(TornadoAI, MoonScriptBossAI);
	TornadoAI(CreaturePointer pCreature) : MoonScriptBossAI(pCreature)
	{
		RegisterAIUpdateEvent(1000);
	}

	void OnLoad()
	{
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		SetCanEnterCombat(false);

		ParentClass::OnLoad();
	}
	void AIUpdate()
	{
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(TORNADO_SPELL), true);

		ParentClass::AIUpdate();
	}
};

class TwilightPortal : public GameObjectAIScript
{
public:
	TwilightPortal(GameObjectPointer goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObjectPointer GO) { return new TwilightPortal(GO); }

	void OnActivate(PlayerPointer pPlayer) 
	{
		pPlayer->SetPhase( 2 );
	}
}; 

void SetupTheObsidianSanctum(ScriptMgr * mgr)
{
	//////////////////////////////////////////////////////////////////////////////////////////
	///////// Mobs
	//////////////////////////////////////////////////////////////////////////////////////////
	///////// Bosses
	mgr->register_creature_script(BOSS_SARTHARION, &SartharionAI::Create);
	mgr->register_creature_script(TSUNAMI, &TsunamiAI::Create);
	mgr->register_creature_script(TORNADO, &TornadoAI::Create);

	//GO Scripts
	//mgr->register_gameobject_script(193988, &TwilightPortal::Create);
};

