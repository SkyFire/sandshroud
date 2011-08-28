/*
 * Sandshroud Hearthstone
 * Copyright (C) 2010 - 2011 Sandshroud <http://www.sandshroud.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"

/// Function pointer holder
pSpellTarget SpellTargetHandler[TOTAL_SPELL_TARGET] =
{
	&Spell::SpellTargetDefault,						// 0
	&Spell::SpellTargetSelf,						// 1
	&Spell::SpellTargetNULL,						// 2 Not handled (Test spells)
	&Spell::SpellTargetInvisibleAOE,				// 3
	&Spell::SpellTargetFriendly,					// 4
	&Spell::SpellTargetPet,							// 5
	&Spell::SpellTargetSingleTargetEnemy,			// 6
	&Spell::SpellTargetNULL,						// 7
	&Spell::SpellTargetCustomAreaOfEffect,			// 8
	&Spell::SpellTargetNULL,						// 9 Not handled (no spells)
	&Spell::SpellTargetNULL,						// 10 Not handled (no spells)
	&Spell::SpellTargetNULL,						// 11 Not handled (Test spells)
	&Spell::SpellTargetNULL,						// 12 Not handled (no spells)
	&Spell::SpellTargetNULL,						// 13 Not handled (no spells)
	&Spell::SpellTargetNULL,						// 14 Not handled (no spells)
	&Spell::SpellTargetAreaOfEffect,				// 15
	&Spell::SpellTargetAreaOfEffect,				// 16
	&Spell::SpellTargetNULL,						// 17 Target is a location
	&Spell::SpellTargetLandUnderCaster,				// 18
	&Spell::SpellTargetNULL,						// 19 Not handled (Test spells)
	&Spell::SpellTargetAllPartyMembersRangeNR,		// 20
	&Spell::SpellTargetSingleTargetFriend,			// 21
	&Spell::SpellTargetAoE,							// 22
	&Spell::SpellTargetSingleGameobjectTarget,		// 23
	&Spell::SpellTargetInFrontOfCaster,				// 24
	&Spell::SpellTargetSingleFriend,				// 25
	&Spell::SpellTargetGameobject_itemTarget,		// 26
	&Spell::SpellTargetPetOwner,					// 27
	&Spell::SpellTargetEnemysAreaOfEffect,			// 28 channeled
	&Spell::SpellTargetTypeTAOE,					// 29
	&Spell::SpellTargetAllyBasedAreaEffect,			// 30
	&Spell::SpellTargetScriptedEffects,				// 31
	&Spell::SpellTargetSummon,						// 32
	&Spell::SpellTargetNearbyPartyMembers,			// 33
	&Spell::SpellTargetNULL,						// 34 Not handled (No spells)
	&Spell::SpellTargetSingleTargetPartyMember,		// 35
	&Spell::SpellTargetScriptedEffects2,			// 36
	&Spell::SpellTargetPartyMember,					// 37
	&Spell::SpellTargetDummyTarget,					// 38
	&Spell::SpellTargetFishing,						// 39
	&Spell::SpellTargetType40,						// 40
	&Spell::SpellTargetTotem,						// 41
	&Spell::SpellTargetTotem,						// 42
	&Spell::SpellTargetTotem,						// 43
	&Spell::SpellTargetTotem,						// 44
	&Spell::SpellTargetChainTargeting,				// 45
	&Spell::SpellTargetSimpleTargetAdd,				// 46
	&Spell::SpellTargetSimpleTargetAdd,				// 47
	&Spell::SpellTargetSimpleTargetAdd,				// 48
	&Spell::SpellTargetSimpleTargetAdd,				// 49
	&Spell::SpellTargetSimpleTargetAdd,				// 50
	&Spell::SpellTargetNULL,						// 51
	&Spell::SpellTargetGameObjectsInArea,			// 52
	&Spell::SpellTargetTargetAreaSelectedUnit,		// 53
	&Spell::SpellTargetInFrontOfCaster2,			// 54
	&Spell::SpellTargetNULL,						// 55 Not handled (Not realy handled by the current spell system)
	&Spell::SpellTarget56,							// 56
	&Spell::SpellTargetTargetPartyMember,			// 57
	&Spell::SpellTargetNULL,						// 58
	&Spell::SpellTargetNULL,						// 59
	&Spell::SpellTargetNULL,						// 60 // scripted target fill..
	&Spell::SpellTargetSameGroupSameClass,			// 61
	&Spell::SpellTargetNULL,						// 62 // targets the priest champion, big range..
	&Spell::SpellTargetScriptedEffects,				// 63 // summon [] creature
	&Spell::SpellTargetSimpleTargetAdd,				// 64 // summon [] creature
	&Spell::SpellTargetSimpleTargetAdd,				// 65 // summon [] creature
	&Spell::SpellTargetSimpleTargetAdd,				// 66 // summon [] creature
	&Spell::SpellTargetSimpleTargetAdd,				// 67 // summon [] creature
	&Spell::SpellTargetNULL,						// 68 Not handled (No spells)
	&Spell::SpellTargetTotem,						// 69 // Spitfire totem
	&Spell::SpellTargetNULL,						// 70 Not handled (No spells)
	&Spell::SpellTargetNULL,						// 71 Not handled (No spells)
	&Spell::SpellTargetSimpleTargetAdd,				// 72 // summon [] creature + trow items
	&Spell::SpellTargetSummon,						// 73
	&Spell::SpellTargetNULL,						// 74
	&Spell::SpellTargetNULL,						// 75
	&Spell::SpellTargetAllTargetsInArea,			// 76
	&Spell::SpellTargetSingleTargetEnemy,			// 77
	&Spell::SpellTargetNULL,						// 78
	&Spell::SpellTargetNULL,						// 79
	&Spell::SpellTargetNULL,						// 80
	&Spell::SpellTargetNULL,						// 81
	&Spell::SpellTargetNULL,						// 82
	&Spell::SpellTargetNULL,						// 83
	&Spell::SpellTargetNULL,						// 84
	&Spell::SpellTargetNULL,						// 85
	&Spell::SpellTargetNULL,						// 86
	&Spell::SpellTargetAreaOfEffect87,				// 87
	&Spell::SpellTargetNULL,						// 88
	&Spell::SpellTargetProjectile,					// 89 Fill Targets Trajectory
	&Spell::SpellTargetNULL,						// 90
	&Spell::SpellTargetNULL,						// 91
	&Spell::SpellTargetNULL,						// 92
	&Spell::SpellTargetNULL,						// 93
	&Spell::SpellTargetVehicle,						// 94
	&Spell::SpellTargetVehicleDriver,				// 95
	&Spell::SpellTargetVehiclePassenger,			// 96
	&Spell::SpellTargetVehiclePassenger1,			// 97
	&Spell::SpellTargetVehiclePassenger2,			// 98
	&Spell::SpellTargetVehiclePassenger3,			// 99
	&Spell::SpellTargetVehiclePassenger4,			// 100
	&Spell::SpellTargetVehiclePassenger5,			// 101
	&Spell::SpellTargetVehiclePassenger6,			// 102
	&Spell::SpellTargetVehiclePassenger7,			// 103
	&Spell::SpellTargetInFrontOfCaster,				// 104
	&Spell::SpellTargetNULL,						// 105
	&Spell::SpellTargetNULL,						// 106
	&Spell::SpellTargetNULL,						// 107
	&Spell::SpellTargetNULL,						// 108
	&Spell::SpellTargetNULL,						// 109
	&Spell::SpellTargetNULL,						// 110
	// all 110 > n spelltargettype's are from test spells
};

/// Fill the target map with the targets
/// the targets are specified with numbers and handled accordingly
void Spell::FillTargetMap(uint32 i)
{
	uint32 cur;

	uint32 TypeA = GetSpellProto()->EffectImplicitTargetA[i];
	uint32 TypeB = GetSpellProto()->EffectImplicitTargetB[i];

	// if all secondary targets are 0 then use only primary targets
	if(!TypeB)
	{
		if(TypeA < TOTAL_SPELL_TARGET)
			(this->*SpellTargetHandler[TypeA])(i, 0);		//0=A
		else
		{
			if(sLog.IsOutDevelopement())
				printf("[SPELL][TARGET] Unknown Target Type  %u in spell %u\n", TypeA, GetSpellProto()->Id);
			else
				sLog.outSpellDebug("[SPELL][TARGET] Unknown Target Type  %u in spell %u", TypeA, GetSpellProto()->Id);
		}

		return;
	}

	// if all primary targets are 0 then use only secondary targets
	if(!TypeA)
	{
		if(TypeB < TOTAL_SPELL_TARGET)
			(this->*SpellTargetHandler[TypeB])(i, 1);		//1=B
		else
		{
			if(sLog.IsOutDevelopement())
				printf("[SPELL][TARGET] Unknown Target Type  %u in spell %u\n", TypeB, GetSpellProto()->Id);
			else
				sLog.outSpellDebug("[SPELL][TARGET] Unknown Target Type  %u in spell %u", TypeB, GetSpellProto()->Id);
		}

		return;
	}

	// j = 0
	cur = GetSpellProto()->EffectImplicitTargetA[i];
	if (cur < TOTAL_SPELL_TARGET)
	{
		(this->*SpellTargetHandler[cur])(i,0);	//0=A
	}
	else
	{
		if(sLog.IsOutDevelopement())
			printf("[SPELL][TARGET] Unknown Target Type  %u in spell %u\n", cur, GetSpellProto()->Id);
		else
			sLog.outSpellDebug("[SPELL][TARGET] Unknown Target Type  %u in spell %u", cur, GetSpellProto()->Id);
	}

	// j = 1
	cur = GetSpellProto()->EffectImplicitTargetB[i];
	if (cur < TOTAL_SPELL_TARGET)
	{
		(this->*SpellTargetHandler[cur])(i,1);	//1=B
	}
	else
	{
		if(sLog.IsOutDevelopement())
			printf("[SPELL][TARGET] Unknown Target Type  %u in spell %u\n", cur, GetSpellProto()->Id);
		else
			sLog.outSpellDebug("[SPELL][TARGET] Unknown Target Type  %u in spell %u", cur, GetSpellProto()->Id);
	}
}

void Spell::SpellTargetNULL(uint32 i, uint32 j)
{
	if(GetSpellProto()->EffectImplicitTargetA[j] != 0 && GetSpellProto()->EffectImplicitTargetB[j] != 0)
	{
		if(sLog.IsOutDevelopement())
			printf("[SPELL][TARGET] Unhandled target typeA: %u typeB: %u\n", GetSpellProto()->EffectImplicitTargetA[j], GetSpellProto()->EffectImplicitTargetB[j]);
		else
			sLog.outSpellDebug("[SPELL][TARGET] Unhandled target typeA: %u typeB: %u", GetSpellProto()->EffectImplicitTargetA[j], GetSpellProto()->EffectImplicitTargetB[j]);
	}
}

/// Spell Target Handling for type 0: Default targeting
void Spell::SpellTargetDefault(uint32 i, uint32 j)
{
	if(j==0 || (m_caster->IsPet() && j==1))
	{
		if(m_targets.m_unitTarget)
			_AddTargetForced(m_targets.m_unitTarget, i);
		else if(m_targets.m_itemTarget)
			_AddTargetForced(m_targets.m_itemTarget, i);
		else if( GetSpellProto()->Effect[i] == SPELL_EFFECT_ADD_FARSIGHT ||
				 GetSpellProto()->Effect[i] == SPELL_EFFECT_SUMMON_DEMON )
			_AddTargetForced(m_caster->GetGUID(), i);
	}
}

/// Spell Target Handling for type 1: Self Target + in moon skin form party member in radius
void Spell::SpellTargetSelf(uint32 i, uint32 j)
{
	_AddTargetForced(m_caster->GetGUID(), i);
}
void Spell::SpellTargetInvisibleAOE(uint32 i, uint32 j)
{
	FillSpecifiedTargetsInArea(i,m_targets.m_destX,m_targets.m_destY,m_targets.m_destZ,GetRadius(i),1); //TARGET_SPEC_INVISIBLE);
}

/// Spell Target Handling for type 4: Target is holder of the aura
void Spell::SpellTargetFriendly(uint32 i, uint32 j)
{
	// O fuck we are contagious...
	// this off course is not tested yet.
	if (p_caster)
		_AddTargetForced(p_caster->GetGUID(), i);
}

/// Spell Target Handling for type 5: Target: Pet
void Spell::SpellTargetPet(uint32 i, uint32 j)
{
	if(p_caster)
	{
		if( p_caster->GetSummon() != NULL )
			_AddTargetForced( p_caster->GetSummon()->GetGUID(), i );
	}
}

/// Spell Target Handling for type 6 and 77: Single Target Enemy (grep thinks 77 fits in 6)
void Spell::SpellTargetSingleTargetEnemy(uint32 i, uint32 j)
{
	if(!m_caster->IsInWorld())
		return;

	Unit* pTarget = m_caster->GetMapMgr()->GetUnit(m_targets.m_unitTarget);
	if(!pTarget)
		return;

	if(GetSpellProto()->TargetCreatureType)
	{
		if(!(1<<(pTarget->GetCreatureType()-1) & GetSpellProto()->TargetCreatureType))
				return;
	}

	if(p_caster && pTarget != p_caster)
	{
		// this is mostly used for things like duels
		if(pTarget->IsPlayer() && !isAttackable(p_caster, pTarget, false))
		{
			cancastresult = SPELL_FAILED_BAD_TARGETS;
			return;
		}
	}

	// magnet!!!!!
	if( pTarget->IsPlayer() && TO_PLAYER(pTarget)->m_magnetAura != NULL && m_magnetTarget == NULL )
	{
		if(!GetSpellProto()->is_melee_spell && GetType() == SPELL_DMG_TYPE_MAGIC )
		{
			// redirect it to the magnet
			m_magnetTarget = TO_PLAYER(pTarget)->m_magnetAura->GetUnitCaster();

			// clear the magnet aura
			TO_PLAYER(pTarget)->m_magnetAura->Remove();
		}
	}

	if( m_magnetTarget != NULL )
	{
		// if the target exists, shoot it at him.
		if( m_magnetTarget != NULL && m_magnetTarget->IsInWorld() && !m_magnetTarget->isDead() )
			pTarget = m_magnetTarget;
	}

	_AddTarget(pTarget, i);

	if(GetSpellProto()->EffectChainTarget[i])
	{
		uint32 jumps = GetSpellProto()->EffectChainTarget[i]-1;
		float range = GetMaxRange(dbcSpellRange.LookupEntry(GetSpellProto()->rangeIndex));//this is probably wrong
		range *= range;

		uint32 placeholder = 0;
		vector<Object*> ChainTargetContainer;
		unordered_set<Object*>::iterator itr;
		for( itr = pTarget->GetInRangeSetBegin(); itr != pTarget->GetInRangeSetEnd(); itr++ )
		{
			if((*itr)->GetGUID() == m_targets.m_unitTarget)
				continue;
			if( !((*itr)->IsUnit()) || !TO_UNIT(*itr)->isAlive())
				continue;

			if(IsInrange(pTarget->GetPositionX(),pTarget->GetPositionY(),pTarget->GetPositionZ(),(*itr),range))
			{
				if(isAttackable(u_caster,TO_UNIT(*itr)))
				{
					ChainTargetContainer.push_back(*itr);
					++placeholder;
				}
			}
		}

		Object *chaintarget = NULL;
		while(jumps && ChainTargetContainer.size())
		{
			placeholder = (rand()%ChainTargetContainer.size());
			chaintarget = ChainTargetContainer.at(placeholder);
			if(chaintarget == NULL)
				continue;

			_AddTarget(TO_UNIT(chaintarget), i);
			ChainTargetContainer.erase(ChainTargetContainer.begin()+placeholder);
			jumps--;
		}
		ChainTargetContainer.clear();
	}
}

/// Spell Target Handling for type 8:  related to Chess Move (DND), Firecrackers, Spotlight, aedm, Spice Mortar
/// Seems to be some kind of custom area of effect... Scripted... or something like that
void Spell::SpellTargetCustomAreaOfEffect(uint32 i, uint32 j)
{
	// This should be good enough for now
	FillAllTargetsInArea(i,m_targets.m_destX,m_targets.m_destY,m_targets.m_destZ,GetRadius(i));
}

/// Spell Target Handling for type 15 / 16: All Enemies in Area of Effect (instant)
void Spell::SpellTargetAreaOfEffect(uint32 i, uint32 j)
{
	FillAllTargetsInArea(i,m_targets.m_destX,m_targets.m_destY,m_targets.m_destZ,GetRadius(i));
}

/// Spell Target Handling for type 18: Land under caster
void Spell::SpellTargetLandUnderCaster(uint32 i, uint32 j) /// I don't think this is the correct name for this one
{
	if(	GetSpellProto()->Effect[i] != SPELL_EFFECT_SUMMON_DEMON &&
		GetSpellProto()->Effect[i] != SPELL_EFFECT_SUMMON_OBJECT_WILD &&
		GetSpellProto()->Effect[i] != SPELL_EFFECT_SUMMON_OBJECT_SLOT1 &&
		GetSpellProto()->Effect[i] != SPELL_EFFECT_SUMMON_OBJECT_SLOT2 &&
		GetSpellProto()->Effect[i] != SPELL_EFFECT_SUMMON_OBJECT_SLOT3 &&
		GetSpellProto()->Effect[i] != SPELL_EFFECT_SUMMON_OBJECT_SLOT4 )
		FillAllTargetsInArea(i, m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), GetRadius(i));
	else
		_AddTargetForced(m_caster->GetGUID(), i);
}

/// Spell Target Handling for type 18: All Party Members around the Caster in given range NOT RAID
void Spell::SpellTargetAllPartyMembersRangeNR(uint32 i, uint32 j)
{
	Player* p = p_caster;

	if( p == NULL )
	{
		if( TO_CREATURE( u_caster)->IsTotem() )
			p = TO_PLAYER( TO_CREATURE(u_caster)->GetSummonOwner());
		else if( u_caster->IsPet() && TO_PET( u_caster )->GetPetOwner() )
			p = TO_PET( u_caster )->GetPetOwner();
	}

	if( p == NULL )
		return;

	float r = GetFriendlyRadius(i);

	r *= r;
	if( IsInrange( m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), p, r ) )
		_AddTargetForced(p->GetGUID(), i);

	SubGroup* subgroup = p->GetGroup() ? p->GetGroup()->GetSubGroup( p->GetSubGroup() ) : 0;

	if( subgroup != NULL )
	{
		p->GetGroup()->Lock();
		for(GroupMembersSet::iterator itr = subgroup->GetGroupMembersBegin(); itr != subgroup->GetGroupMembersEnd(); itr++)
		{
			if(!(*itr)->m_loggedInPlayer || m_caster == (*itr)->m_loggedInPlayer)
				continue;
			if(IsInrange(m_caster->GetPositionX(),m_caster->GetPositionY(),m_caster->GetPositionZ(),(*itr)->m_loggedInPlayer,r))
				_AddTargetForced( (*itr)->m_loggedInPlayer->GetGUID(), i );
		}
		p->GetGroup()->Unlock();
	}
}

/// Spell Target Handling for type 21: Single Target Friend
void Spell::SpellTargetSingleTargetFriend(uint32 i, uint32 j)
{
	Unit* Target;
	if(m_targets.m_unitTarget == m_caster->GetGUID())
		Target = u_caster;
	else
		Target = m_caster->IsInWorld() ? m_caster->GetMapMgr()->GetUnit(m_targets.m_unitTarget) : NULLUNIT;
	if(Target == NULLUNIT)
		return;

	float r = GetFriendlyMaxRange(dbcSpellRange.LookupEntry(GetSpellProto()->rangeIndex));
	if(IsInrange(m_caster, Target, r*r))
		_AddTargetForced(Target->GetGUID(), i);
}

/// Spell Target Handling for type 22: Enemy Targets around the Caster//changed party members around you
/// place around the target / near the target //targeted Area effect
void Spell::SpellTargetAoE(uint32 i, uint32 j) // something special
// grep: this is *totally* broken. AoE only attacking friendly party members and self
// is NOT correct. // not correct at all:P
{
	FillAllTargetsInArea(i,m_caster->GetPositionX(),m_caster->GetPositionY(),m_caster->GetPositionZ(),GetRadius(i));
}

/// Spell Target Handling for type 23: Gameobject Target
void Spell::SpellTargetSingleGameobjectTarget(uint32 i, uint32 j)
{
	_AddTargetForced(m_targets.m_unitTarget, i);
}

/// Spell Target Handling for type 24: Targets in Front of the Caster
void Spell::SpellTargetInFrontOfCaster(uint32 i, uint32 j)
{
	unordered_set< Object* >::iterator itr,itr2;

	if( GetSpellProto()->cone_width == 0.0f )
	{
		for( itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd();)
		{
			itr2 = itr++;
			if((*itr2)->IsUnit() && !(TO_UNIT(*itr2)->isAlive()))
				continue;

			//is Creature in range
			if(m_caster->isInRange((*itr2),GetRadius(i)))
			{
				if(m_caster->isTargetInFront(*itr2))
				{
					if((*itr2)->IsUnit())
					{
						if(isAttackable(u_caster, (*itr2)))
							_AddTarget(TO_UNIT(*itr2), i);
					}
					else
						_AddTargetForced((*itr2)->GetGUID(), i);
				}
			}
		}
	}
	else
	{
		for( itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd();)
		{
			itr2 = itr++;
			if((*itr2)->IsUnit() && !(TO_UNIT(*itr2)->isAlive()))
				continue;

			//is Creature in range
			if(m_caster->isInArc(*itr2, GetSpellProto()->cone_width))
			{
				if(m_caster->isTargetInFront((*itr2)))
				{
					if((*itr2)->IsUnit())
					{
						if(isAttackable(u_caster, (*itr2)))
							_AddTarget(TO_UNIT(*itr2), i);
					}
					else
						_AddTargetForced((*itr2)->GetGUID(), i);
				}
			}
		}
	}
}

/// Spell Target Handling for type 25: Single Target Friend	 // Used o.a. in Duel
void Spell::SpellTargetSingleFriend(uint32 i, uint32 j)
{
	_AddTargetForced(m_targets.m_unitTarget, i);
}

/// Spell Target Handling for type 26: unit target/Item Target
/// game object and item related... research pickpocket stuff
void Spell::SpellTargetGameobject_itemTarget(uint32 i, uint32 j)
{
	if( m_targets.m_unitTarget )
		_AddTargetForced(m_targets.m_unitTarget, i);

	if( m_targets.m_itemTarget )
		_AddTargetForced(m_targets.m_itemTarget, i);
}

/// Spell Target Handling for type 27: target is owner of pet
void Spell::SpellTargetPetOwner(uint32 i, uint32 j)
{
	if( u_caster->IsPet() && TO_PET(u_caster)->GetPetOwner() )
		_AddTargetForced( TO_PET(u_caster)->GetPetOwner()->GetGUID(), i );
}

/// this is handled in DO
/// Spell Target Handling for type 28: All Enemies in Area of Effect(Blizzard/Rain of Fire/volley) channeled
void Spell::SpellTargetEnemysAreaOfEffect(uint32 i, uint32 j)
{
	FillAllTargetsInArea(i,m_targets.m_destX,m_targets.m_destY,m_targets.m_destZ,GetRadius(i));
}

// all object around the the caster / object
/*
mysql> select id,name from spell where EffectImplicitTargetb1 = 29;
+-------+-----------------------------------------------+
| 23467 | Tower Buff									|
| 32087 | Putrid Cloud								  |
| 34378 | Thrall Calls Thunder						  |
| 35487 | Seed of Revitalization Lightning Cloud Visual |
| 36037 | Rina's Bough Lightning Cloud Visual		   |
+-------+-----------------------------------------------+
*/
/// Spell Target Handling for type 29: all object around the the caster / object (so it seems)
void Spell::SpellTargetTypeTAOE(uint32 i, uint32 j)
{
	Unit* Target = m_caster->GetMapMgr()->GetUnit(m_targets.m_unitTarget);
	if( Target == NULL )
		return;

	// tranquility
	if( u_caster != NULL && GetSpellProto()->NameHash == SPELL_HASH_TRANQUILITY )
		_AddTargetForced(u_caster->GetGUID(), i);
	else
	{
		FillAllTargetsInArea( (LocationVector&)Target->GetPosition(), i );
	}
}

