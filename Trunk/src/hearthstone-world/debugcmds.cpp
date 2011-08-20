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

/////////////////////////////////////////////////
//  Debug Chat Commands
//

#include "StdAfx.h"

bool ChatHandler::HandleDebugInFrontCommand(const char* args, WorldSession *m_session)
{
	Unit* unit = getSelectedUnit(m_session, true);
	if(unit == NULL)
		return false;

	SystemMessage(m_session, format("Target is %s you.", (m_session->GetPlayer()->isInFront(unit) ? "infront of" : "behind")).c_str());
	return true;
}

bool ChatHandler::HandleShowReactionCommand(const char* args, WorldSession *m_session)
{
	if(args == NULL)
		return false;

	Creature* creature = getSelectedCreature(m_session, true);
	if(creature == NULL)
		return false;

	uint32 Reaction = atol(args);
	if(!Reaction)
		return false;

	WorldPacket data(SMSG_AI_REACTION, 12);
	data << creature->GetGUID() << uint32(Reaction);
	m_session->SendPacket( &data );

	SystemMessage(m_session,  format("Sent forced reaction %u from %s.", Reaction, creature->GetName()).c_str());
	return true;
}

bool ChatHandler::HandleDistanceCommand(const char* args, WorldSession *m_session)
{
	Unit* pUnit = getSelectedChar(m_session, false);
	if(!pUnit)
	{
		pUnit = getSelectedCreature(m_session, false);
		if(!pUnit)
		{
			m_session->GetPlayer()->BroadcastMessage("You must select a Unit.");
			return false;
		}
	}

	SystemMessage(m_session, format("Distance: %f", m_session->GetPlayer()->CalcDistance(pUnit)).c_str());
	return true;
}

bool ChatHandler::HandleMoveInfoCommand(const char* args, WorldSession *m_session)
{
	Player* plr = m_session->GetPlayer();
	Creature* creature = getSelectedCreature(m_session, true);
	if(creature == NULL)
		return false;

	Unit *unitToFollow = creature->GetAIInterface()->getUnitToFollow();
	SystemMessage(m_session, "Move Info:");
	if(unitToFollow != NULL)
		GreenSystemMessage(m_session, format("Following Unit: Low: %u; High: %u;", unitToFollow->GetLowGUID(), unitToFollow->GetHighGUID()).c_str());
	GreenSystemMessage(m_session, format("Distance is: %f;", plr->CalcDistance(creature)).c_str());
	GreenSystemMessage(m_session, format("Mob Facing Player: %s; Player Facing Mob %s;", (creature->isInFront(plr) ? "true" : "false"),
		(plr->isInFront(creature) ? "true" : "false")).c_str());
	GreenSystemMessage(m_session, format("Attackers Count: %u;", uint32(creature->GetAIInterface()->getAITargetsCount())).c_str());
	GreenSystemMessage(m_session, format("Creature State: %u; Run: %s;", creature->GetAIInterface()->m_creatureState,
		(creature->GetAIInterface()->getMoveRunFlag() ? "true" : "false")).c_str());
	GreenSystemMessage(m_session, format("AIState: %u; AIType: %u; AIAgent: %u;", creature->GetAIInterface()->getAIState(), creature->GetAIInterface()->getAIType(),
		creature->GetAIInterface()->getCurrentAgent()).c_str());
	GreenSystemMessage(m_session, format("Movetype: %u; Current Waypoint: %u", creature->GetAIInterface()->getMoveType(),
		creature->GetAIInterface()->getCurrentWaypoint()).c_str());
	return true;
}

