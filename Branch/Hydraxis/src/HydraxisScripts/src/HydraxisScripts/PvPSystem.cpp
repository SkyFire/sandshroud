/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#include "PvPMgr.h"

#define REWARD_GOLD 1
/*
/* Ranks * /
const int Rank1  = 0;
const int Rank2  = 5;
const int Rank3  = 50;
const int Rank4  = 100;
const int Rank5  = 500;
const int Rank6  = 1000;
const int Rank7  = 1500;
const int Rank8  = 4000;
const int Rank9  = 7000;
const int Rank10 = 10000;
const int Rank11 = 15000;
const int Rank12 = 22000;
const int Rank13 = 30000;
const int Rank14 = 39000;
const int Rank15 = 50000;

PvPStorage *Storage;

void OnKillPlayer(Player * attacker, Player * victim)
{
	PvPMgr *aMgr = Storage->GetMgr(attacker->GetLowGUID(), true);
	PvPMgr *vMgr = Storage->GetMgr(victim->GetLowGUID(), true);
												   
	if(aMgr == NULL || vMgr == NULL)
		return;

	if(aMgr->ChecksPassed(attacker, victim) == false)
	{
		vMgr->CurrentKills = 0;
		vMgr->PlayersHealed = 0;
		vMgr->TotalDeaths++;
		return;
	}

	if ( attacker->GetTeam() == victim->GetTeam() )
		return;

	/* Player is in BG or Arena * /
	if( attacker->m_bg || (attacker->m_bg && attacker->m_bg->IsArena()) )
	{
		aMgr->CurrentKills++;

		switch ( aMgr->CurrentKills ) // Give players with specific kills something special :P
		{
			case 5 :
			{
				attacker->CastSpell(attacker, 33344, true);
			}
			break;
			case 10 :
			{
				/* char msg[100];
				sprintf(msg, "|cff00ff00[BG PvP]|r |cffff0000 10 Kills - %s killed %s for a 10 kill streak!|r", attacker->GetName(), victim->GetName());
				sWorld.SendWorldWideScreenText(msg);

				char msg2[100];
				sprintf(msg2, "|cff00ff00[BG PvP]|r |cffff0000 10 Kills - %s killed %s for a 10 kill streak!|r", attacker->GetName(), victim->GetName());
				sWorld.SendWorldText(msg); // send message * /

				attacker->CastSpell(attacker, 24378, true);
			}
			break;
			case 20 :
			{
				char msg[100];
				sprintf(msg, "|cff00ff00[Feathermoon PvP]|r |cffff0000 20 Kills - %s killed %s for a 20 kill streak!|r", attacker->GetName(), victim->GetName());
				sWorld.SendWorldWideScreenText(msg);

				char msg2[100];
				sprintf(msg2, "|cff00ff00[Feathermoon PvP]|r |cffff0000 20 Kills - %s killed %s for a 20 kill streak!|r", attacker->GetName(), victim->GetName());
				sWorld.SendWorldText(msg);
			}
			break;
			case 30:
			{
				char msg[100];
				sprintf(msg, "|cff00ff00[Feathermoon PvP]|r |cffff0000 30 Kills - %s is on a Killing Frenzy!|r", attacker->GetName());
				sWorld.SendWorldWideScreenText(msg);

				char msg2[100];
				sprintf(msg2, "|cff00ff00[Feathermoon PvP]|r |cffff0000 30 Kills - %s is on a Killing Frenzy!|r", attacker->GetName());
				sWorld.SendWorldText(msg); // send message	
			}
			break;
			case 40:
			{
				char msg[100];
				sprintf(msg, "|cff00ff00[Feathermoon PvP]|r |cffff0000 40 Kills - %s is Annihilating the Opponent!|r", attacker->GetName());
				sWorld.SendWorldWideScreenText(msg);

				char msg2[100];
				sprintf(msg2, "|cff00ff00[Feathermoon PvP]|r |cffff0000 40 Kills - %s is Annihilating the Opponent!|r", attacker->GetName());
				sWorld.SendWorldText(msg); // send message
			}
			break;
			case 50:
			{
				char msg[100];
				sprintf(msg, "|cff00ff00[Feathermoon PvP]|r |cffff0000 50 Kills - %s is Jaw Dropping!|r", attacker->GetName());
				sWorld.SendWorldWideScreenText(msg);

				char msg2[100];
				sprintf(msg2, "|cff00ff00[Feathermoon PvP]|r |cffff0000 50 Kills - %s is Jaw Dropping!|r", attacker->GetName());
				sWorld.SendWorldText(msg); // send message
			}
			break;
			case 60:
			{
				char msg[100];
				sprintf(msg, "|cff00ff00[Feathermoon PvP]|r |cffff0000 60 Kills - %s is Unstoppable!|r", attacker->GetName());
				sWorld.SendWorldWideScreenText(msg);

				char msg2[100];
				sprintf(msg2, "|cff00ff00[Feathermoon PvP]|r |cffff0000 60 Kills - %s is Unstoppable!|r", attacker->GetName());
				sWorld.SendWorldText(msg); // send message
			}
			break;
			case 70:
			{
				char msg[100];
				sprintf(msg, "|cff00ff00[Feathermoon PvP]|r |cffff0000 70 Kills - WTF! Somebody just kill %s already!", attacker->GetName());
				sWorld.SendWorldWideScreenText(msg);

				char msg2[100];
				sprintf(msg2, "|cff00ff00[Feathermoon PvP]|r |cffff0000 70 Kills - WTF! Somebody just kill %s already!", attacker->GetName());
				sWorld.SendWorldText(msg); // send message
			}
			break;
			case 80:
			{
				char msg[100];
				sprintf(msg, "|cff00ff00[Feathermoon PvP]|r |cffff0000 80 Kills - %s is Superior!", attacker->GetName());
				sWorld.SendWorldWideScreenText(msg);

				char msg2[100];
				sprintf(msg2, "|cff00ff00[Feathermoon PvP]|r |cffff0000 80 Kills - %s is Superior!", attacker->GetName());
				sWorld.SendWorldText(msg); // send message
			}
			break;
			case 90:
			{
				char msg[100];
				sprintf(msg, "|cff00ff00[Feathermoon PvP]|r |cffff0000 90 Kills - %s is beyond Superior", attacker->GetName());
				sWorld.SendWorldWideScreenText(msg);

				char msg2[100];
				sprintf(msg2, "|cff00ff00[Feathermoon PvP]|r |cffff0000 90 Kills - %s is beyond Superior!", attacker->GetName());
				sWorld.SendWorldText(msg); // send message
			}
			break;
			case 100:
			{
				char msg[100];
				sprintf(msg, "|cff00ff00[Feathermoon PvP]|r |cffff0000 100 Kills - %s is The TRUE King!", attacker->GetName());
				sWorld.SendWorldWideScreenText(msg);

				char msg2[100];
				sprintf(msg2, "|cff00ff00[Feathermoon PvP]|r |cffff0000 100 Kills - %s is The TRUE King!", attacker->GetName());
				sWorld.SendWorldText(msg); // send message
			}
			break;
		}
		
		if(aMgr->CurrentKills > 100)
		{
			char msg[100];
			sprintf(msg, "|cff00ff00[Feathermoon PvP]|r |cffff0000 100 Kills - %s is The TRUE King!", attacker->GetName());
			sWorld.SendWorldWideScreenText(msg);

			char msg2[100];
			sprintf(msg2, "|cff00ff00[Feathermoon PvP]|r |cffff0000 100 Kills - %s is The TRUE King!", attacker->GetName());
			sWorld.SendWorldText(msg); // send message
		}

		if ( !(attacker->GetGroup()) )
		{
			attacker->BroadcastMessage("|cff00ff00[Feathermoon PvP]|r Nice job! You received %u gold for slaying %s%s|r", REWARD_GOLD, MSG_COLOR_WHITE, victim->GetName());

			vMgr->CurrentKills = 0;
			vMgr->PlayersHealed = 0;
			vMgr->TotalDeaths++;

			victim->BroadcastMessage("|cff00ff00[Feathermoon PvP]|r Your kills have been reset since you died!");

			attacker->BroadcastMessage("|cff00ff00[Feathermoon PvP]|r %sCurrent Kills: %u", MSG_COLOR_GOLD, aMgr->CurrentKills);

			return;
		}

		std::vector<Player *> GroupMembers;
		std::set<Player *> PotentialHealers;
		attacker->GetGroup()->Lock();
		for(uint8 i = 0; i < attacker->GetGroup()->GetSubGroupCount(); i++)
		{
			SubGroup * pSubGroup = attacker->GetGroup()->GetSubGroup(i);
			if(!pSubGroup) continue;

			GroupMembersSet::iterator itr = pSubGroup->GetGroupMembersBegin();
			for(; itr != pSubGroup->GetGroupMembersEnd(); itr++)
			{
				PlayerInfo * pi = *itr;
				if(!pi) continue;

				Player * tPlr = pi->m_loggedInPlayer;
				if(!tPlr) 
					continue;

				// Ineligible due to distance.
				if(tPlr->GetDistance2dSq(attacker) > 80.0f && tPlr != attacker)
					continue;
			
				GroupMembers.push_back(pi->m_loggedInPlayer);
			}
		}

		// Loop over it again for the healers.
		for(std::set<Player *>::iterator itr2 = PotentialHealers.begin(); itr2 != PotentialHealers.end(); itr2++)
		{
			Player * pHealer = (*itr2);
			if(!pHealer) continue;

			// Giant waste of CPU power inc
			for(std::vector<Player *>::iterator itr3 = GroupMembers.begin(); itr3 != GroupMembers.end(); itr3++)
			{
				Player * pGroupMember = (*itr3);
				if(pHealer->CombatStatus.DidHeal(pGroupMember->GetLowGUID()))
				{
					GroupMembers.push_back(pHealer);
					break;
				}
			}
		}

		// Pick a number, any number!
		if(GroupMembers.size() > 0)
		{
			uint32 reward = RandomUInt(GroupMembers.size() - 1);
			GroupMembers[reward]->ModUnsigned32Value(PLAYER_FIELD_COINAGE, (REWARD_GOLD * 10000));
			GroupMembers[reward]->BroadcastMessage("|cff00ff00[Feathermoon PvP]|r You've been awarded 1 gold for slaying %s%s.|r", victim->GetName(), MSG_COLOR_WHITE);
		}
		attacker->GetGroup()->Unlock();

		attacker->BroadcastMessage("|cff00ff00[Feathermoon PvP]|r %sCurrent Kills: %u", MSG_COLOR_GOLD, aMgr->CurrentKills);

		victim->BroadcastMessage("|cff00ff00[Feathermoon PvP]|r Your kills have been reset since you died!");

		vMgr->CurrentKills = 0;
		vMgr->PlayersHealed = 0;
		vMgr->TotalDeaths++;
	}
	/* World PvP Setup * /
	else
	{
		aMgr->CurrentKills++;

		if ( aMgr->CurrentKills <= 4 ) // Player gets regular amount of gold until he hits killing spree
		{
			if ( !(victim->GetUInt32Value(PLAYER_FIELD_COINAGE) >= 1) )
				// Fuck off, I don't have gold LOL!
				attacker->BroadcastMessage("|cff00ff00[Feathermoon PvP]|r %sSorry,|r %s%s|r doesn't have any gold for you to get!", MSG_COLOR_GOLD, victim->GetName(), MSG_COLOR_WHITE);
			else
			{
				// Give attacker one gold for killing the player
				attacker->BroadcastMessage("|cff00ff00[Feathermoon PvP]|r Nice job! You received %u Token for slaying %s%s.|r", REWARD_GOLD, MSG_COLOR_WHITE, victim->GetName());
				attacker->ModUnsigned32Value(PLAYER_FIELD_COINAGE, REWARD_GOLD * 10000);

				// Subtract one gold from player for dying
				victim->BroadcastMessage("|cff00ff00[Feathermoon PvP]|r %sYou have lost one gold for getting killed outside a BG!", MSG_COLOR_LIGHTRED);
				victim->ModUnsigned32Value(PLAYER_FIELD_COINAGE, PLAYER_FIELD_COINAGE - (REWARD_GOLD * 10000));
			}

			aMgr->killingSpree = false;
			aMgr->killingSpree2 = true;
		}
		else
			aMgr->killingSpree = true;

		if ( aMgr->killingSpree && aMgr->killingSpree2 ) // Just show this one that he now has a killing spree
		{
			if ( !(victim->GetUInt32Value(PLAYER_FIELD_COINAGE) >= (2 * 10000)) )
			{
				attacker->BroadcastMessage("|cff00ff00[Feathermoon PvP]|r You have started a killing spree! You will now receive double the gold."); // Still announce it
				attacker->CastSpell(attacker, 40735, true);

				// Fuck off, I don't have gold LOL!
				attacker->BroadcastMessage("|cff00ff00[Feathermoon PvP]|r %sSorry,|r %s%s|r doesn't have any gold for you to get!", MSG_COLOR_GOLD, MSG_COLOR_WHITE, victim->GetName());
			}
			else
			{
				// Give attacker .25 * victim's kills for killing the player
				attacker->BroadcastMessage("|cff00ff00[Feathermoon PvP]|r You have started a killing spree! You will now receive double the gold.", REWARD_GOLD * 2);
				attacker->ModUnsigned32Value(PLAYER_FIELD_COINAGE, (REWARD_GOLD * 10000) * .25f * vMgr->CurrentKills * 2);

				// Subtract one gold from player for dying
				victim->BroadcastMessage("|cff00ff00[Feathermoon PvP]|r %sYou have lost one gold for getting killed outside a BG!", MSG_COLOR_LIGHTRED);
				victim->ModUnsigned32Value(PLAYER_FIELD_COINAGE, PLAYER_FIELD_COINAGE - (REWARD_GOLD * 10000));
			}

			aMgr->killingSpree = false;
			aMgr->killingSpree2 = false;
		}

		if ( aMgr->CurrentKills > 4 ) // Keep giving him double gold for the killing spree
		{
			if ( !(victim->GetUInt32Value(PLAYER_FIELD_COINAGE) >= 1) )
				// Fuck off, I don't have gold LOL!
				attacker->BroadcastMessage("|cff00ff00[Feathermoon PvP]|r %sSorry,|r %s%s|r doesn't have any gold for you to get!", MSG_COLOR_GOLD, MSG_COLOR_WHITE, victim->GetName());
			else
			{
				// Give attacker one gold for killing the player
				attacker->BroadcastMessage("|cff00ff00[Feathermoon PvP]|r Nice job! You received %u gold for slaying %s%s.|r", REWARD_GOLD * 2, MSG_COLOR_WHITE, victim->GetName());
				attacker->ModUnsigned32Value(PLAYER_FIELD_COINAGE, (REWARD_GOLD * 10000) * 2);

				// Subtract one gold from player for dying
				victim->BroadcastMessage("%sYou have lost one gold for getting killed outside a BG!", MSG_COLOR_LIGHTRED);
				victim->ModUnsigned32Value(PLAYER_FIELD_COINAGE, PLAYER_FIELD_COINAGE - (REWARD_GOLD * 10000));
			}
		}

		switch ( aMgr->CurrentKills ) // Give players with specific kills something special :P
		{
			case 10 :
			{
				/* char msg[100];
				sprintf(msg, "|cff00ff00[Feathermoon PvP]|r |cffff0000 10 Kills - %s killed %s for a 10 kill streak!|r", attacker->GetName(), victim->GetName());
				sWorld.SendWorldWideScreenText(msg);

				char msg2[100];
				sprintf(msg2, "|cff00ff00[Feathermoon PvP]|r |cffff0000 10 Kills - %s killed %s for a 10 kill streak!|r", attacker->GetName(), victim->GetName());
				sWorld.SendWorldText(msg); // send message * /

				attacker->CastSpell(attacker, 24378, true);
			}
			break;
			case 20 :
			{
				char msg[100];
				sprintf(msg, "|cff00ff00[Feathermoon PvP]|r |cffff0000 20 Kills - %s killed %s for a 20 kill streak!|r", attacker->GetName(), victim->GetName());
				sWorld.SendWorldWideScreenText(msg);

				char msg2[100];
				sprintf(msg2, "|cff00ff00[Feathermoon PvP]|r |cffff0000 20 Kills - %s killed %s for a 20 kill streak!|r", attacker->GetName(), victim->GetName());
				sWorld.SendWorldText(msg); // send message
			}
			break;
			case 30:
			{
				char msg[100];
				sprintf(msg, "|cff00ff00[Feathermoon PvP]|r |cffff0000 30 Kills - %s is on a Killing Frenzy!|r", attacker->GetName());
				sWorld.SendWorldWideScreenText(msg);

				char msg2[100];
				sprintf(msg2, "|cff00ff00[Feathermoon PvP]|r |cffff0000 30 Kills - %s is on a Killing Frenzy!|r", attacker->GetName());
				sWorld.SendWorldText(msg); // send message	
			}
			break;
			case 40:
			{
				char msg[100];
				sprintf(msg, "|cff00ff00[Feathermoon PvP]|r |cffff0000 40 Kills - %s is Annihilating the Opponent!|r", attacker->GetName());
				sWorld.SendWorldWideScreenText(msg);

				char msg2[100];
				sprintf(msg2, "|cff00ff00[Feathermoon PvP]|r |cffff0000 40 Kills - %s is Annihilating the Opponent!|r", attacker->GetName());
				sWorld.SendWorldText(msg); // send message
			}
			break;
			case 50:
			{
				char msg[100];
				sprintf(msg, "|cff00ff00[Feathermoon PvP]|r |cffff0000 50 Kills - %s is Jaw Dropping!|r", attacker->GetName());
				sWorld.SendWorldWideScreenText(msg);

				char msg2[100];
				sprintf(msg2, "|cff00ff00[Feathermoon PvP]|r |cffff0000 50 Kills - %s is Jaw Dropping!|r", attacker->GetName());
				sWorld.SendWorldText(msg); // send message
			}
			break;
			case 60:
			{
				char msg[100];
				sprintf(msg, "|cff00ff00[Feathermoon PvP]|r |cffff0000 60 Kills - %s is Unstoppable!|r", attacker->GetName());
				sWorld.SendWorldWideScreenText(msg);

				char msg2[100];
				sprintf(msg2, "|cff00ff00[Feathermoon PvP]|r |cffff0000 60 Kills - %s is Unstoppable!|r", attacker->GetName());
				sWorld.SendWorldText(msg); // send message
			}
			break;
			case 70:
			{
				char msg[100];
				sprintf(msg, "|cff00ff00[Feathermoon PvP]|r |cffff0000 70 Kills - WTF! Somebody just kill %s already!", attacker->GetName());
				sWorld.SendWorldWideScreenText(msg);

				char msg2[100];
				sprintf(msg2, "|cff00ff00[Feathermoon PvP]|r |cffff0000 70 Kills - WTF! Somebody just kill %s already!", attacker->GetName());
				sWorld.SendWorldText(msg); // send message
			} 
			break;
			case 80:
			{
				char msg[100];
				sprintf(msg, "|cff00ff00[Feathermoon PvP]|r |cffff0000 80 Kills - %s is Superior!", attacker->GetName());
				sWorld.SendWorldWideScreenText(msg);

				char msg2[100];
				sprintf(msg2, "|cff00ff00[Feathermoon PvP]|r |cffff0000 80 Kills - %s is Superior!", attacker->GetName());
				sWorld.SendWorldText(msg); // send message
			}
			break;
			case 90:
			{
				char msg[100];
				sprintf(msg, "|cff00ff00[Feathermoon PvP]|r |cffff0000 90 Kills - %s is beyond Superior!", attacker->GetName());
				sWorld.SendWorldWideScreenText(msg);

				char msg2[100];
				sprintf(msg2, "|cff00ff00[Feathermoon PvP]|r |cffff0000 90 Kills - %s is beyond Superior!", attacker->GetName());
				sWorld.SendWorldText(msg); // send message
			}
			break;
			case 100:
			{
				char msg[100];
				sprintf(msg, "|cff00ff00[Feathermoon PvP]|r |cffff0000 100 Kills - %s is The TRUE King!", attacker->GetName());
				sWorld.SendWorldWideScreenText(msg);

				char msg2[100];
				sprintf(msg2, "|cff00ff00[Feathermoon PvP]|r |cffff0000 100 Kills - %s is The TRUE King!", attacker->GetName());
				sWorld.SendWorldText(msg); // send message
			}
			break;
		}
		
		
		if ( vMgr->CurrentKills > 4 ) // Give player who kills someone with killing spree double gold
		{
			if ( !(victim->GetUInt32Value(PLAYER_FIELD_COINAGE) >= 1) )
			{
				// Fuck off, I don't have gold LOL!
				attacker->BroadcastMessage("|cff00ff00[Feathermoon PvP]|r %sSorry,|r %s%s|r doesn't have any gold for you to get!", MSG_COLOR_GOLD, MSG_COLOR_WHITE, victim->GetName());
			}
			else
			{
				// Give attacker .25 * victim's kills for killing the player
				attacker->ModUnsigned32Value(PLAYER_FIELD_COINAGE, (REWARD_GOLD * 10000) * .25f * vMgr->CurrentKills * 2);
				attacker->BroadcastMessage("|cff00ff00[Feathermoon PvP]|r %sNice!! You received %u gold for ending %s%s|r killing spree!",MSG_COLOR_LIGHTBLUE, REWARD_GOLD * 2, MSG_COLOR_WHITE, victim->GetName());

				// Subtract one gold from player for dying
				victim->BroadcastMessage("|cff00ff00[Feathermoon PvP]|r %sYou have lost one gold for getting killed outside a BG!", MSG_COLOR_LIGHTRED);
				victim->ModUnsigned32Value(PLAYER_FIELD_COINAGE, PLAYER_FIELD_COINAGE - (REWARD_GOLD * 10000));
			}
		}
		else
			victim->BroadcastMessage("|cff00ff00[Feathermoon PvP]|r Your kills have been reset since you died!");


		vMgr->CurrentKills = 0;
		vMgr->PlayersHealed = 0;
		vMgr->TotalDeaths++;
	
		attacker->BroadcastMessage("|cff00ff00[Feathermoon PvP]|r %sCurrent Kills: %u", MSG_COLOR_GOLD, aMgr->CurrentKills);
	}
}

void OnHealPlayer(Player * pPlayer, SpellEntry * pSpell, Unit * pTarget)
{
	if(!pTarget) return;

	// Hey, he's healing
	if(pSpell->c_is_flags & SPELL_FLAG_IS_HEALING)
	{
		if( !(pPlayer->m_bg) || !(pPlayer->m_bg && pPlayer->m_bg->IsArena()) )
			return;

		// Ok, let's setup this chance system.
		uint32 Chance = 3;
		SpellCastTime *sd = dbcSpellCastTime.LookupEntry(pSpell->CastingTimeIndex);
		if(sd && sd->CastTime == 0) // Instant casts
		{
			Chance = 2;
		}

		if(pPlayer == TO_PLAYER(pTarget) || pTarget->GetHealthPct() >= 95)
			return;

		// rewards!
		if(Rand(Chance))
		{
			pPlayer->BroadcastMessage("|cff00ff00[Feathermoon PvP]|r Nice job! You received %u gold for healing %s%s.", REWARD_GOLD, MSG_COLOR_WHITE, TO_PLAYER(pTarget)->GetName());
			pPlayer->ModUnsigned32Value(PLAYER_FIELD_COINAGE, (REWARD_GOLD * 10000));
		}
		return;
	}
}

bool OnDesertedBattleground(Player * pPlayer, SpellEntry * pSpell, Unit * pTarget)
{
	if(pSpell->Id == 26013)
	{
		PvPMgr*mgr = Storage->GetMgr(pPlayer->GetLowGUID(),true);
		mgr->CurrentKills = 0;
		mgr->killingSpree = false;
		mgr->killingSpree2 = false;
		return false;
	}
	return true;
}

void ShowCredits(Player * pPlayer)
{
	pPlayer->BroadcastMessage("%sFeathermoon's PVP System,", MSG_COLOR_RED);
	pPlayer->BroadcastMessage("Designed by: %sFeathermoon|r", MSG_COLOR_LIGHTRED, MSG_COLOR_LIGHTRED, MSG_COLOR_LIGHTRED);
}

void PvPTitles(Player * pPlayer, Player * pVictim)
{
	int32 Honorablekills = pPlayer->m_killsLifetime;

	if ( Honorablekills >= Rank1 && Honorablekills < Rank2 )
	{
		if ( pPlayer->GetTeam() )
			{
				pPlayer->SetKnownTitle( static_cast< RankTitles >( 15 ), true );
				pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 15 );
				
			}
			else
			{
				pPlayer->SetKnownTitle( static_cast< RankTitles > ( 1 ), true );
				pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 1 );
				
			}
	}
	else if ( Honorablekills >= Rank2 && Honorablekills < Rank3 )
	{
		if ( pPlayer->GetTeam() )
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 16 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 16 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 15 ) );
			
		}
		else
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 2 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 2 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 1 ) );
			
		}
	}
	else if ( Honorablekills >= Rank3 && Honorablekills < Rank4 )
	{
		if ( pPlayer->GetTeam() )
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 17 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 17 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 16 ) );
			
		}
		else
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 3 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 3 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 2 ) );
			
		}
	}
	else if ( Honorablekills >= Rank4 && Honorablekills < Rank5 )
	{
		if ( pPlayer->GetTeam() )
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 18 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 18 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 17 ) );
			
		}
		else
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 4 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 4 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 3 ) );
			
		}
	}
	else if ( Honorablekills >= Rank5 && Honorablekills < Rank6 )
	{
		if ( pPlayer->GetTeam() )
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 19 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 19 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 18 ) );
			
		}
		else
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 5 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 5 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 4 ) );
			
		}
	}
	else if ( Honorablekills >= Rank6 && Honorablekills < Rank7 )
	{
		if ( pPlayer->GetTeam() )
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 20 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 20 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 19 ) );
			
		}
		else
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 6 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 6 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 5 ) );
			
		}
	}
	else if ( Honorablekills >= Rank7 && Honorablekills < Rank8 )
	{
		if ( pPlayer->GetTeam() )
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 21 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 21 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 20 ) );
			
		}
		else
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 7 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 7 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 6 ) );
			
		}
	}
	else if ( Honorablekills >= Rank8 && Honorablekills < Rank9 )
	{
		if ( pPlayer->GetTeam() )
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 22 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 22 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 21 ) );
			
		}
		else
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 8 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 8 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 7 ) );
			
		}
	}
	else if ( Honorablekills >= Rank9 && Honorablekills < Rank10 )
	{
		if ( pPlayer->GetTeam() )
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 23 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 23 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 22 ) );
			
		}
		else
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 9 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 9 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 8 ) );
			
		}
	}
	else if ( Honorablekills >= Rank10 && Honorablekills < Rank11 )
	{
		if ( pPlayer->GetTeam() )
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 24 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 24 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 23 ) );
			
		}
		else
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 10 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 10 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 9 ) );
			
		}
	}
	else if ( Honorablekills >= Rank11 && Honorablekills < Rank12 )
	{
		if ( pPlayer->GetTeam() )
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 25 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 25 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 24 ) );
			
		}
		else
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 11 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 11 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 10 ) );
			
		}
	}
	else if ( Honorablekills >= Rank12 && Honorablekills < Rank13 )
	{
		if ( pPlayer->GetTeam() )
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 26 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 26 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 25 ) );
			
		}
		else
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 12 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 12 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 11 ) );
			
		}
	}
	else if ( Honorablekills >= Rank13 && Honorablekills < Rank14 )
	{
		if ( pPlayer->GetTeam() )
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 27 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 27 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 26 ) );
			
		}
		else
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 13 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 13 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 12 ) );
			
		}
	}
	else if ( Honorablekills >= Rank14 && Honorablekills < Rank15 )
	{
		if ( pPlayer->GetTeam() )
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 28 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 28 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 27 ) );
			
		}
		else
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 14 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 14 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 13 ) );
			
		}
	}
	else if ( Honorablekills >= Rank15 )
	{
		if ( pPlayer->GetTeam() )
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 95 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 95 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 28 ) );
			
		}
		else
		{
			pPlayer->SetKnownTitle( static_cast< RankTitles >( 94 ), true );
			pPlayer->SetUInt32Value( PLAYER_CHOSEN_TITLE, 94 );
			pPlayer->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, pPlayer->GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~uint64(1) << uint8( 14 ) );
			
		}
	}

	if ( Honorablekills == Rank14 )
	{
		if ( pPlayer->GetTeam() )
		{
			char msg[100];
			sprintf(msg, "|cffff0000 %s has achieved High Warlord|r", pPlayer->GetName());
			sWorld.SendWorldWideScreenText(msg);

			char msg2[100];
			sprintf(msg2, "|cffff0000 %s has achieved High Warlord|r", pPlayer->GetName());
			sWorld.SendWorldText(msg); // send message
		}
		else
		{
			char msg[100];
			sprintf(msg, "|cffff0000 %s has achieved Grand Marshall|r", pPlayer->GetName());
			sWorld.SendWorldWideScreenText(msg);

			char msg2[100];
			sprintf(msg2, "|cffff0000 %s has achieved Grand Marshall|r", pPlayer->GetName());
			sWorld.SendWorldText(msg); // send message	
		}
	}
	else if ( Honorablekills == Rank15 )
	{
		if ( pPlayer->GetTeam() )
		{
			char msg[100];
			sprintf(msg, "|cffff0000 Through persistence and dedication, %s is now %s Of the Horde|r", pPlayer->GetName(), pPlayer->GetName());
			sWorld.SendWorldWideScreenText(msg);

			char msg2[100];
			sprintf(msg2, "|cffff0000 Through persistence and dedication, %s is now %s Of the Horde|r", pPlayer->GetName(), pPlayer->GetName());
			sWorld.SendWorldText(msg); // send message
		}
		else
		{
			char msg[100];
			sprintf(msg, "|cffff0000 Through persistence and dedication, %s is now %s Of the Alliance|r", pPlayer->GetName(), pPlayer->GetName());
			sWorld.SendWorldWideScreenText(msg);

			char msg2[100];
			sprintf(msg2, "|cffff0000 Through persistence and dedication, %s is now %s Of the Alliance|r", pPlayer->GetName(), pPlayer->GetName());
			sWorld.SendWorldText(msg); // send message	
		}
	}

}

void OnLogOut(Player * pPlayer)
{
	PvPMgr*mgr = Storage->GetMgr(pPlayer->GetLowGUID(),true);
	
	if (mgr == NULL)
		return;

	mgr->CurrentKills = 0;
	mgr->killingSpree = false;
	mgr->killingSpree2 = false;
	mgr->TotalDeaths = 0;
}
	

void SetupPvPSystem(ScriptMgr *mgr)
{
	Storage = PvPStorage::GetInstance();
	mgr->register_hook(SERVER_HOOK_EVENT_ON_KILL_PLAYER, (void*)OnKillPlayer);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_POST_SPELL_CAST, (void*)OnHealPlayer);

	mgr->register_hook(SERVER_HOOK_EVENT_ON_POST_SPELL_CAST, (bool*)OnDesertedBattleground);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_FIRST_ENTER_WORLD, (void*)ShowCredits);

	mgr->register_hook(SERVER_HOOK_EVENT_ON_HONORABLE_KILL, (void*)PvPTitles);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_LOGOUT, (void*)OnLogOut);
}
*/