void Spell::SpellTargetPositionOfTarget(uint32 i, uint32 j)
{
	// Something with AOE. Who knows?
}

/// Spell Target Handling for type 30: PBAE Party Based Area Effect
void Spell::SpellTargetAllyBasedAreaEffect(uint32 i, uint32 j)
{
	/* Description
	We take the selected party member(also known as target), then we get a list of all the party members in the area

	// Used in
	26043 -> Battle Shout
	*/
	FillAllFriendlyInArea(i,m_caster->GetPositionX(),m_caster->GetPositionY(),m_caster->GetPositionZ(),GetFriendlyRadius(i));
}

/// Spell Target Handling for type 31: related to scripted effects
void Spell::SpellTargetScriptedEffects(uint32 i, uint32 j)
{
	if( GetSpellProto()->NameHash == SPELL_HASH_WILD_GROWTH || GetSpellProto()->NameHash == SPELL_HASH_CIRCLE_OF_HEALING )
	{
		if( p_caster == NULL || !p_caster->IsInWorld() )
			return;

		if( p_caster->GetGroup() )
		{
			uint32 TargetCount = 0;
			p_caster->GetGroup()->Lock();
			for(uint32 x = 0; x < p_caster->GetGroup()->GetSubGroupCount(); ++x)
			{
				if( TargetCount == 5 )
					break;
				for(GroupMembersSet::iterator itr = p_caster->GetGroup()->GetSubGroup( x )->GetGroupMembersBegin(); itr != p_caster->GetGroup()->GetSubGroup( x )->GetGroupMembersEnd(); itr++)
				{
					if((*itr)->m_loggedInPlayer && TargetCount != 5)
					{
						Player* p_target = (*itr)->m_loggedInPlayer;
						if( p_caster->GetDistance2dSq( p_target ) <= 225 ) // both spells have 15yd range, change in future if needed
						{
							_AddTargetForced( (*itr)->m_loggedInPlayer->GetGUID(), i );
							TargetCount += 1;
						}
					}
				}
			}
			p_caster->GetGroup()->Unlock();
		}
	}
	else
		FillAllTargetsInArea(i,m_targets.m_destX,m_targets.m_destY,m_targets.m_destZ,GetRadius(i));
}