bool ChatHandler::HandleDebugSetPhase(const char* args, WorldSession *m_session)
{
	Unit* pUnit = getSelectedChar(m_session, false);
	if(!pUnit)
	{
		pUnit = getSelectedCreature(m_session, false);
		if(!pUnit)
		{
			m_session->GetPlayer()->BroadcastMessage("You must select a Unit.");
			return true;
		}
	}

	int32 phaseId = 0;
	int save = 0;
	if(sscanf(args, "%u %u", &phaseId, &save) < 1)
		return false;

	if(phaseId < -1)
	{
		m_session->GetPlayer()->BroadcastMessage("You must specify a valid phase id.");
		return true;
	}
	if(phaseId == 0)
	{
		pUnit->SetPhaseMask(1);
		m_session->GetPlayer()->BroadcastMessage("Resetting Phase.");
		return true;
	}

	if(pUnit->IsPlayer())
		TO_PLAYER(pUnit)->SetPhaseMask( phaseId, (save ? true : false) );
	else
		pUnit->SetPhaseMask( phaseId );
	m_session->GetPlayer()->BroadcastMessage("Target's phase altered successfully.");
	return true;
}

bool ChatHandler::HandleAIMoveCommand(const char* args, WorldSession *m_session)
{
	Creature* obj = getSelectedCreature(m_session, false);
	if(obj == NULL)
	{
		SystemMessage(m_session, "You should select a creature.");
		return true;
	}

	uint32 Move  = 1;
	uint32 Run  = 0;
	uint32 Time = 0;
	uint32 Meth = 0;

	char* pMove = strtok((char*)args, " ");
	if (pMove)
		Move  = atoi(pMove);

	char* pRun = strtok(NULL, " ");
	if (pRun)
		Run  = atoi(pRun);

	char* pTime = strtok(NULL, " ");
	if (pTime)
		Time  = atoi(pTime);

	char* pMeth = strtok(NULL, " ");
	if (pMeth)
		Meth  = atoi(pMeth);

	float x = m_session->GetPlayer()->GetPositionX();
	float y = m_session->GetPlayer()->GetPositionY();
	float z = m_session->GetPlayer()->GetPositionZ();
	float o = m_session->GetPlayer()->GetOrientation();
	TO_CREATURE(obj)->GetAIInterface()->setMoveRunFlag((Run>0?true:false));
	float distance = TO_CREATURE(obj)->CalcDistance(x,y,z);
	if(Move == 1)
	{
		if(Meth == 1)
		{
			float q = distance-0.5f;
			x = (TO_CREATURE(obj)->GetPositionX()+x*q)/(1+q);
			y = (TO_CREATURE(obj)->GetPositionY()+y*q)/(1+q);
			z = (TO_CREATURE(obj)->GetPositionZ()+z*q)/(1+q);
		}
		else if(Meth == 2)
		{
			float q = distance-1;
			x = (TO_CREATURE(obj)->GetPositionX()+x*q)/(1+q);
			y = (TO_CREATURE(obj)->GetPositionY()+y*q)/(1+q);
			z = (TO_CREATURE(obj)->GetPositionZ()+z*q)/(1+q);
		}
		else if(Meth == 3)
		{
			float q = distance-2;
			x = (TO_CREATURE(obj)->GetPositionX()+x*q)/(1+q);
			y = (TO_CREATURE(obj)->GetPositionY()+y*q)/(1+q);
			z = (TO_CREATURE(obj)->GetPositionZ()+z*q)/(1+q);
		}
		else if(Meth == 4)
		{
			float q = distance-2.5f;
			x = (TO_CREATURE(obj)->GetPositionX()+x*q)/(1+q);
			y = (TO_CREATURE(obj)->GetPositionY()+y*q)/(1+q);
			z = (TO_CREATURE(obj)->GetPositionZ()+z*q)/(1+q);
		}
		else if(Meth == 5)
		{
			float q = distance-3;
			x = (TO_CREATURE(obj)->GetPositionX()+x*q)/(1+q);
			y = (TO_CREATURE(obj)->GetPositionY()+y*q)/(1+q);
			z = (TO_CREATURE(obj)->GetPositionZ()+z*q)/(1+q);
		}
		else if(Meth == 6)
		{
			float q = distance-3.5f;
			x = (TO_CREATURE(obj)->GetPositionX()+x*q)/(1+q);
			y = (TO_CREATURE(obj)->GetPositionY()+y*q)/(1+q);
			z = (TO_CREATURE(obj)->GetPositionZ()+z*q)/(1+q);
		}
		else
		{
			float q = distance-4;
			x = (TO_CREATURE(obj)->GetPositionX()+x*q)/(1+q);
			y = (TO_CREATURE(obj)->GetPositionY()+y*q)/(1+q);
			z = (TO_CREATURE(obj)->GetPositionZ()+z*q)/(1+q);
		}
		TO_CREATURE(obj)->GetAIInterface()->MoveTo(x,y,z);
	}
	else
	{
		uint32 moveTime = 0;
		if(!Time)
		{
			//float dx = x - TO_CREATURE(obj)->GetPositionX();
			//float dy = y - TO_CREATURE(obj)->GetPositionY();
			//float dz = z - TO_CREATURE(obj)->GetPositionZ();

			//float distance = sqrt((dx*dx) + (dy*dy) + (dz*dz));
			if(!distance)
			{
				SystemMessage(m_session, "The Creature is already there.");
				return true;
			}

			float moveSpeed = 1.0f;
			if(!Run)
			{
				moveSpeed = 2.5f*0.001f;
			}
			else
			{
				moveSpeed = 7.0f*0.001f;
			}

			moveTime = uint32(distance / moveSpeed);
		}
		else
		{
			moveTime = Time;
		}
		//TO_CREATURE(obj)->setMovementState(MOVING);
		TO_CREATURE(obj)->GetAIInterface()->SendMoveToPacket(x,y,z,o,moveTime,Run);
	}
	return true;
}

