/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#include "StdAfx.h"

/****************
 *  TempSummon	*
 ****************/
TempSummon::TempSummon(SummonPropertiesEntry const *properties, Unit *owner) : Creature(0), m_type(TEMPSUMMON_MANUAL_DESPAWN), m_timer(0), m_lifetime(0), m_Properties(properties)
{
    m_summonerGUID = owner ? owner->GetGUID() : 0;
    m_unitTypeMask |= UNIT_MASK_SUMMON;
}

Unit* TempSummon::GetSummoner() const
{
    return m_summonerGUID ? GetUnit(*this, m_summonerGUID) : NULL;
}

void TempSummon::InitSummon()
{
    Unit* owner = GetSummoner();
    if(owner)
    {
        if(owner->GetTypeId()==TYPEID_UNIT && ((Creature*)owner)->IsAIEnabled)
            ((Creature*)owner)->AI()->JustSummoned(this);

        if(IsAIEnabled)
            AI()->IsSummonedBy(owner);
    }
}

void TempSummon::InitStats(uint32 duration)
{
	assert(!isPet());

	m_timer = duration;
	m_lifetime = duration;

	if(m_type == TEMPSUMMON_MANUAL_DESPAWN)
		m_type = (duration == 0) ? TEMPSUMMON_DEAD_DESPAWN : TEMPSUMMON_TIMED_DESPAWN;

	Unit *owner = GetSummoner();

	if(owner && isTrigger() && m_spells[0])
	{
		setFaction(owner->getFaction());
		SetLevel(owner->getLevel());
		if(owner->GetTypeId() == TYPEID_PLAYER)\
			m_ControlledByPlayer = true;
	}

	if(!m_Properties)
		return;

	if(owner)
	{
		if(uint32 slot = m_Properties->slot)
		{
			if(owner->m_SummonSlot[slot] && owner->m_SummonSlot[slot] != GetGUID())
			{
				Creature *oldSummon = GetMapMgr()->GetCreature(owner->m_SummonSlot[slot]);
				if(oldSummon && oldSummon->isSummon())
					((TempSummon*)oldSummon)->UnSummon();
			}
			owner->m_SummonSlot[slot] = GetGUID();
		}
	}

	if(m_Properties->factionId)
		setFaction(m_Properties->factionId);
	else if(IsVehicle()) // properties should be vehicle
		setFaction(owner->getFaction());
}

void TempSummon::Update(uint32 diff)
{
    Creature::Update(diff);

    if (m_deathState == DEAD)
    {
        UnSummon();
        return;
    }
    switch(m_type)
    {
        case TEMPSUMMON_MANUAL_DESPAWN:
            break;
        case TEMPSUMMON_TIMED_DESPAWN:
        {
            if (m_timer <= diff)
            {
                UnSummon();
                return;
            }

            m_timer -= diff;
            break;
        }
        case TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT:
        {
			if (!CombatStatus.IsInCombat())
            {
                if (m_timer <= diff)
                {
                    UnSummon();
                    return;
                }

                m_timer -= diff;
            }
            else if (m_timer != m_lifetime)
                m_timer = m_lifetime;

            break;
        }

        case TEMPSUMMON_CORPSE_TIMED_DESPAWN:
        {
            if ( m_deathState == CORPSE)
            {
                if (m_timer <= diff)
                {
                    UnSummon();
                    return;
                }

                m_timer -= diff;
            }
            break;
        }
        case TEMPSUMMON_CORPSE_DESPAWN:
        {
            // if m_deathState is DEAD, CORPSE was skipped
            if ( m_deathState == CORPSE || m_deathState == DEAD)
            {
                UnSummon();
                return;
            }

            break;
        }
        case TEMPSUMMON_DEAD_DESPAWN:
        {
            if ( m_deathState == DEAD )
            {
                UnSummon();
                return;
            }
            break;
        }
        case TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN:
        {
            // if m_deathState is DEAD, CORPSE was skipped
            if ( m_deathState == CORPSE || m_deathState == DEAD)
            {
                UnSummon();
                return;
            }

            if (!CombatStatus.IsInCombat())
            {
                if (m_timer <= diff)
                {
                    UnSummon();
                    return;
                }
                else
                    m_timer -= diff;
            }
            else if (m_timer != m_lifetime)
                m_timer = m_lifetime;
            break;
        }
        case TEMPSUMMON_TIMED_OR_DEAD_DESPAWN:
        {
            // if m_deathState is DEAD, CORPSE was skipped
            if (m_deathState == DEAD)
            {
                UnSummon();
                return;
            }

            if (!CombatStatus.IsInCombat() && isAlive() )
            {
                if (m_timer <= diff)
                {
                    UnSummon();
                    return;
                }
                else
                    m_timer -= diff;
            }
            else if (m_timer != m_lifetime)
                m_timer = m_lifetime;
            break;
        }
        default:
            UnSummon();
            sLog.outError("Temporary summoned creature (entry: %u) have unknown type %u of ",GetEntry(),m_type);
            break;
    }
}

void TempSummon::SetTempSummonType(TempSummonType type)
{
    m_type = type;
}

void TempSummon::UnSummon()
{
    //assert(!isPet());
    if(isPet())
    {
        ((Pet*)this)->Remove( true, true, true );
        assert(!IsInWorld());
        return;
    }

    Unit* owner = GetSummoner();
    if(owner && owner->GetTypeId() == TYPEID_UNIT && ((Creature*)owner)->IsAIEnabled)
        ((Creature*)owner)->AI()->SummonedCreatureDespawn(this);

	if(IsInWorld())
		RemoveFromWorld();
}

void TempSummon::RemoveFromWorld()
{
    if(!IsInWorld())
        return;

    if(m_Properties)
    {
        if(uint32 slot = m_Properties->slot)
        {
            if(Unit* owner = GetSummoner())
            {
                if(owner->m_SummonSlot[slot] = GetGUID())
                    owner->m_SummonSlot[slot] = 0;
            }
        }
    }

    //if(GetOwnerGUID())
    //    sLog.outError("Unit %u has owner guid when removed from world", GetEntry());

    Creature::RemoveFromWorld( false, true);
}

/****************
 *	  Minion	*
 ****************/
Minion::Minion(SummonPropertiesEntry const *properties, Unit *owner) : TempSummon(properties, owner), m_owner(owner)
{
    assert(m_owner);
    m_unitTypeMask |= UNIT_MASK_MINION;
    m_followAngle = PET_FOLLOW_ANGLE;
}

/****************
 *	 Guardian	*
 ****************/

Guardian::Guardian(SummonPropertiesEntry const *properties, Unit *owner) : Minion(properties, owner), m_bonusdamage(0)
{
    m_unitTypeMask |= UNIT_MASK_GUARDIAN;
    if (properties && properties->type == SUMMON_TYPE_PET)
    {
        m_unitTypeMask |= UNIT_MASK_CONTROLABLE_GUARDIAN;
        //InitCharmInfo();
    }
}

/****************
 *	  Puppet 	*
 ****************/

Puppet::Puppet(SummonPropertiesEntry const *properties, Unit *owner) : Minion(properties, owner)
{
    assert(owner->GetTypeId() == TYPEID_PLAYER);
    m_owner = (Player*)owner;
    m_unitTypeMask |= UNIT_MASK_PUPPET;
}