/// Spell Target Handling for type 32 / 73: related to summoned pet or creature
void Spell::SpellTargetSummon(uint32 i, uint32 j)
{// Minion Target
	if(m_caster->GetUInt64Value(UNIT_FIELD_SUMMON) == 0)
		_AddTargetForced(m_caster->GetGUID(), i);
	else
		_AddTargetForced(m_caster->GetUInt64Value(UNIT_FIELD_SUMMON), i);
}

/// Spell Target Handling for type 33: Party members of totem, inside given range
void Spell::SpellTargetNearbyPartyMembers(uint32 i, uint32 j)
{
	// this implementation is wrong.... this one is for totems
	if( u_caster != NULL )
	{
		if( u_caster->GetTypeId()==TYPEID_UNIT)
		{
			if( TO_CREATURE( u_caster )->IsTotem() )
			{
				float r = GetRadius(i);
				r *= r;

				Player* p = TO_PLAYER( TO_CREATURE(u_caster)->GetSummonOwner());
				if( p == NULL)
					return;

				if(IsInrange(m_caster->GetPositionX(),m_caster->GetPositionY(),m_caster->GetPositionZ(),p,r))
					_AddTargetForced(p->GetGUID(), i);

				SubGroup * pGroup = p->GetGroup() ?
					p->GetGroup()->GetSubGroup(p->GetSubGroup()) : 0;

				if(pGroup)
				{
					p->GetGroup()->Lock();
					for(GroupMembersSet::iterator itr = pGroup->GetGroupMembersBegin();
						itr != pGroup->GetGroupMembersEnd(); itr++)
					{
						if(!(*itr)->m_loggedInPlayer || p == (*itr)->m_loggedInPlayer)
							continue;
						if(IsInrange(m_caster->GetPositionX(),m_caster->GetPositionY(),m_caster->GetPositionZ(),(*itr)->m_loggedInPlayer,r))
							_AddTargetForced((*itr)->m_loggedInPlayer->GetGUID(), i);
					}
					p->GetGroup()->Unlock();
				}
			}
		}
	}
}