bool ChatHandler::HandleFaceCommand(const char* args, WorldSession *m_session)
{
	Creature* obj = getSelectedCreature(m_session, false);
	if(obj == NULL)
	{
		SystemMessage(m_session,  "You should select a creature.");
		return true;
	}

	uint32 Orentation = 0;
	char* pOrentation = strtok((char*)args, " ");
	if (pOrentation)
		Orentation  = atoi(pOrentation);

	/* Convert to Blizzards Format */
	float theOrientation = Orentation/(360/float(6.28));

	obj->SetPosition(obj->GetPositionX(), obj->GetPositionY(), obj->GetPositionZ(), theOrientation, false);
	OUT_DEBUG("facing sent");
	return true;
}

bool ChatHandler::HandleSetBytesCommand(const char* args, WorldSession *m_session)
{
	Object* obj;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if(guid != 0)
	{
		if(!(obj = m_session->GetPlayer()->GetMapMgr()->GetUnit(guid)))
		{
			SystemMessage(m_session, "You should select a character or a creature.");
			return true;
		}
	}
	else
		obj = TO_OBJECT(m_session->GetPlayer());

	char* pBytesIndex = strtok((char*)args, " ");
	if (!pBytesIndex)
		return false;

	uint32 BytesIndex  = atoi(pBytesIndex);

	char* pValue1 = strtok(NULL, " ");
	if (!pValue1)
		return false;

	uint8 Value1  = static_cast<uint8>(atol(pValue1));

	char* pValue2 = strtok(NULL, " ");
	if (!pValue2)
		return false;

	uint8 Value2  = static_cast<uint8>(atol(pValue2));

	char* pValue3 = strtok(NULL, " ");
	if (!pValue3)
		return false;

	uint8 Value3  = static_cast<uint8>(atol(pValue3));

	char* pValue4 = strtok(NULL, " ");
	if (!pValue4)
		return false;

	uint8 Value4  = static_cast<uint8>(atol(pValue4));

	std::stringstream sstext;
	sstext << "Set Field " << BytesIndex << " bytes to " << uint16((uint8)Value1) << " " << uint16((uint8)Value2) << " " << uint16((uint8)Value3) << " " << uint16((uint8)Value4) << '\0';
	obj->SetUInt32Value(BytesIndex, ( ( Value1 ) | ( Value2 << 8 ) | ( Value3 << 16 ) | ( Value4 << 24 ) ) );
	SystemMessage(m_session, sstext.str().c_str());
	return true;
}

