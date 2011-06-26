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

#include "RStdAfx.h"

static const uint32 DKNodesMask[12] = {4294967295,4093640703,830406655,0,33570816,1310944,3250593812,73752,896,67111952,0,0};//all old continents are available to DK's by default.

Player::Player( uint32 guid )
{
	pguid = guid;
}

void Player::Init()
{

}

Player::~Player ( )
{

}

///====================================================================
///  Create
///  params: p_newChar
///  desc:   data from client to create a new character
///====================================================================
int Player::Create(WorldPacket& data )
{
	uint8 skin, face, hairStyle, hairColor, facialHair, outfitId;

	// unpack data into member variables
	data >> m_name;

	// correct capitalization
	CapitalizeString(m_name);

	data >> race >> class_ >> gender >> skin >> face;
	data >> hairStyle >> hairColor >> facialHair >> outfitId;

	info = sClientMgr.GetPlayerCreateInfo(race, class_);
	if(!info)
		return 2;

	m_mapId = info->mapId;
	m_zoneId = info->zoneId;
	SetPosition( info->positionX, info->positionY, info->positionZ, info->Orientation);
	m_bind_pos_x = info->positionX;
	m_bind_pos_y = info->positionY;
	m_bind_pos_z = info->positionZ;
	m_bind_mapid = info->mapId;
	m_bind_zoneid = info->zoneId;
	load_health = info->health;
	load_mana = info->mana;

	memset(m_taximask, 0, sizeof(uint32)*14);

	// set race dbc
	myRace = dbcCharRace.LookupEntry(race);
	myClass = dbcCharClass.LookupEntry(class_);
	if(!myRace || !myClass)
		return 2;

	uint8 powertype = uint8(myClass->power_type);

	// Automatically add the race's taxi hub to the character's taximask at creation time ( 1 << (taxi_node_id-1) )
	memset(m_taximask,0,sizeof(m_taximask));
	if(class_ == DEATHKNIGHT)
	{
		for(uint8 i=0;i<14;++i)
			m_taximask[i] |= DKNodesMask[i];
	}

	switch(race)
	{
	case RACE_TAUREN:	AddTaximaskNode(22);						break;
	case RACE_HUMAN:	AddTaximaskNode(2);							break;
	case RACE_DWARF:	AddTaximaskNode(6);							break;
	case RACE_GNOME:	AddTaximaskNode(6);							break;
	case RACE_ORC:		AddTaximaskNode(23);						break;
	case RACE_TROLL:	AddTaximaskNode(23);						break;
	case RACE_UNDEAD:	AddTaximaskNode(11);						break;
	case RACE_NIGHTELF:	{AddTaximaskNode(26); AddTaximaskNode(27);}	break;
	case RACE_BLOODELF:	AddTaximaskNode(82);						break;
	case RACE_DRAENEI:	AddTaximaskNode(94);						break;
	}
	// team dependant taxi node
	AddTaximaskNode(100-myRace->team_id);
	level = uint32(class_ != DEATHKNIGHT ? 1: 55);
	bytes1 = (((skin) | (face << 8) | (hairStyle << 16) | (hairColor << 24)));
	//PLAYER_BYTES_2							   GM ON/OFF	 BANKBAGSLOTS   RESTEDSTATE
	bytes2 = ((facialHair | (0x02 << 24)));//no bank slot by default!

	for(std::set<uint32>::iterator sp = info->spell_list.begin();sp!=info->spell_list.end();sp++)
	{
		mSpells.insert((*sp));
	}

	for(std::list<CreateInfo_SkillStruct>::iterator ss = info->skills.begin(); ss!=info->skills.end(); ss++)
		_AddSkillLine(ss->skillid, ss->currentval, ss->maxval);

	// Add actionbars
	for(std::list<CreateInfo_ActionBarStruct>::iterator itr = info->actionbars.begin();itr!=info->actionbars.end();itr++)
	{
		setAction(itr->button, itr->action, itr->type, itr->misc);
	}
	return 0;
}