/// Spell Target Handling for type 35: Single Target Party Member (if not in party then the target can not be himself)
/// this one requeres more research
void Spell::SpellTargetSingleTargetPartyMember(uint32 i, uint32 j)
{
	if(!m_caster->IsInWorld())
		return;

	Unit* Target = m_caster->GetMapMgr()->GetPlayer((uint32)m_targets.m_unitTarget);
	if(!Target)
		return;
	float r=GetFriendlyMaxRange(dbcSpellRange.LookupEntry(GetSpellProto()->rangeIndex));
	if(IsInrange(m_caster->GetPositionX(),m_caster->GetPositionY(),m_caster->GetPositionZ(),Target,r*r))
		_AddTargetForced(m_targets.m_unitTarget, i);
}

/// Spell Target Handling for type 36: these targets are scripted :s or something.. there seems to be a system...
void Spell::SpellTargetScriptedEffects2(uint32 i, uint32 j)
{

}

/// Spell Target Handling for type 37: all Members of the targets party
void Spell::SpellTargetPartyMember(uint32 i, uint32 j)
{
	if(!m_caster->IsInWorld())
		return;

	// if no group target self
	Player* Target = m_caster->GetMapMgr()->GetPlayer((uint32)m_targets.m_unitTarget);
	if(!Target)
		return;

	SubGroup * subgroup = Target->GetGroup() ?
		Target->GetGroup()->GetSubGroup(Target->GetSubGroup()) : 0;

	if(subgroup)
	{
		Target->GetGroup()->Lock();
		for(GroupMembersSet::iterator itr = subgroup->GetGroupMembersBegin(); itr != subgroup->GetGroupMembersEnd(); itr++)
		{
			if((*itr)->m_loggedInPlayer)
				_AddTargetForced( (*itr)->m_loggedInPlayer->GetGUID(), i );
		}
		Target->GetGroup()->Unlock();
	}
	else
		_AddTargetForced(Target->GetGUID(), i);
}

