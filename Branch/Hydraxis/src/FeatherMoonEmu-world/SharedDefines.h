/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */
 
#ifndef _SHARED_DEFINES_H
#define _SHARED_DEFINES_H

#define MAKE_NEW_GUID(l, e, h)   uint64( uint64(l) | ( uint64(e) << 24 ) | ( uint64(h) << 48 ) )
#define ACE_TEXT(STRING) STRING
#define ACE_UINT64_FORMAT_SPECIFIER ACE_TEXT ("%I64u")
#define UI64FMTD ACE_UINT64_FORMAT_SPECIFIER
#define MAKE_PAIR64(l, h)  uint64( uint32(l) | ( uint64(h) << 32 ) )
#define OTHER_TEAM(a) (a == TEAM_ALLIANCE ? TEAM_HORDE : TEAM_ALLIANCE)
#define getDefenderTeam() m_defender
#define getAttackerTeam() OTHER_TEAM(m_defender)
#define NPC_CLICK_CAST_CASTER_PLAYER              0x01
#define NPC_CLICK_CAST_TARGET_PLAYER              0x02
#define NPC_CLICK_CAST_ORIG_CASTER_OWNER          0x04

enum TeamId
{
    TEAM_ALLIANCE = 0,
    TEAM_HORDE,
    TEAM_NEUTRAL,
};

enum Team
{
    HORDE               = 67,
    ALLIANCE            = 469,
    //TEAM_STEAMWHEEDLE_CARTEL = 169,                       // not used in code
    //TEAM_ALLIANCE_FORCES     = 891,
    //TEAM_HORDE_FORCES        = 892,
    //TEAM_SANCTUARY           = 936,
    //TEAM_OUTLAND             = 980,
    TEAM_OTHER               = 0,                         // if ReputationListId > 0 && Flags != FACTION_FLAG_TEAM_HEADER
};

enum RaidDifficulties
{
    RAID_DIFFICULTY_10MAN_NORMAL = 0,
    RAID_DIFFICULTY_10MAN_HEROIC = 1,
    RAID_DIFFICULTY_25MAN_NORMAL = 2,
    RAID_DIFFICULTY_25MAN_HEROIC = 3,
    TOTAL_RAID_DIFFICULTIES
};

enum SummonCategory
{
    SUMMON_CATEGORY_WILD        = 0,
    SUMMON_CATEGORY_ALLY        = 1,
    SUMMON_CATEGORY_PET         = 2,
    SUMMON_CATEGORY_PUPPET      = 3,
    SUMMON_CATEGORY_VEHICLE     = 4,
};

const Team TeamId2Team[3] = {ALLIANCE, HORDE, TEAM_OTHER};

#endif