#include "StdAfx.h"
#include "Setup.h"
#include "Base.h"

////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Anomalus ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#define CN_ANOMALUS 26763
#define CN_CHAOTIC_RIFT 26918
#define CN_CRAZED_MANA_WRAITH 26746

#define SPARK 47751 
#define SPARK_HC 57062
#define CHAOTIC_ENERGY_BURST 47688

class AnomalusAI : public MoonScriptBossAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(AnomalusAI, MoonScriptBossAI);
	AnomalusAI(CreaturePointer pCreature) : MoonScriptBossAI(pCreature)
    {
		if(IsHeroic())
			AddSpell(SPARK_HC, Target_RandomPlayer, 80, 0, 3);
		else
			AddSpell(SPARK, Target_RandomPlayer, 80, 0, 3);
    };
    
    void OnCombatStart(UnitPointer mTarget)
    {
		Emote("Chaos beckons.", Text_Yell, 0);
		summon = 0;

		ParentClass::OnCombatStart(mTarget);
    };

    void OnDied(UnitPointer  mKiller)
    {
		Emote("Of course.", Text_Yell, 0);

		ParentClass::OnDied(mKiller);
    };

    void AIUpdate()
    {
		if((GetHealthPercent() <= 76 && summon == 0) 
		|| (GetHealthPercent() <= 51 && summon == 2) 
		|| (GetHealthPercent() <= 25 && summon == 4))
			summon += 5;
		
		if(summon == 1 || summon == 3 || summon == 5) 
			Summon();

		ParentClass::AIUpdate();
    };

	void Summon()
	{
		Emote("Reality... unwoven.", Text_Yell, 0);

		float x = _unit->GetPositionX() + 6.5f;
		SpawnCreature( CN_CHAOTIC_RIFT, x, _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), true);
		_unit->CastSpell( _unit, 47748, true); // me immune

		/*CreaturePointer ChaoticRift = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( x, _unit->GetPositionY(), _unit->GetPositionZ(), CN_CHAOTIC_RIFT);
		
		if(ChaoticRift)
			ChaoticRift->Root();*/

		summon += 1;
	};

	void OnCombatStop(UnitPointer pTarget)
	{
		MoveToSpawnOrigin();
		ParentClass::OnCombatStop(pTarget);
	};

	void Destroy()
    {
		delete this;
    };

	int			summon;
};

class ChaoticRiftAI : public MoonScriptBossAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(ChaoticRiftAI, MoonScriptBossAI);
	ChaoticRiftAI(CreaturePointer pCreature) : MoonScriptBossAI(pCreature)
    {
		AddSpell(CHAOTIC_ENERGY_BURST, Target_RandomPlayer, 80, 0, 2);

		//SetCanMove(false);
		mDespawnTimer = AddTimer(40000);
    };

	void OnCombatStart(UnitPointer pTarget)
	{
		SetCanMove(false);

		ParentClass::OnCombatStart(pTarget);
	};
	void OnLoad()
	{
		float x = _unit->GetPositionX() + 1.5f;
		SpawnCreature( CN_CRAZED_MANA_WRAITH, x, _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), true);

		float x2 = x - 3.5f;
		SpawnCreature( CN_CRAZED_MANA_WRAITH, x2, _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), true);

		ParentClass::OnLoad();
	};

	void AIUpdate()
	{
		if(IsTimerFinished(mDespawnTimer))
			Despawn( 1000, 0);

		ParentClass::AIUpdate();
	};

    void OnDied(UnitPointer  mKiller)
    {
		CreaturePointer Anomalus = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), CN_ANOMALUS);
		Anomalus->RemoveAura(47748);
		_unit->Despawn( 1000, 0);

		float x = _unit->GetPositionX() + 1.5f;
		SpawnCreature( CN_CRAZED_MANA_WRAITH, x, _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), true);

		float x2 = x - 3.5f;
		SpawnCreature( CN_CRAZED_MANA_WRAITH, x2, _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), true);

		ParentClass::OnDied(mKiller);
    };

	void OnCombatStop(UnitPointer pTarget)
	{
		_unit->Despawn( 1000, 0);

		ParentClass::OnCombatStop(pTarget);
	};

	void Destroy()
    {
		delete this;
    };

	uint32			mDespawnTimer;
};

class CraziedManaWrathAI : public MoonScriptBossAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(CraziedManaWrathAI, MoonScriptBossAI);
	CraziedManaWrathAI(CreaturePointer pCreature) : MoonScriptBossAI(pCreature){};

	void OnCombatStop(UnitPointer pTarget)
	{
		_unit->Despawn( 1000, 0);

		ParentClass::OnCombatStop(pTarget);
	};

	void OnDied(UnitPointer  mKiller)
    {
		_unit->Despawn( 1000, 0);

		ParentClass::OnDied(mKiller);
	};

	void Destroy()
    {
		delete this;
    };
};


////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Grand Magus Telestra //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

struct Spawns
{
	float x;
	float y;
	float z;
	float o;
};