/// Spell Target Handling for type 38: Dummy Target (Server-side script effect)
void Spell::SpellTargetDummyTarget(uint32 i, uint32 j)
{
	//TargetsList *tmpMap=&m_targetUnits[i];
	if( GetSpellProto()->Id == 51699 )
	{
		if( p_caster )
		{
			Aura* aur = p_caster->FindAura( 52916 );
			if( aur && aur->GetUnitCaster() )
			{
				_AddTargetForced(aur->GetUnitCaster()->GetGUID(), i);
				return;
			}
		}
	}
	else if( GetSpellProto()->Id == 12938 )
	{
		//FIXME:this ll be immortal targets
		FillAllTargetsInArea(i,m_targets.m_destX,m_targets.m_destY,m_targets.m_destZ,GetRadius(i));
	}
	_AddTargetForced(m_caster->GetGUID(), i);
}

/// Spell Target Handling for type 39: Fishing
void Spell::SpellTargetFishing(uint32 i, uint32 j)
{
	_AddTargetForced(m_caster->GetGUID(), i);
}

/// Spell Target Handling for type 40: Activate Object target(probably based on focus)
/// movement proc, like traps.
void Spell::SpellTargetType40(uint32 i, uint32 j)
{

}

/// Spell Target Handling for type 41 / 42 / 43 / 44: Totems
void Spell::SpellTargetTotem(uint32 i, uint32 j)
{
	_AddTargetForced(m_caster->GetGUID(), i);
}

