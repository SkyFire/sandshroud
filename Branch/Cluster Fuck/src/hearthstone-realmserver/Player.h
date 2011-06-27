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

#ifndef _PLAYER_H
#define _PLAYER_H

class SERVER_DECL Player
{
	friend class WorldSession;
	friend class Pet;
	friend class SkillIterator;

public:
	Player ( uint32 guid );
	~Player ( );
	virtual void Init();
	int Create ( WorldPacket &data );
	CharRaceEntry * myRace;
	CharClassEntry * myClass;

	/************************************************************************/
	/* Player loading and savings                                           */
	/* Serialize character to db                                            */
	/************************************************************************/
	void SaveToDB(bool bNewCharacter);
	bool SetPosition( float newX, float newY, float newZ, float newOrientation);
	HEARTHSTONE_INLINE void SetTaximask (uint8 index, uint32 value ) { m_taximask[index] = value; }
	const uint32& GetTaximask( uint8 index ) const { return m_taximask[index]; }
	void AddTaximaskNode(uint32 nodeidx){SetTaximaskNode(nodeidx, false);}
	void SetTaximaskNode(uint32 nodeidx, bool UnSet = false);
	void _AddSkillLine(uint32 SkillLine, uint32 Current, uint32 Max);
	void setAction(uint8 button, uint16 action, uint8 type, uint8 misc);
	void _SaveSpellsToDB(QueryBuffer * buf);
	void _SaveSkillsToDB(QueryBuffer * buf);
	void _SaveItemsToDB(QueryBuffer * buf);

	Session* m_session;
	uint32 pguid;
	uint8 race, class_, gender, level;
	uint32 load_health, load_mana, bytes1, bytes2;
	uint32 m_taximask[14];

	// bind
	float m_bind_pos_x;
	float m_bind_pos_y;
	float m_bind_pos_z;
	uint32 m_bind_mapid;
	uint32 m_bind_zoneid;

private:
	std::string m_name;	// max 21 character name
	PlayerCreateInfo *info;
	LocationVector m_position;
	std::set<uint32> mSpells;
	std::map<uint32, PlayerSkill> m_skills;
	std::map<uint32, CreateInfo_ItemStruct> mIteminfo;

	//! Zone id.
	uint32 m_zoneId;
	//! Continent/map id.
	uint32 m_mapId;
	ActionButton mActions[132];

	uint8 skin, face, hairStyle, hairColor, facialHair, outfitId;
};

#endif