static Spawns FormSpawns[] =	
{
	{ 494.726990f, 89.128799f, -15.941300f, 6.021390f },
	{ 495.006012f, 89.328102f, -16.124609f, 0.027486f },
	{ 504.798431f, 102.248375f, -16.124609f, 4.629921f }
};

#define CN_TELESTRA 26731
#define CN_TELESTRA_FROST 26930
#define CN_TELESTRA_FIRE 26928
#define CN_TELESTRA_ARCANE 26929

#define GRAVITY_WELL 47756

// Normal mode spells 
#define ICE_NOVA 47772
#define FIREBOMB 47773

// Heroic mode spells
#define ICE_NOVA_HC 56935
#define FIREBOMB_HC 56934

// Arcane spells
#define CRITTER 47731
#define TIME_STOP 47736

// Fire
#define FIRE_BLAST 47721
#define FIRE_BLAST_HC 56939
#define SCORCH 47723
#define SCORCH_HC 56938

// Frost 
#define BLIZZARD 47727
#define BLIZZARD_HC 56936
#define ICE_BARB 47729
#define ICE_BARB_HC 56937

class TelestraBossAI : public MoonScriptBossAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(TelestraBossAI, MoonScriptBossAI);
	TelestraBossAI(CreaturePointer pCreature) : MoonScriptBossAI(pCreature)
	{
		if(IsHeroic())
		{
			AddSpell(ICE_NOVA_HC, Target_Self, 20, 2, 16);
			AddSpell(FIREBOMB_HC, Target_RandomPlayer, 35, 1.5f, 5);
			AddSpell(GRAVITY_WELL, Target_Self, 6, 0.5f, 30);
		}
		else
		{
			AddSpell(ICE_NOVA, Target_Self, 20, 2, 16);
			AddSpell(FIREBOMB, Target_RandomPlayer, 35, 1.5f, 5);
			AddSpell(GRAVITY_WELL, Target_Self, 6, 0.5f, 30);
		};

		SetCanMove(false);
		SetAIUpdateFreq(1000);

		AddEmote(Event_OnCombatStart, "You know what they say about curiosity. ", Text_Yell, 0);
		AddEmote(Event_OnTargetDied, "Death becomes you!", Text_Yell, 0);
		AddEmote(Event_OnDied, "Damn the... luck.", Text_Yell, 0);
	};

	void AIUpdate()
	{
		if(GetHealthPercent() <= 50 && GetPhase() == 1)
		{
			const char * Text;

			uint32 Random = rand()%2;
			switch (Random)	
			{
				case 0:
					Text =  "I'll give you more than you can handle.";
					break;
				case 1:
					Text = "There's plenty of me to go around.";
					break;
			}
			
			if(Text != NULL)
				Emote(Text, Text_Yell, 0);

			SetPhase(2);
			mScale = GetScale();
			SetScale(0);
			SetCanMove(false); // lemme hijack a little 
			SetCanEnterCombat(false); 
			/*SetAllowMelee(false);
			SetAllowRanged(false);
			SetAllowSpell(false);
			SetAllowTargeting(false);*/

			for(int i = 0; i < 3; ++i)
				SpawnCreature(CN_TELESTRA_FIRE + i, FormSpawns[i].x, FormSpawns[i].y, FormSpawns[i].z, FormSpawns[i].o, true);
		};

		if(GetPhase() == 3)
		{
			Emote("Now to finish the job!", Text_Yell, 0);
			SetScale(mScale);
			SetCanEnterCombat(true); 
			/*SetAllowMelee(true);
			SetAllowRanged(true);
			SetAllowSpell(true);
			SetAllowTargeting(true);*/
		};

		ParentClass::AIUpdate();
	};

	
	void Destroy()
    {
		delete this;
    };

	float	mScale;
};

class TelestraFireAI : public MoonScriptBossAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(TelestraFireAI, MoonScriptBossAI);
	TelestraFireAI(CreaturePointer pCreature) : MoonScriptBossAI(pCreature)
	{
		if(IsHeroic())
		{
			AddSpell(FIRE_BLAST_HC, Target_RandomPlayer, 30, 0, 14);
			AddSpell(SCORCH_HC, Target_Current, 100, 1, 3);
		}
		else
		{
			AddSpell(FIRE_BLAST, Target_RandomPlayer, 30, 0, 14);
			AddSpell(SCORCH, Target_Current, 100, 1, 3);
		};
		
	};

	void OnLoad()
	{
		SetCanMove(false);
		ParentClass::OnLoad();
	};

	void OnCombatStop(UnitPointer pTarget)
	{
		Despawn(0,0);
		ParentClass::OnCombatStop(pTarget);
	};

	void OnDied(UnitPointer pKiller)
	{
		if( !_unit->GetMapMgr() )
			return;

		MoonScriptCreatureAI* pFrost = (MoonScriptCreatureAI*)GetNearestCreature(CN_TELESTRA_FROST);
		MoonScriptCreatureAI* pArcane = (MoonScriptCreatureAI*)GetNearestCreature(CN_TELESTRA_ARCANE);

		if((!pFrost || !pFrost->IsAlive()) && (!pArcane || !pArcane->IsAlive()))
		{
			pTelestra = (MoonScriptBossAI*)GetNearestCreature(CN_TELESTRA);
			
			if(pTelestra != NULL && pTelestra->IsAlive())
				pTelestra->SetPhase(3);
		};

		Despawn();

		ParentClass::OnDied(pKiller);
	};

	void Destroy()
    {
		delete this;
    };

	MoonScriptBossAI*	pTelestra;
};