/// Spell Target Handling for type 45: Chain,!!only for healing!! for chain lightning =6
void Spell::SpellTargetChainTargeting(uint32 i, uint32 j)
{
	if( !m_caster->IsInWorld() )
		return;

	//if selected target is party member, then jumps on party
	Unit* firstTarget;

	bool PartyOnly = false;
	float range = GetMaxRange(dbcSpellRange.LookupEntry(GetSpellProto()->rangeIndex));//this is probably wrong,
	//this is cast distance, not searching distance
	range *= range;

	firstTarget = m_caster->GetMapMgr()->GetPlayer((uint32)m_targets.m_unitTarget);
	if( firstTarget && p_caster != NULL )
	{
		if( p_caster->InGroup() )
			if( p_caster->GetSubGroup() == TO_PLAYER( firstTarget )->GetSubGroup() )
				PartyOnly = true;
	}
	else
	{
		firstTarget = m_caster->GetMapMgr()->GetUnit(m_targets.m_unitTarget);
		if(!firstTarget)
			return;
	}

	uint32 jumps = GetSpellProto()->EffectChainTarget[i];
	if(GetSpellProto()->SpellGroupType && u_caster)
		SM_FIValue(u_caster->SM[SMT_ADDITIONAL_TARGET][0],(int32*)&jumps,GetSpellProto()->SpellGroupType);

	_AddTargetForced(firstTarget->GetGUID(), i);
	if(jumps == NULL)
		return;
	jumps--;
	if(PartyOnly)
	{
		GroupMembersSet::iterator itr;
		SubGroup * pGroup = p_caster->GetGroup() ?
			p_caster->GetGroup()->GetSubGroup(p_caster->GetSubGroup()) : 0;

		if(pGroup)
		{
			p_caster->GetGroup()->Lock();
			for(itr = pGroup->GetGroupMembersBegin(); itr != pGroup->GetGroupMembersEnd(); itr++)
			{
				if(!(*itr)->m_loggedInPlayer || (*itr)->m_loggedInPlayer==u_caster || (*itr)->m_loggedInPlayer->GetUInt32Value(UNIT_FIELD_HEALTH) == (*itr)->m_loggedInPlayer->GetUInt32Value(UNIT_FIELD_MAXHEALTH))
					continue;
				if(IsInrange(firstTarget->GetPositionX(),firstTarget->GetPositionY(),firstTarget->GetPositionZ(),(*itr)->m_loggedInPlayer, range))
				{
					_AddTargetForced((*itr)->m_loggedInPlayer->GetGUID(), i);
					if(!--jumps)
					{
						p_caster->GetGroup()->Unlock();
						return;
					}
				}
			}
			p_caster->GetGroup()->Unlock();
		}
	}//find nearby friendly target
	else
	{
		uint32 placeholder = 0;
		vector<Object*> ChainTargetContainer;
		unordered_set<Object* >::iterator itr;
		for( itr = firstTarget->GetInRangeSetBegin(); itr != firstTarget->GetInRangeSetEnd(); itr++ )
		{
			if( !(*itr)->IsUnit() || !TO_UNIT(*itr)->isAlive())
				continue;

			if(IsInrange(firstTarget->GetPositionX(),firstTarget->GetPositionY(),firstTarget->GetPositionZ(),*itr, range))
			{
				if(!isAttackable(u_caster,TO_UNIT(*itr)) && (*itr)->GetUInt32Value(UNIT_FIELD_HEALTH) != (*itr)->GetUInt32Value(UNIT_FIELD_MAXHEALTH))
				{
					ChainTargetContainer.push_back(*itr);
					++placeholder;
				}
			}
		}

		Object *chaintarget = NULL;
		while(jumps && ChainTargetContainer.size())
		{
			placeholder = (rand()%ChainTargetContainer.size());
			chaintarget = ChainTargetContainer.at(placeholder);
			if(chaintarget == NULL)
				continue;

			_AddTarget(TO_UNIT(chaintarget), i);
			ChainTargetContainer.erase(ChainTargetContainer.begin()+placeholder);
			jumps--;
		}
		ChainTargetContainer.clear();
	}
}

/// Spell target handling for commonly used simple target add's
/// mostly used in scripted and kinda unknown target

/// This handler is used in
/// Spell Target Handling for type 46: Unknown Summon Atal'ai Skeleton
/// Spell Target Handling for type 47: Portal (Not sure, fix me.)
/// Spell Target Handling for type 48: fix me
/// Spell Target Handling for type 49: fix me
/// Spell Target Handling for type 50: fix me
/// Spell Target Handling for type 52: Lightwells, etc (Need more research)

void Spell::SpellTargetSimpleTargetAdd(uint32 i, uint32 j)
{
	_AddTargetForced(m_caster->GetGUID(), i);
}

void Spell::SpellTargetGameObjectsInByCasterPos(uint32 i, uint32 j)
{
	if(m_caster == NULL)
		return;

	FillAllGameObjectTargetsInArea(i, m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), GetRadius(i));
}