bool ChatHandler::HandleGetBytesCommand(const char* args, WorldSession *m_session)
{
	Object* obj;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid != 0)
	{
		if(!(obj = m_session->GetPlayer()->GetMapMgr()->GetUnit(guid)))
		{
			SystemMessage(m_session, "You should select a character or a creature.");
			return true;
		}
	}
	else
		obj = TO_OBJECT(m_session->GetPlayer());

	char* pBytesIndex = strtok((char*)args, " ");
	if (!pBytesIndex)
		return false;

	uint32 BytesIndex  = atoi(pBytesIndex);
	uint32 theBytes = obj->GetUInt32Value(BytesIndex);
	/*
	switch (Bytes)
	{
	case 0:
	theBytes = obj->GetUInt32Value(UNIT_FIELD_BYTES_0);
	break;
	case 1:
	theBytes = obj->GetUInt32Value(UNIT_FIELD_BYTES_1);
	break;
	case 2:
	theBytes = obj->GetUInt32Value(UNIT_FIELD_BYTES_2);
	break;
	}
	*/
	std::stringstream sstext;
	sstext << "bytes for Field " << BytesIndex << " are " << uint16((uint8)theBytes & 0xFF) << " " << uint16((uint8)(theBytes >> 8) & 0xFF) << " ";
	sstext << uint16((uint8)(theBytes >> 16) & 0xFF) << " " << uint16((uint8)(theBytes >> 24) & 0xFF) << '\0';

	SystemMessage(m_session, sstext.str().c_str());
	return true;
}
bool ChatHandler::HandleDebugLandWalk(const char* args, WorldSession *m_session)
{
	Player* chr = getSelectedChar(m_session);
	char buf[256];

	if (chr == NULL) // Ignatich: what should NOT happen but just in case...
	{
		SystemMessage(m_session, "No character selected.");
		return false;
	}
	chr->SetMovement(MOVE_LAND_WALK,8);
	snprintf((char*)buf,256, "Land Walk Test Ran.");
	SystemMessage(m_session, buf);
	return true;
}

bool ChatHandler::HandleDebugWaterWalk(const char* args, WorldSession *m_session)
{
	Player* chr = getSelectedChar(m_session);
	if (chr == NULL) // Ignatich: what should NOT happen but just in case...
	{
		SystemMessage(m_session, "No character selected.");
		return false;
	}

	char buf[256];
	chr->SetMovement(MOVE_WATER_WALK, 4);
	snprintf((char*)buf,256, "Water Walk Test Ran.");
	SystemMessage(m_session,  buf);
	return true;
}

bool ChatHandler::HandleDebugUnroot(const char* args, WorldSession *m_session)
{
	Player* chr = getSelectedChar(m_session);
	if (chr == NULL) // Ignatich: what should NOT happen but just in case...
	{
		SystemMessage(m_session,  "No character selected.");
		return false;
	}

	chr->SetMovement(MOVE_UNROOT,5);

	char buf[256];
	snprintf((char*)buf,256, "UnRoot Test Ran.");
	SystemMessage(m_session, buf);
	return true;
}

bool ChatHandler::HandleDebugRoot(const char* args, WorldSession *m_session)
{
	Player* chr = getSelectedChar(m_session);
	if (chr == NULL) // Ignatich: what should NOT happen but just in case...
	{
		SystemMessage(m_session, "No character selected.");
		return true;
	}
	chr->SetMovement(MOVE_ROOT,1);

	char buf[256];
	snprintf((char*)buf,256, "Root Test Ran.");
	SystemMessage(m_session, buf);
	return true;
}

bool ChatHandler::HandleAggroRangeCommand(const char* args, WorldSession *m_session)
{
	Unit* obj = NULLUNIT;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid != 0)
	{
		if(!(obj = m_session->GetPlayer()->GetMapMgr()->GetUnit(guid)))
		{
			SystemMessage(m_session,  "You should select a character or a creature.");
			return true;
		}
	}
	else
	{
		SystemMessage(m_session, "You should select a character or a creature.");
		return true;
	}

	float aggroRange = obj->GetAIInterface()->_CalcAggroRange(m_session->GetPlayer());
	std::stringstream sstext;
	sstext << "Aggrorange is: " << sqrtf(aggroRange) <<'\0';

	SystemMessage(m_session, sstext.str().c_str());

	return true;
}