void Player::SaveToDB(bool bNewCharacter /* =false */)
{
	QueryBuffer * buf = new QueryBuffer;

	std::stringstream ss;
	ss << "REPLACE INTO characters VALUES ("
	<< pguid << ", "
	<< m_session->GetAccountId() << ","

	// stat saving
	<< "'" << m_name << "', "
	<< uint32(race) << ","
	<< uint32(class_) << ","
	<< uint32(gender) << ","
	<< uint32(0) << ",";

	ss << uint32(0) << ",";
	ss << uint32(level) << ","
	<< uint32(0) << ","
	<< uint32(0) << ","

	// dump exploration data
	<< "'";

	for(uint32 i = 0; i < 128; i++)
		ss << uint32(0) << ",";

	ss << "','0', "; //skip saving oldstyle skills, just fill with 0

	ss << uint32(0) << ","
	<< uint32(0) << ","
	<< uint64(0) << ","
	<< uint64(0) << ","
	<< uint64(0) << ","
	<< uint32(0) << ","
	<< uint32(0) << ","
	<< uint32(0) << ","
	<< uint32(0) << ","
	<< load_health << ","
	<< load_mana << ","
	<< uint32(0) << ","
	<< bytes1 << ","
	<< bytes2 << ","
	<< uint32(0) << ","
	<< bytes1 << ","
	<< m_position.x << ", "
	<< m_position.y << ", "
	<< m_position.z << ", "
	<< m_position.o << ", "
	<< m_mapId << ", ";

	ss << m_zoneId << ", '";

	for(uint8 i = 0; i < 14; i++ )
		ss << m_taximask[i] << " ";
	ss << "', "

	<< uint32(0) << ", '', "
	<< (uint32)UNIXTIME << ",";
	ss << "0,"
	<< m_bind_pos_x << ", "
	<< m_bind_pos_y << ", "
	<< m_bind_pos_z << ", "
	<< m_bind_mapid << ", "
	<< m_bind_zoneid << ", "
	<< uint32(0) << ", "
	<< uint32(0) << ", "
	<< uint32(0) << ", '"
	<< uint32(0) << " "
	<< uint32(0) << " "
	<< uint32(0) << " ', "
	<< uint32(0) << ", "

	<< uint32(0) << ", "
	<< uint32(1) << ", " // First Login
	<< uint32(0) << ","
	<< uint32(0) << ","
	<< uint32(0) << ",";

	// instances
	ss << uint32(0) << ", "
	<< uint32(0) << ", "
	<< float(0) << ", "
	<< float(0) << ", "
	<< float(0) << ", "
	<< float(0) << ", "
	<< uint32(0) << ", ";
	ss << "0, 0, 0";
	ss << "," << uint32(0);
	ss << ",'" << float(0) << "','" << float(0) << "','" << float(0) << "'";
	ss << ",'";
	ss << "','";

	// Add player action bars
	for(uint32 i = 0; i < 132; i++)
	{
		ss << uint32(mActions[i].Action) << ","
			<< uint32(mActions[i].Type) << ","
			<< uint32(mActions[i].Misc) << ",";
	}
	ss << "','";
	ss << "','";
	ss << "','";
	ss << "', ";
	ss << uint32(0) << ", ";
	ss << uint32(0) << ", " << uint32(0) << ", " << uint32(0) << ", ";
	ss << uint32(0) << ", " << uint32(0) << ", ";
	ss << uint32(0) << ", ";
	ss << uint32(0) << ", ";
	ss << uint32(0) << ", ";

	ss << uint32(0) << ", ";
	ss << uint32(0) << ", ";

	ss << "0, 0)";	// Reset for talents and position

	buf->AddQueryStr(ss.str());
	_SaveSpellsToDB(buf);
	CharacterDatabase.AddQueryBuffer(buf);
}

void Player::_SaveSpellsToDB(QueryBuffer * buf)
{
	// Dump spell data to stringstream
	std::stringstream ss;
	ss << "INSERT INTO playerspells VALUES ";
	std::set<uint32>::iterator spellItr = mSpells.begin();
	bool first = true;
	for(; spellItr != mSpells.end(); ++spellItr)
	{
		SpellEntry * sp = dbcSpell.LookupEntry( *spellItr );
		if( !sp )
			continue;

		if(!first)
			ss << ",";
		else
			first = false;

		ss << "("<< pguid << "," << uint32(*spellItr) << ")";
	}
	if(buf == NULL)
		CharacterDatabase.Execute(ss.str().c_str());
	else
		buf->AddQueryStr(ss.str());
}

void Player::setAction(uint8 button, uint16 action, uint8 type, uint8 misc)
{
	if( button > 132 - 1 )
		return;

	mActions[button].Action = action;
	mActions[button].Type = type;
	mActions[button].Misc = misc;
}

bool Player::SetPosition( float newX, float newY, float newZ, float newOrientation)
{
	m_position.ChangeCoords(newX, newY, newZ, newOrientation);
	return true;
}

void Player::SetTaximaskNode(uint32 nodeidx, bool Unset)
{
	uint8  field   = uint8((nodeidx - 1) / 32);
	uint32 submask = 1<<((nodeidx-1)%32);
	if(Unset)
	{
		//We have this node allready? Remove it
		if ((GetTaximask(field)& submask) == submask)
			SetTaximask(field,(GetTaximask(field)& ~submask));
	}
	else
	{
		//We don't have this node allready? Add it.
		if ((GetTaximask(field)& submask) != submask)
			SetTaximask(field,(GetTaximask(field)|submask));
	}
}

void Player::_AddSkillLine(uint32 SkillLine, uint32 Curr_sk, uint32 Max_sk)
{
	skilllineentry * CheckedSkill = dbcSkillLine.LookupEntry(SkillLine);
	if (!CheckedSkill) //skill doesn't exist, exit here
		return;

	uint32 customlvlcapskcap = 50+(80*5);

	// force to be within limits
	Max_sk = Max_sk > customlvlcapskcap ? customlvlcapskcap : Max_sk;
	Curr_sk = Curr_sk > Max_sk ? Max_sk : Curr_sk < 1 ? 1 : Curr_sk ;

	std::map<uint32, PlayerSkill>::iterator itr = m_skills.find(SkillLine);
	if(itr != m_skills.end())
	{
		if( (Curr_sk > itr->second.CurrentValue && Max_sk >= itr->second.MaximumValue) || (Curr_sk == itr->second.CurrentValue && Max_sk > itr->second.MaximumValue) )
		{
			itr->second.CurrentValue = Curr_sk;
			itr->second.MaximumValue = Max_sk;
		}
	}
	else
	{
		PlayerSkill inf;
		inf.Skill = CheckedSkill;
		inf.MaximumValue = Max_sk;
		inf.CurrentValue = Curr_sk;
		inf.BonusValue = 0;
		m_skills.insert( make_pair( SkillLine, inf ) );
	}
}