void Spell::SpellTargetGameObjectsInArea(uint32 i, uint32 j)
{
	if(m_caster == NULL)
		return;

	FillAllGameObjectTargetsInArea(i, m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ, GetRadius(i));
}

/// Spell Target Handling for type 53: Target Area by Players CurrentSelection()
void Spell::SpellTargetTargetAreaSelectedUnit(uint32 i, uint32 j)
{
	Unit* Target = NULLUNIT;
	if(m_caster->IsInWorld())
	{
		if(p_caster)
			Target = m_caster->GetMapMgr()->GetUnit(p_caster->GetSelection());
		else
			Target = m_caster->GetMapMgr()->GetUnit(m_targets.m_unitTarget);
	}

	if(!Target)
		return;

	FillAllTargetsInArea((LocationVector&)Target->GetPosition(), i);
}

/// Spell Target Handling for type 54: Targets in Front of the Caster
void Spell::SpellTargetInFrontOfCaster2(uint32 i, uint32 j)
{
	unordered_set<Object* >::iterator itr;

	for( itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); itr++ )
	{
		if(!((*itr)->IsUnit()) || !TO_UNIT(*itr)->isAlive())
			continue;
		//is Creature in range
		if(m_caster->isInRange(TO_UNIT(*itr),GetRadius(i)))
		{
			if(m_caster->isTargetInFront(TO_UNIT(*itr)))
			{
				if(isAttackable(u_caster, TO_UNIT(*itr)))
					_AddTarget(TO_UNIT(*itr), i);
			}
		}
	}
}

/// Spell Target Handling for type 56: your party or raid within radius
void Spell::SpellTarget56(uint32 i, uint32 j)
{
	SpellTargetAllPartyMembersRangeNR(i, j);
	/*if(!m_caster->IsInWorld())
		return;

	_AddTargetForced(m_caster->GetGUID(), i);*/
}

/// Spell Target Handling for type 57: Targeted Party Member
void Spell::SpellTargetTargetPartyMember(uint32 i, uint32 j)
{
	if(!m_caster->IsInWorld())
		return;

	Unit* Target = m_caster->GetMapMgr()->GetPlayer ((uint32)m_targets.m_unitTarget);
	if(!Target)
		Target = u_caster;
	if(!Target)
		return;

	float r=GetFriendlyMaxRange(dbcSpellRange.LookupEntry(GetSpellProto()->rangeIndex));
	if(IsInrange(m_caster->GetPositionX(),m_caster->GetPositionY(),m_caster->GetPositionZ(),Target,r*r))
		_AddTargetForced(Target->GetGUID(), i);
}

/// Spell Target Handling for type 61: targets with the same group/raid and the same class
void Spell::SpellTargetSameGroupSameClass(uint32 i, uint32 j)
{
	if(!m_caster->IsInWorld())
		return;

	if( m_caster->IsPlayer() )
		if( !TO_PLAYER(m_caster)->GetGroup() )
			_AddTargetForced(m_caster->GetGUID(), i);

	Player* Target = m_caster->GetMapMgr()->GetPlayer((uint32)m_targets.m_unitTarget);
	if(!Target)
		return;

	if( m_caster->IsPlayer() && Target->IsPlayer() && Target->GetGroup() && (Target == m_caster || Target->GetGroup()->HasMember(TO_PLAYER(m_caster))) )
	{
		Target->GetGroup()->Lock();

		for( uint32 x = 0; x < Target->GetGroup()->GetSubGroupCount(); ++x )
		{
			for(GroupMembersSet::iterator itr = Target->GetGroup()->GetSubGroup( x )->GetGroupMembersBegin(); itr != Target->GetGroup()->GetSubGroup( x )->GetGroupMembersEnd(); itr++)
			{
				if(!(*itr)->m_loggedInPlayer || Target->getClass() != (*itr)->m_loggedInPlayer->getClass())
					continue;

				_AddTargetForced((*itr)->m_loggedInPlayer->GetGUID(), i);
			}
		}
		Target->GetGroup()->Unlock();
	}
}


// returns Guid of lowest percentage health friendly party or raid target within sqrt('dist') yards
uint64 Spell::FindLowestHealthRaidMember(Player* Target, uint32 dist)
{
	if(!Target || !Target->IsInWorld())
		return 0;

	uint64 lowestHealthTarget = Target->GetGUID();
	uint32 lowestHealthPct = Target->GetHealthPct();
	Group *group = Target->GetGroup();
	if(group)
	{
		group->Lock();
		for(uint32 j = 0; j < group->GetSubGroupCount(); ++j) {
			for(GroupMembersSet::iterator itr = group->GetSubGroup(j)->GetGroupMembersBegin(); itr != group->GetSubGroup(j)->GetGroupMembersEnd(); itr++)
			{
				if((*itr)->m_loggedInPlayer && Target->GetDistance2dSq((*itr)->m_loggedInPlayer) <= dist)
				{
					uint32 healthPct = (*itr)->m_loggedInPlayer->GetHealthPct();
					if(healthPct < lowestHealthPct)
					{
						lowestHealthPct = healthPct;
						lowestHealthTarget = (*itr)->m_loggedInPlayer->GetGUID();
					}
				}
			}
		}
		group->Unlock();
	}
	return lowestHealthTarget;
}

/// Spell Target Handling for type 76 All targets in area
void Spell::SpellTargetAllTargetsInArea(uint32 i, uint32 j)
{
	if( !m_caster || !m_caster->IsInWorld() )
		return;

	FillAllTargetsInArea(i, m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ, GetRadius(i));
}

/// Spell Target Handling for type 87
void Spell::SpellTargetAreaOfEffect87(uint32 i, uint32 j)
{
	if( !m_caster || !m_caster->IsInWorld() )
		return;

	FillAllGameObjectTargetsInArea(i, m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ, GetRadius(i));
}

/// Spell Target Handling for type 89
void Spell::SpellTargetProjectile(uint32 i, uint32 j)
{
	if( !m_caster || !m_caster->IsInWorld() )
		return;

	FillAllTargetsInArea(i, m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ, GetRadius(i));
}

/// Spell Target Handling for type 94: Target: Vehicle
void Spell::SpellTargetVehicle(uint32 i, uint32 j)
{
	if(p_caster)
	{
		if( p_caster->GetVehicle() != NULL )
			_AddTargetForced( p_caster->GetVehicle()->GetGUID(), i );
	}
}