bool ChatHandler::HandleKnockBackCommand(const char* args, WorldSession *m_session)
{
	float hspeed, vspeed;
	Unit* target = getSelectedChar(m_session, true);
	if(sscanf(args, "%f %f", &hspeed, &vspeed) != 2)
		return false;

	target->knockback(hspeed, vspeed, false);
	return true;
}

bool ChatHandler::HandleFadeCommand(const char* args, WorldSession *m_session)
{
	Unit* target = m_session->GetPlayer()->GetMapMgr()->GetUnit(m_session->GetPlayer()->GetSelection());
	if(!target)
		target = m_session->GetPlayer();
	char* v = strtok((char*)args, " ");
	if(!v)
		return false;

	target->ModThreatModifier(atoi(v));

	std::stringstream sstext;
	sstext << "threat is now reduced by: " << target->GetThreatModifier() <<'\0';

	SystemMessage(m_session, sstext.str().c_str());
	return true;
}

bool ChatHandler::HandleThreatModCommand(const char* args, WorldSession *m_session)
{
	Unit* target = m_session->GetPlayer()->GetMapMgr()->GetUnit(m_session->GetPlayer()->GetSelection());
	if(!target)
		target = m_session->GetPlayer();
	char* v = strtok((char*)args, " ");
	if(!v)
		return false;

	target->ModGeneratedThreatModifier(atoi(v));

	std::stringstream sstext;
	sstext << "new threat caused is now reduced by: " << target->GetGeneratedThreatModifier() << "%" <<'\0';

	SystemMessage(m_session, sstext.str().c_str());
	return true;
}

bool ChatHandler::HandleCalcThreatCommand(const char* args, WorldSession *m_session)
{
	Unit* target = m_session->GetPlayer()->GetMapMgr()->GetUnit(m_session->GetPlayer()->GetSelection());
	if(!target)
	{
		SystemMessage(m_session, "You should select a creature.");
		return true;
	}
	char* dmg = strtok((char*)args, " ");
	if(!dmg)
		return false;
	char* spellId = strtok(NULL, " ");
	if(!spellId)
		return false;

	uint32 threat = target->GetAIInterface()->_CalcThreat(atol(dmg), dbcSpell.LookupEntry( atoi( spellId ) ), m_session->GetPlayer());

	std::stringstream sstext;
	sstext << "generated threat is: " << threat <<'\0';

	SystemMessage(m_session, sstext.str().c_str());
	return true;
}

bool ChatHandler::HandleThreatListCommand(const char* args, WorldSession *m_session)
{
	Unit* target = NULLUNIT;
	target = m_session->GetPlayer()->GetMapMgr()->GetUnit(m_session->GetPlayer()->GetSelection());
	if(!target)
	{
		SystemMessage(m_session, "You should select a creature.");
		return true;
	}

	std::stringstream sstext;
	sstext << "threatlist of creature: " << GUID_LOPART(m_session->GetPlayer()->GetSelection()) << " " << GUID_HIPART(m_session->GetPlayer()->GetSelection()) << '\n';
	TargetMap::iterator itr;
	for(itr = target->GetAIInterface()->GetAITargets()->begin(); itr != target->GetAIInterface()->GetAITargets()->end();)
	{
		if(!itr->second)
		{
			++itr;
			continue;
		}
		sstext << "guid: " << itr->first->GetGUID() << " | threat: " << itr->second << "| threat after mod: " << (itr->second + itr->first->GetThreatModifier()) << "\n";
		++itr;
	}

	SendMultilineMessage(m_session, sstext.str().c_str());
	return true;
}
bool ChatHandler::HandleSendItemPushResult(const char* args, WorldSession* m_session)
{
	uint32 itemid, count, type, unk1, unk2, unk3, unk4;
	char* arg = const_cast<char*>(args);
	itemid = atol(strtok(arg, " "));
	if(!itemid) return false;
	count = atol(strtok(NULL, " "));
	type = atol(strtok(NULL, " "));
	unk1 = atol(strtok(NULL, " "));
	unk2 = atol(strtok(NULL, " "));
	unk3 = atol(strtok(NULL, " "));
	unk4 = atol(strtok(NULL, " "));

	// lookup item
//	ItemPrototype *proto = ItemPrototypeStorage.LookupEntry(itemid);

	WorldPacket data;
	data.SetOpcode(SMSG_ITEM_PUSH_RESULT);

	data << m_session->GetPlayer()->GetGUID();			   // recivee_guid
	data << type << uint32(1);  // unk
	data << count;			  // count
	data << uint8(0xFF);				// uint8 unk const 0xFF
	data << unk1;	   // unk
	data << itemid;
	data << unk2;		  // unk
	data << unk3;		 // random prop
	data << unk4;
	m_session->SendPacket(&data);
	return true;
	//data << ((proto != NULL) ? proto->Quality : uint32(0)); // quality
}