class TelestraFrostAI : public MoonScriptBossAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(TelestraFrostAI, MoonScriptBossAI);
	TelestraFrostAI(CreaturePointer pCreature) : MoonScriptBossAI(pCreature)
	{
		if(IsHeroic())
		{
			AddSpell(BLIZZARD_HC, Target_RandomPlayerDestination, 15, 0, 20);
			AddSpell(ICE_BARB_HC, Target_RandomPlayer, 1, 0.5, 2);
		}
		else
		{
			AddSpell(BLIZZARD, Target_RandomPlayerDestination, 15, 0, 20);
			AddSpell(ICE_BARB, Target_RandomPlayer, 1, 0.5, 2);
		};
	};

	void OnLoad()
	{
		SetCanMove(false);
		ParentClass::OnLoad();
	};

	void OnCombatStop(UnitPointer pTarget)
	{
		Despawn(0,0);
		ParentClass::OnCombatStop(pTarget);
	};

	void OnDied(UnitPointer pKiller)
	{
		if( !_unit->GetMapMgr() )
			return;

		MoonScriptCreatureAI* pFire = (MoonScriptCreatureAI*)GetNearestCreature(CN_TELESTRA_FIRE);
		MoonScriptCreatureAI* pArcane = (MoonScriptCreatureAI*)GetNearestCreature(CN_TELESTRA_ARCANE);

		if((!pFire || !pFire->IsAlive()) && (!pArcane || !pArcane->IsAlive()))
		{
			pTelestra = (MoonScriptBossAI*)GetNearestCreature(CN_TELESTRA);
			
			if(pTelestra != NULL && pTelestra->IsAlive())
				pTelestra->SetPhase(3);
		};

		Despawn();

		ParentClass::OnDied(pKiller);
	};

	void Destroy()
    {
		delete this;
    };

	MoonScriptBossAI*	pTelestra;
};

class TelestraArcaneAI : public MoonScriptBossAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(TelestraArcaneAI, MoonScriptBossAI);
	TelestraArcaneAI(CreaturePointer pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(TIME_STOP, Target_Self, 25, 1.5f, 30);
	};

	void OnLoad()
	{
		SetCanMove(false);
		ParentClass::OnLoad();
	};

	void OnCombatStop(UnitPointer pTarget)
	{
		Despawn(0,0);
		ParentClass::OnCombatStop(pTarget);
	};

	void OnDied(UnitPointer pKiller)
	{
		if( !_unit->GetMapMgr() )
			return;

		MoonScriptCreatureAI* pFrost = (MoonScriptCreatureAI*)GetNearestCreature(CN_TELESTRA_FROST);
		MoonScriptCreatureAI* pFire = (MoonScriptCreatureAI*)GetNearestCreature(CN_TELESTRA_FIRE);

		if((!pFrost || !pFrost->IsAlive() ) && (!pFire || !pFire->IsAlive()))
		{
			pTelestra = (MoonScriptBossAI*)GetNearestCreature(CN_TELESTRA);
			
			if(pTelestra != NULL && pTelestra->IsAlive())
				pTelestra->SetPhase(3);
		};

		Despawn();

		ParentClass::OnDied(pKiller);
	};

	void Destroy()
    {
		delete this;
    };

	MoonScriptBossAI*	pTelestra;
};

class WrathGateQuestCinema : public QuestScript
{	
public:
  void OnQuestComplete(PlayerPointer mTarget, QuestLogEntry *qLogEntry)
  {
	if( mTarget == NULL )
		return;

	// send packet for movie
	uint32 id = 14;	
	mTarget->GetSession()->OutPacket(SMSG_TRIGGER_MOVIE, sizeof(uint32), &id);
  }
};

void SetupNexus(ScriptMgr * mgr)
{
	// Anomalus Encounter
    mgr->register_creature_script(CN_ANOMALUS, &AnomalusAI::Create);
	mgr->register_creature_script(CN_CHAOTIC_RIFT, &ChaoticRiftAI::Create);
	mgr->register_creature_script(CN_CRAZED_MANA_WRAITH, &CraziedManaWrathAI::Create);

	// Grand Magus Telestra Encounter 
	mgr->register_creature_script(CN_TELESTRA, &TelestraBossAI::Create);
	mgr->register_creature_script(CN_TELESTRA_ARCANE, &TelestraArcaneAI::Create);
	mgr->register_creature_script(CN_TELESTRA_FROST, &TelestraFrostAI::Create);
	mgr->register_creature_script(CN_TELESTRA_FIRE, &TelestraFireAI::Create);

	QuestScript *WrathGateCinema = (QuestScript*) new WrathGateQuestCinema();
	mgr->register_quest_script(12499, WrathGateCinema);
	mgr->register_quest_script(12500, WrathGateCinema);
}