void Spell::SpellTargetVehicleDriver(uint32 i, uint32 j)
{
	if(!v_caster)
		return;
	if(v_caster->GetControllingUnit())
		_AddTargetForced(v_caster->GetControllingUnit(), i );
}

void Spell::SpellTargetVehiclePassenger(uint32 i, uint32 j)
{
	if(!v_caster)
		return;

	if(v_caster->GetPassenger(0))
		_AddTargetForced(v_caster->GetPassenger(0), i );
}

void Spell::SpellTargetVehiclePassenger1(uint32 i, uint32 j)
{
	if(v_caster)
	{
		if(v_caster->GetPassenger(1))
			_AddTargetForced(v_caster->GetPassenger(1), i );
		return;
	}
	Unit* target = m_caster->GetMapMgr()->GetUnit(m_targets.m_unitTarget);
	if(!target)
		return;
	if(target->IsVehicle())
	{
		if(TO_VEHICLE(target)->GetPassenger(1))
			_AddTargetForced(TO_VEHICLE(target)->GetPassenger(1), i );
		return;
	}
	if(target->GetVehicle(true))
		if(TO_VEHICLE(target->GetVehicle(true))->GetPassenger(1))
			_AddTargetForced(TO_VEHICLE(target)->GetPassenger(1), i );
}

void Spell::SpellTargetVehiclePassenger2(uint32 i, uint32 j)
{
	if(v_caster)
	{
		if(v_caster->GetPassenger(2))
			_AddTargetForced(v_caster->GetPassenger(2), i );
		return;
	}
	Unit* target = m_caster->GetMapMgr()->GetUnit(m_targets.m_unitTarget);
	if(!target)
		return;
	if(target->IsVehicle())
	{
		if(TO_VEHICLE(target)->GetPassenger(2))
			_AddTargetForced(TO_VEHICLE(target)->GetPassenger(2), i );
		return;
	}
	if(target->GetVehicle(true))
		if(TO_VEHICLE(target->GetVehicle(true))->GetPassenger(2))
			_AddTargetForced(TO_VEHICLE(target)->GetPassenger(2), i );
}

void Spell::SpellTargetVehiclePassenger3(uint32 i, uint32 j)
{
	if(v_caster)
	{
		if(v_caster->GetPassenger(3))
			_AddTargetForced(v_caster->GetPassenger(3), i );
		return;
	}
	Unit* target = m_caster->GetMapMgr()->GetUnit(m_targets.m_unitTarget);
	if(!target)
		return;
	if(target->IsVehicle())
	{
		if(TO_VEHICLE(target)->GetPassenger(3))
			_AddTargetForced(TO_VEHICLE(target)->GetPassenger(3), i );
		return;
	}
	if(target->GetVehicle(true))
		if(TO_VEHICLE(target->GetVehicle(true))->GetPassenger(3))
			_AddTargetForced(TO_VEHICLE(target)->GetPassenger(3), i );
}

void Spell::SpellTargetVehiclePassenger4(uint32 i, uint32 j)
{
	if(v_caster)
	{
		if(v_caster->GetPassenger(4))
			_AddTargetForced(v_caster->GetPassenger(4), i );
		return;
	}
	Unit* target = m_caster->GetMapMgr()->GetUnit(m_targets.m_unitTarget);
	if(!target)
		return;
	if(target->IsVehicle())
	{
		if(TO_VEHICLE(target)->GetPassenger(4))
			_AddTargetForced(TO_VEHICLE(target)->GetPassenger(4), i );
		return;
	}
	if(target->GetVehicle(true))
		if(TO_VEHICLE(target->GetVehicle(true))->GetPassenger(4))
			_AddTargetForced(TO_VEHICLE(target)->GetPassenger(4), i );
}

void Spell::SpellTargetVehiclePassenger5(uint32 i, uint32 j)
{
	if(v_caster)
	{
		if(v_caster->GetPassenger(5))
			_AddTargetForced(v_caster->GetPassenger(5), i );
		return;
	}
	Unit* target = m_caster->GetMapMgr()->GetUnit(m_targets.m_unitTarget);
	if(!target)
		return;
	if(target->IsVehicle())
	{
		if(TO_VEHICLE(target)->GetPassenger(5))
			_AddTargetForced(TO_VEHICLE(target)->GetPassenger(5), i );
		return;
	}
	if(target->GetVehicle(true))
		if(TO_VEHICLE(target->GetVehicle(true))->GetPassenger(5))
			_AddTargetForced(TO_VEHICLE(target)->GetPassenger(5), i );
}

void Spell::SpellTargetVehiclePassenger6(uint32 i, uint32 j)
{
	if(v_caster)
	{
		if(v_caster->GetPassenger(6))
			_AddTargetForced(v_caster->GetPassenger(6), i );
		return;
	}
	Unit* target = m_caster->GetMapMgr()->GetUnit(m_targets.m_unitTarget);
	if(!target)
		return;
	if(target->IsVehicle())
	{
		if(TO_VEHICLE(target)->GetPassenger(6))
			_AddTargetForced(TO_VEHICLE(target)->GetPassenger(6), i );
		return;
	}
	if(target->GetVehicle(true))
		if(TO_VEHICLE(target->GetVehicle(true))->GetPassenger(6))
			_AddTargetForced(TO_VEHICLE(target)->GetPassenger(6), i );
}

void Spell::SpellTargetVehiclePassenger7(uint32 i, uint32 j)
{
	if(v_caster)
	{
		if(v_caster->GetPassenger(7))
			_AddTargetForced(v_caster->GetPassenger(7), i );
		return;
	}
	Unit* target = m_caster->GetMapMgr()->GetUnit(m_targets.m_unitTarget);
	if(!target)
		return;
	if(target->IsVehicle())
	{
		if(TO_VEHICLE(target)->GetPassenger(7))
			_AddTargetForced(TO_VEHICLE(target)->GetPassenger(7), i );
		return;
	}
	if(target->GetVehicle(true))
		if(TO_VEHICLE(target->GetVehicle(true))->GetPassenger(7))
			_AddTargetForced(TO_VEHICLE(target)->GetPassenger(7), i );
}