bool ChatHandler::HandleModifyBitCommand(const char* args, WorldSession* m_session)
{

	Object* obj;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid != 0)
	{
		if(!(obj = m_session->GetPlayer()->GetMapMgr()->GetUnit(guid)))
		{
			SystemMessage(m_session, "You should select a character or a creature.");
			return true;
		}
	}
	else
		obj = TO_OBJECT(m_session->GetPlayer());

	char* pField = strtok((char*)args, " ");
	if (!pField)
		return false;

	char* pBit = strtok(NULL, " ");
	if (!pBit)
		return false;

	uint16 field = atoi(pField);
	uint32 bit   = atoi(pBit);

	if (field < 1 || field >= PLAYER_END)
	{
		SystemMessage(m_session, "Incorrect values.");
		return true;
	}

	if (bit < 1 || bit > 32)
	{
		SystemMessage(m_session, "Incorrect values.");
		return true;
	}

	char buf[256];

	if ( obj->HasFlag( field, (1<<(bit-1)) ) )
	{
		obj->RemoveFlag( field, (1<<(bit-1)) );
		snprintf((char*)buf,256, "Removed bit %i in field %i.", (unsigned int)bit, (unsigned int)field);
	}
	else
	{
		obj->SetFlag( field, (1<<(bit-1)) );
		snprintf((char*)buf,256, "Set bit %i in field %i.", (unsigned int)bit, (unsigned int)field);
	}

	SystemMessage(m_session, buf);
	return true;
}

bool ChatHandler::HandleModifyValueCommand(const char* args,  WorldSession* m_session)
{
	Object* obj;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid != 0)
	{
		if(!(obj = TO_OBJECT(m_session->GetPlayer())->GetMapMgr()->GetUnit(guid)))
		{
			SystemMessage(m_session, "You should select a character or a creature.");
			return true;
		}
	}
	else
		obj = TO_OBJECT(m_session->GetPlayer());

	char* pField = strtok((char*)args, " ");
	if (!pField)
		return false;

	char* pValue = strtok(NULL, " ");
	if (!pValue)
		return false;

	uint16 field = atoi(pField);
	uint32 value   = atoi(pValue);

	if (field < 1 || field >= PLAYER_END)
	{
		SystemMessage(m_session, "Incorrect Field.");
		return true;
	}
/*
	if (value > sizeof(uint32))
	{
		FillSystemMessageData(&data, m_session, "Incorrect Value.");
		m_session->SendPacket( &data );
		return true;
	}
*/
	char buf[256];
	uint32 oldValue = obj->GetUInt32Value(field);
	obj->SetUInt32Value(field,value);

	snprintf((char*)buf,256,"Set Field %i from %i to %i.", (unsigned int)field, (unsigned int)oldValue, (unsigned int)value);

	if( obj->IsPlayer() )
		TO_PLAYER( obj )->UpdateChances();

	SystemMessage(m_session, buf);

	return true;
}

bool ChatHandler::HandleDebugDumpCoordsCommmand(const char * args, WorldSession * m_session)
{
	Player* p = m_session->GetPlayer();
	//char buffer[200] = {0};
	FILE * f = fopen("C:\\script_dump.txt", "a");
	if(!f) return true;

	fprintf(f, "{ %f, %f, %f, %f },\n", p->GetPositionX(), p->GetPositionY(), p->GetPositionZ(),
		p->GetOrientation());
	fclose(f);
	return true;
}

//As requested by WaRxHeAd for database development.
//This should really only be available in special cases and NEVER on real servers... -DGM
bool ChatHandler::HandleSQLQueryCommand(const char* args, WorldSession *m_session)
{
	#ifdef _ONLY_FOOLS_TRY_THIS_
	if(!*args)
	{
		RedSystemMessage(m_session, "No query given.");
		return false;
	}

	bool isok = WorldDatabase.Execute(args);

	if(isok)
		GreenSystemMessage(m_session, "Query was executed successfully.");
	else
		RedSystemMessage(m_session, "Query failed to execute.");

	#endif

	return true;
}

//#define _ONLY_FOOLS_TRY_THIS_

bool ChatHandler::HandleSendpacket(const char * args, WorldSession * m_session)
{
#ifdef _ONLY_FOOLS_TRY_THIS_

	uint32 arg_len = strlen(args);
	char * xstring = new char [arg_len];
	memcpy(xstring, args,arg_len);

	for (uint32 i = 0; i < arg_len; i++)
	{
		if (xstring[i] == ' ')
		{
			xstring[i] = '\0';
		}
	}

	// we receive our packet as hex, that means we get it like ff ff ff ff
	// the opcode consists of 2 bytes

	if (!xstring)
	{
		OUT_DEBUG("[Debug][Sendpacket] Packet is invalid");
		return false;
	}

	WorldPacket data(arg_len);

	uint32 loop = 0;
	uint16 opcodex = 0;
	uint16 opcodez = 0;

	// get the opcode
	sscanf(xstring , "%x", &opcodex);

	// should be around here
	sscanf(&xstring[3] , "%x", &opcodez);

	opcodex =  opcodex << 8;
	opcodex |= opcodez;
	data.Initialize(opcodex);


	int j = 3;
	int x = 0;
	do
	{
		if (xstring[j] == '\0')
		{
			uint32 HexValue;
			sscanf(&xstring[j+1], "%x", &HexValue);
			if (HexValue > 0xFF)
			{
				OUT_DEBUG("[Debug][Sendpacket] Packet is invalid");
				return false;
			}
			data << uint8(HexValue);
			//++j;
		}
		++j;
	} while(j < arg_len);

	data.hexlike();

	m_session->SendPacket(&data);

	OUT_DEBUG("[Debug][Sendpacket] Packet was send");
#endif
	return true;
}
bool ChatHandler::HandleDebugGoDamage(const char* args, WorldSession *m_session)
{
	GameObject* GObj = NULLGOB;

	GObj = m_session->GetPlayer()->m_GM_SelectedGO;
	if( !GObj )
	{
		RedSystemMessage(m_session, "No selected GameObject...");
		return true;
	}
	uint32 damage = (float)atof(args);
	GObj->TakeDamage(uint32(damage),  m_session->GetPlayer(), m_session->GetPlayer(), 5555);
	BlueSystemMessage(m_session, "Gameobject Has Taken %u", damage);
	return true;
}

bool ChatHandler::HandleDebugGoRepair(const char* args, WorldSession *m_session)
{
	GameObject* GObj = NULLGOB;

	GObj = m_session->GetPlayer()->m_GM_SelectedGO;
	if( !GObj )
	{
		RedSystemMessage(m_session, "No selected GameObject...");
		return true;
	}
	GObj->Rebuild();

	BlueSystemMessage(m_session, "Gameobject rebuilt.");
	return true;

}
