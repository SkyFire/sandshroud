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

Vehicle::Vehicle(uint64 guid) : Creature(guid)
{
	m_vehicleEntry = 0;
	m_ppassengerCount = 0;
	m_maxPassengers = 0;
	m_seatSlotMax = 0;
	m_isVehicle = true;
	Initialised = false;
	m_CreatedFromSpell = false;
	m_mountSpell = 0;

	for(uint8 i = 0; i < 8; i++)
	{
		m_vehicleSeats[i] = NULL;
		seatisusable[i] = false;
		m_passengers[i] = NULL;
	}
}

Vehicle::~Vehicle()
{	
	m_ppassengerCount = 0;
	if( IsInWorld() )
		RemoveFromWorld(false, true);
}

void Vehicle::Init()
{
	Creature::Init();
}

void Vehicle::InitSeats(uint32 vehicleEntry, Player* pRider)
{
	DisableAI();
	m_maxPassengers = 0;
	m_seatSlotMax = 0;
	SetVehicleEntry(vehicleEntry);
	VehicleEntry * ve = dbcVehicle.LookupEntry( vehicleEntry );
	if(!ve)
	{
		if(sLog.IsOutDevelopement())
			printf("Attempted to create non-existant vehicle %u.\n", vehicleEntry);
		else
			OUT_DEBUG("Attempted to create non-existant vehicle %u.", vehicleEntry);
		return;
	}

	for( uint32 i = 0; i < 8; i++ )
	{
		if( ve->m_seatID[i] )
		{
			m_vehicleSeats[i] = dbcVehicleSeat.LookupEntry( ve->m_seatID[i] );
			m_seatSlotMax = i + 1;

			if(m_vehicleSeats[i]->IsUsable())
			{
				seatisusable[i] = true;
				++m_maxPassengers;
			}
		}
	}

	Initialised = true;

	if( pRider != NULL)
		AddPassenger( pRider );
}

void Vehicle::InstallAccessories()
{
	//Disabled.
	CreatureProtoVehicle* acc = CreatureProtoVehicleStorage.LookupEntry(GetEntry());

	if(acc == NULL)
	{
		sLog.outDetail("Vehicle %u has no accessories.", GetEntry());
		return;
	}

	MapMgr* map = (GetMapMgr() ? GetMapMgr() : sInstanceMgr.GetMapMgr(GetMapId()));
	if(map == NULL) // Shouldn't ever really happen.
		return;

	for(int i = 0; i < 8; i++)
	{
		AccessoryInfo accessories = acc->accessories[i];
		if(!accessories.accessoryentry)
			continue;

		if(m_vehicleSeats[i] == NULL)
		{
			sLog.outDetail("No seatmap for selected seat.\n");
			continue;
		}

		// Load the Proto
		CreatureProto* proto = CreatureProtoStorage.LookupEntry(accessories.accessoryentry);
		CreatureInfo* info = CreatureNameStorage.LookupEntry(accessories.accessoryentry);

		if(!proto || !info)
		{
			sLog.outError("No proto/info for vehicle accessory %u in vehicle %u", accessories.accessoryentry, GetEntry());
			continue;
		}

		// Remove any passengers.
		if(m_passengers[i])
			RemovePassenger(m_passengers[i]);

		// Create the Unit!
		if(proto->vehicle_entry > 0) // Vehicle
		{
			Vehicle* pass = map->CreateVehicle(accessories.accessoryentry);
			if(pass != NULL)
			{
				pass->Load(proto, (IsInInstance() ? map->iInstanceMode : MODE_5PLAYER_NORMAL),
					GetPositionX()+m_vehicleSeats[i]->m_attachmentOffsetX,
					GetPositionY()+m_vehicleSeats[i]->m_attachmentOffsetY,
					GetPositionZ()+m_vehicleSeats[i]->m_attachmentOffsetZ);

				pass->Init();
				pass->m_TransporterGUID = GetGUID();
				pass->InitSeats(proto->vehicle_entry);
				if(pass->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK))
					pass->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK); // Accessory

				AddPassenger(pass, i, true);
				pass->PushToWorld(map);
			}
		}
		else
		{
			Creature* pass = map->CreateCreature(accessories.accessoryentry);
			if(pass != NULL)
			{
				pass->Load(proto, map->iInstanceMode,
					GetPositionX()+m_vehicleSeats[i]->m_attachmentOffsetX,
					GetPositionY()+m_vehicleSeats[i]->m_attachmentOffsetY,
					GetPositionZ()+m_vehicleSeats[i]->m_attachmentOffsetZ);

				pass->Init();
				pass->m_TransporterGUID = GetGUID();
				AddPassenger(pass, i, true);
				pass->PushToWorld(map);
			}
		}
	}
}

void Vehicle::Load(CreatureProto * proto_, uint32 mode, float x, float y, float z, float o /* = 0.0f */)
{
	proto = proto_;
	if(!proto)
		return;

	if(proto->vehicle_entry != -1)
	{
		m_vehicleEntry = proto->vehicle_entry;
	}
	else
	{
		m_vehicleEntry = 124;
		if(sLog.IsOutDevelopement())
			printf("Attempted to create vehicle %u with invalid vehicle_entry, defaulting to 124, check your creature_proto table.\n", proto->Id);
		else
			OUT_DEBUG("Attempted to create vehicle %u with invalid vehicle_entry, defaulting to 124, check your creature_proto table.", proto->Id);
	}

	m_maxPassengers = 0;
	m_seatSlotMax = 0;
	VehicleEntry * ve = dbcVehicle.LookupEntry( m_vehicleEntry );
	if(!ve)
	{
		if(sLog.IsOutDevelopement())
			printf("Attempted to create non-existant vehicle %u.\n", GetVehicleEntry());
		else
			OUT_DEBUG("Attempted to create non-existant vehicle %u.", GetVehicleEntry());
		return;
	}

	for( uint32 i = 0; i < 8; i++ )
	{
		if( ve->m_seatID[i] )
		{
			m_vehicleSeats[i] = dbcVehicleSeat.LookupEntry( ve->m_seatID[i] );
			m_seatSlotMax = (i = 0 ? i + 1 : i); // i plus 1 so that we can count 0. Plus, no vehicle has 0 seats, so this ensures 1.

			if(m_vehicleSeats[i]->IsUsable())
			{
				seatisusable[i] = true;
				++m_maxPassengers;
			}
		}
	}

	Initialised = true;

	Creature::Load(proto_, mode, x, y, z, o);
}

bool Vehicle::Load(CreatureSpawn *spawn, uint32 mode, MapInfo *info)
{
	proto = CreatureProtoStorage.LookupEntry(spawn->entry);
	if(!proto)
		return false;

	if(proto->vehicle_entry != -1)
	{
		m_vehicleEntry = proto->vehicle_entry;
	}
	else
	{
		m_vehicleEntry = 124;
		if(sLog.IsOutDevelopement())
			printf("Attempted to create vehicle %u with invalid vehicle_entry, defaulting to 124, check your creature_proto table.\n", proto->Id);
		else
			OUT_DEBUG("Attempted to create vehicle %u with invalid vehicle_entry, defaulting to 124, check your creature_proto table.", proto->Id);
	}

	m_maxPassengers = 0;
	m_seatSlotMax = 0;
	VehicleEntry * ve = dbcVehicle.LookupEntry( m_vehicleEntry );
	if(!ve)
	{
		if(sLog.IsOutDevelopement())
			printf("Attempted to create non-existant vehicle %u.\n", GetVehicleEntry());
		else
			OUT_DEBUG("Attempted to create non-existant vehicle %u.", GetVehicleEntry());
		return false;
	}

	for( uint32 i = 0; i < 8; i++ )
	{
		if( ve->m_seatID[i] )
		{
			m_vehicleSeats[i] = dbcVehicleSeat.LookupEntry( ve->m_seatID[i] );
			m_seatSlotMax = i + 1;

			if(m_vehicleSeats[i]->IsUsable())
			{
				seatisusable[i] = true;
				++m_maxPassengers;
			}
		}
	}

	Initialised = true;

	return Creature::Load(spawn, mode, info);
}

void Vehicle::OnPushToWorld()
{
	InstallAccessories();
}

void Vehicle::SendSpells(uint32 entry, Player* plr)
{
	CreatureProtoVehicle* acc = CreatureProtoVehicleStorage.LookupEntry(GetEntry());
	if(!acc)
	{
		WorldPacket data(SMSG_PET_SPELLS, 12);
		data << uint64(0);
		data << uint32(0);
		plr->GetSession()->SendPacket(&data);
		return;
	}

	uint8 count = 0;

	WorldPacket data(SMSG_PET_SPELLS, 8+2+4+4+4*10+1+1);
	data << uint64(GetGUID());
	data << uint16(0);
	data << uint32(0);
	data << uint32(0x00000101);

	for (uint32 i = 0; i < 6; i++)
	{
		uint32 spellId = acc->VehicleSpells[i];
		if (!spellId)
			continue;

		SpellEntry const *spellInfo = dbcSpell.LookupEntryForced( spellId );
		if (!spellInfo)
			continue;

		if(spellInfo->Attributes & ATTRIBUTES_PASSIVE)
		{
			CastSpell(GetGUID(), spellId, true);
			data << uint16(0) << uint8(0) << uint8(i+8);
		}
		else
		{
			switch(spellId)
			{
			case 62286: // Tar
			case 62308: // Ram
			case 62522: // Electroshock!
				{
					if(IsInInstance())
					{
						if(dbcMap.LookupEntry(GetMapId())->israid()) // Difficulty Check
						{
							if(plr->iRaidType > MODE_10PLAYER_NORMAL)
							{
								data << uint32(0);
								continue;
							}
						}
					}
					data << uint32(MAKE_ACTION_BUTTON(spellId,i+8));
					++count;
				}break;

			default:
				data << uint32(MAKE_ACTION_BUTTON(spellId,i+8));
				++count;
				break;
			}
		}
	}

	for(uint8 i = 6; i < 10; i++)
	{
		data << uint16(0) << uint8(0) << uint8(i+8);
	}

	data << count; // spells count
	data << uint8(0);
	plr->GetSession()->SendPacket(&data);
}

void Vehicle::Despawn(uint32 delay, uint32 respawntime)
{
	if(delay)
	{
		sEventMgr.AddEvent(TO_VEHICLE(this), &Vehicle::Despawn, (uint32)0, respawntime, EVENT_VEHICLE_RESPAWN, delay, 1,0);
		return;
	}

	if(!IsInWorld())
		return;

	if(respawntime)
	{
		for(int i = 0; i < 8; i++)
		{
			if(m_passengers[i] != NULL)
			{
				if(m_passengers[i]->IsPlayer())
					// Remove any passengers
					RemovePassenger(m_passengers[i]);
				else
					delete m_passengers[i];
			}
		}

		/* get the cell with our SPAWN location. if we've moved cell this might break :P */
		MapCell * pCell = m_mapMgr->GetCellByCoords(m_spawnLocation.x, m_spawnLocation.y);
		if(!pCell)
			pCell = m_mapCell;

		ASSERT(pCell);
		pCell->_respawnObjects.insert(TO_OBJECT(this));
		sEventMgr.RemoveEvents(this);
		sEventMgr.AddEvent(m_mapMgr, &MapMgr::EventRespawnVehicle, TO_VEHICLE(this), pCell, EVENT_VEHICLE_RESPAWN, respawntime, 1, 0);
		Unit::RemoveFromWorld(false);
		m_position = m_spawnLocation;
		m_respawnCell=pCell;
	}
	else
	{
		Unit::RemoveFromWorld(true);
		SafeDelete();
	}
}

void Vehicle::Update(uint32 time)
{
	Creature::Update(time);
}

void Vehicle::SafeDelete()
{
	for(int i = 0; i < 8; i++)
	{
		if(m_passengers[i] != NULL)
		{
			if(m_passengers[i]->IsPlayer())
				// Remove any passengers
				RemovePassenger(m_passengers[i]);
			else
				delete m_passengers[i];
		}
	}

	sEventMgr.RemoveEvents(this);
	sEventMgr.AddEvent(TO_VEHICLE(this), &Vehicle::DeleteMe, EVENT_VEHICLE_SAFE_DELETE, 1000, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void Vehicle::DeleteMe()
{
	if(IsInWorld())
		RemoveFromWorld(false, true);

	delete this;
}

void Vehicle::AddPassenger(Unit* pPassenger, uint8 requestedseat /*= 0*/, bool force /*= false*/)
{

	if(!m_maxPassengers || !m_seatSlotMax) //how is this happening?
	{
		sLog.outColor(TRED, "Vehicle was not correctly initialised, retrying|r\n");
		InitSeats(m_vehicleEntry);
	}

	if(pPassenger->m_CurrentVehicle)
		pPassenger->m_CurrentVehicle->RemovePassenger(pPassenger);

	OUT_DEBUG("AddPassenger: Max Vehicle Slot: %u, Max Passengers: %u\n", m_seatSlotMax, m_maxPassengers);

	if(requestedseat > 0)
	{
		if(force)
		{
			if(m_vehicleSeats[requestedseat]) // Slot available?
			{
				if(pPassenger->IsPlayer() && seatisusable[requestedseat] == false)
					return;

				if(m_passengers[requestedseat])
					RemovePassenger(m_passengers[requestedseat]);

				_AddToSlot(pPassenger, requestedseat);
				return;
			}
		}
		else
		{
			if(!m_passengers[requestedseat] && m_vehicleSeats[requestedseat] && seatisusable[requestedseat] == true) // Slot available?
			{
				_AddToSlot(pPassenger, requestedseat );
				return;
			}
		}
	}
	else
	{ // Find us a slot!
		for(uint8 i = 0; i < m_seatSlotMax; i++)
		{
			if(pPassenger->IsPlayer())
			{
				if(!m_passengers[i] && m_vehicleSeats[i] && (seatisusable[i] == true)) // Found a slot
				{
					_AddToSlot(pPassenger, i );
					break;
				}
			}
			else
			{
				if(!m_passengers[i] && m_vehicleSeats[i])
				{
					_AddToSlot(pPassenger, i );
					break;
				}
			}
		}
	}
}

uint8 Vehicle::GetPassengerSlot(Unit* pPassenger)
{
	for(uint8 i = 0; i < m_seatSlotMax; i++)
	{
		if( m_passengers[i] == pPassenger ) // Found a slot
		{
			return i;
			break;
		}
	}
	return 0;
}

void Vehicle::RemovePassenger(Unit* pPassenger)
{
	if(pPassenger == NULL) // We have enough problems that we need to do this :(
		return;

	uint8 slot = pPassenger->m_inVehicleSeatId;

	pPassenger->m_CurrentVehicle = NULL;
	pPassenger->m_inVehicleSeatId = 0;

	pPassenger->RemoveFlag(UNIT_FIELD_FLAGS, (UNIT_FLAG_UNKNOWN_5 | UNIT_FLAG_PREPARATION | UNIT_FLAG_NOT_SELECTABLE));
	if( pPassenger->IsPlayer() )
		pPassenger->RemoveAura(TO_PLAYER(pPassenger)->m_MountSpellId);

	if( m_mountSpell )
		pPassenger->RemoveAura( m_mountSpell );

	WorldPacket data(SMSG_MONSTER_MOVE, 85);
	data << pPassenger->GetNewGUID();			// PlayerGUID
	data << uint8(0x40);						// Unk - blizz uses 0x40
	data << pPassenger->GetPosition();			// Player Position xyz
	data << getMSTime();						// Timestamp
	data << uint8(0x4);							// Flags
	data << pPassenger->GetOrientation();		// Orientation
	data << uint32(MOVEFLAG_AIR_SUSPENSION);	// MovementFlags
	data << uint32(0);							// MovementTime
	data << uint32(1);							// Pointcount
	data << GetPosition();						// Vehicle Position xyz
	SendMessageToSet(&data, false);

	pPassenger->movement_info.flags &= ~MOVEFLAG_TAXI;
	pPassenger->movement_info.transX = 0;
	pPassenger->movement_info.transY = 0;
	pPassenger->movement_info.transZ = 0;
	pPassenger->movement_info.transO = 0;
	pPassenger->movement_info.transTime = 0;
	pPassenger->movement_info.transSeat = 0;
	pPassenger->movement_info.transGuid = WoWGuid(uint64(NULL));

	if(pPassenger->IsPlayer())
	{
		Player* plr = TO_PLAYER(pPassenger);
		if(plr == GetControllingUnit())
		{
			plr->m_CurrentCharm = NULL;
			data.Initialize(SMSG_CLIENT_CONTROL_UPDATE);
			data << GetNewGUID() << (uint8)0;
			plr->GetSession()->SendPacket(&data);
		}
		RemoveFlag(UNIT_FIELD_FLAGS, (UNIT_FLAG_PLAYER_CONTROLLED_CREATURE | UNIT_FLAG_PLAYER_CONTROLLED));

		plr->SetPlayerStatus(TRANSFER_PENDING); // We get an ack later, if we don't set this now, we get disconnected.
		sEventMgr.AddEvent(plr, &Player::CheckPlayerStatus, (uint8)TRANSFER_PENDING, EVENT_PLAYER_CHECK_STATUS_Transfer, 5000, 0, 0);
		plr->m_sentTeleportPosition.ChangeCoords(GetPositionX(), GetPositionY(), GetPositionZ());
		plr->SetPosition(GetPositionX(), GetPositionY(), GetPositionZ(), GetOrientation());

		data.Initialize(MSG_MOVE_TELEPORT_ACK);
		data << plr->GetNewGUID();
		data << plr->m_teleportAckCounter;
		plr->m_teleportAckCounter++;
		data << uint32(MOVEFLAG_FLYING);
		data << uint16(0x40);
		data << getMSTime();
		data << GetPositionX();
		data << GetPositionY();
		data << GetPositionZ();
		data << GetOrientation();
		data << uint32(0);
		plr->GetSession()->SendPacket(&data);

		plr->SetUInt64Value( PLAYER_FARSIGHT, 0 );

		data.Initialize(SMSG_PET_DISMISS_SOUND);
		data << uint32(m_vehicleSeats[slot]->m_exitUISoundID);
		data << plr->GetPosition();
		plr->GetSession()->SendPacket(&data);

		data.Initialize(SMSG_PET_SPELLS);
		data << uint64(0);
		data << uint32(0);
		plr->GetSession()->SendPacket(&data);

		CreatureProtoVehicle* vehicleproto = CreatureProtoVehicleStorage.LookupEntry(GetEntry());
		if(vehicleproto && vehicleproto->healthfromdriver)
		{
			if(slot == 0)
			{
				uint32 health = GetUInt32Value(UNIT_FIELD_HEALTH);
				uint32 maxhealth = GetUInt32Value(UNIT_FIELD_MAXHEALTH);
				uint32 healthdiff = maxhealth - health;

				SetUInt32Value(UNIT_FIELD_HEALTH, GetProto()->MaxHealth - (healthdiff/(((plr->GetTotalItemLevel())*(vehicleproto->healthunitfromitemlev)))));
				SetUInt32Value(UNIT_FIELD_MAXHEALTH, GetProto()->MaxHealth);
			}
		}
	}
		
	if(slot == 0)
	{
		m_redirectSpellPackets = NULLPLR;
		CombatStatus.Vanished();
		pPassenger->SetUInt64Value( UNIT_FIELD_CHARM, 0 );
		SetUInt64Value(UNIT_FIELD_CHARMEDBY, 0);
		SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, GetCharmTempVal());
		/* update target faction set */
		_setFaction();
		UpdateOppFactionSet();
		//Despawn(0,1000);
	}

	m_passengers[slot] = NULL;
	pPassenger->m_TransporterGUID = NULL; // We need to null this out


	if(pPassenger->IsPlayer())
		--m_ppassengerCount;

	//note: this is not blizz like we should despawn
	//and respawn at spawn point.
	//Well actually this is how blizz wanted it
	//but they couldnt get it to work xD
	bool haspassengers = false;
	for(uint8 i = 0; i < m_seatSlotMax; i++)
	{
		if(m_passengers[i] != NULL && m_passengers[i]->IsPlayer())
		{
			haspassengers = true;
			break;
		}
	}

	if(!haspassengers && !m_CurrentVehicle) // Passenger and accessory checks.
	{
		if( m_spawn == NULL )
			SafeDelete();
	}

	if(!IsFull())
		SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
}

void Vehicle::DeletePassengerData(Unit* pPassenger)
{
	uint8 slot = pPassenger->m_inVehicleSeatId;
	pPassenger->m_inVehicleSeatId = 0;
	m_passengers[slot] = NULL;
}

bool Vehicle::HasPassenger(Unit* pPassenger)
{
	for(uint8 i = 0; i < m_seatSlotMax; i++)
	{
		if( m_passengers[i] == pPassenger )
			return true;
	}
	return false;
}

void Vehicle::_AddToSlot(Unit* pPassenger, uint8 slot)
{
	assert( slot < m_seatSlotMax );

	if(pPassenger->IsPlayer() && TO_PLAYER(pPassenger)->m_CurrentCharm)
		return;

	if(pPassenger->IsPlayer() && TO_PLAYER(pPassenger)->m_isGmInvisible)
	{
		sChatHandler.GreenSystemMessage(TO_PLAYER(pPassenger)->GetSession(), "Please turn off invis before entering vehicle.");
		return;
	}

	m_passengers[slot] = pPassenger;

	LocationVector v;
	v.x = m_vehicleSeats[slot]->m_attachmentOffsetX; /* pPassenger->m_TransporterX = */
	v.y = m_vehicleSeats[slot]->m_attachmentOffsetY; /* pPassenger->m_TransporterY = */
	v.z = m_vehicleSeats[slot]->m_attachmentOffsetZ; /* pPassenger->m_TransporterZ = */
	v.o = 0; /* pPassenger->m_TransporterO = */
	//pPassenger->m_transportPosition =& v; // This is handled elsewhere, do not initialize here.
	pPassenger->movement_info.flags |= MOVEFLAG_TAXI;
	pPassenger->movement_info.transX = v.x;
	pPassenger->movement_info.transY = v.y;
	pPassenger->movement_info.transZ = v.z;
	pPassenger->movement_info.transO = GetOrientation();
	pPassenger->movement_info.transSeat = slot;
	pPassenger->movement_info.transGuid = WoWGuid(GetGUID());
	pPassenger->m_inVehicleSeatId = slot;
	pPassenger->m_TransporterGUID = GetGUID();

	if( m_mountSpell )
		pPassenger->CastSpell( pPassenger, m_mountSpell, true );

	RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SELF_RES);

	// This is where the real magic happens
	if( pPassenger->IsPlayer() )
	{
		Player* pPlayer = TO_PLAYER(pPassenger);
		//pPlayer->Root();

		//Dismount
		if(pPlayer->m_MountSpellId)
			pPlayer->RemoveAura(pPlayer->m_MountSpellId);

		//Remove morph spells
		if(pPlayer->GetUInt32Value(UNIT_FIELD_DISPLAYID) != pPlayer->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID))
		{
			pPlayer->RemoveAllAurasOfType(SPELL_AURA_TRANSFORM);
			pPlayer->RemoveAllAurasOfType(SPELL_AURA_MOD_SHAPESHIFT);
		}

		//Dismiss any pets
		if(pPlayer->GetSummon())
		{
			if(pPlayer->GetSummon()->GetUInt32Value(UNIT_CREATED_BY_SPELL) > 0)
				pPlayer->GetSummon()->Dismiss(false);				// warlock summon -> dismiss
			else
				pPlayer->GetSummon()->Remove(false, true, true);	// hunter pet -> just remove for later re-call
		}

		pPlayer->m_CurrentVehicle = TO_VEHICLE(this);

		pPlayer->SetFlag(UNIT_FIELD_FLAGS, (UNIT_FLAG_UNKNOWN_5 | UNIT_FLAG_PREPARATION | UNIT_FLAG_NOT_SELECTABLE));

		//pPlayer->ResetHeartbeatCoords();
		pPlayer->SetUInt64Value(PLAYER_FARSIGHT, GetGUID());

		//WorldPacket data3(SMSG_CONTROL_VEHICLE, 0);
		//pPlayer->GetSession()->SendPacket(&data3);

		pPlayer->SetPlayerStatus(TRANSFER_PENDING);
		sEventMgr.AddEvent(pPlayer, &Player::CheckPlayerStatus, (uint8)TRANSFER_PENDING, EVENT_PLAYER_CHECK_STATUS_Transfer, 5000, 0, 0);
		pPlayer->m_sentTeleportPosition.ChangeCoords(GetPositionX(), GetPositionY(), GetPositionZ());

		WorldPacket data(SMSG_MONSTER_MOVE_TRANSPORT, 100);
		data << pPlayer->GetNewGUID();							// Passengerguid
		data << GetNewGUID();									// Transporterguid (vehicleguid)
		data << uint8(slot);									// Vehicle Seat ID
		data << uint8(0);										// Unknown
		data << GetPositionX() - pPlayer->GetPositionX();		// OffsetTransporterX
		data << GetPositionY() - pPlayer->GetPositionY();		// OffsetTransporterY
		data << GetPositionZ() - pPlayer->GetPositionZ();		// OffsetTransporterZ
		data << getMSTime();									// Timestamp
		data << uint8(0x04);									// Flags
		data << float(0);										// Orientation Offset
		data << uint32(MOVEFLAG_TB_MOVED);						// MovementFlags
		data << uint32(0);										// MoveTime
		data << uint32(1);										// Points
		data << v.x;											// GetTransOffsetX();
		data << v.y;											// GetTransOffsetY();
		data << v.z;											// GetTransOffsetZ();
		SendMessageToSet(&data, true);

		if(slot == 0)
		{
			SetControllingUnit(pPlayer);
			m_redirectSpellPackets = pPlayer;

			SetSpeed(RUN, m_runSpeed);
			SetSpeed(FLY, m_flySpeed);

			// send "switch mover" packet
			data.Initialize(SMSG_CLIENT_CONTROL_UPDATE);
			data << GetNewGUID() << uint8(1);
			pPlayer->GetSession()->SendPacket(&data);

			pPlayer->m_CurrentCharm = TO_UNIT(this);
			pPlayer->SetUInt64Value(UNIT_FIELD_CHARM, GetGUID());
			SetCharmTempVal(pPlayer->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, pPlayer->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			SetUInt64Value(UNIT_FIELD_CHARMEDBY, pPlayer->GetGUID());
			SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE);

			CreatureProtoVehicle* vehicleproto = CreatureProtoVehicleStorage.LookupEntry(GetEntry());
			if(vehicleproto && vehicleproto->healthfromdriver)
			{
				uint32 health = GetUInt32Value(UNIT_FIELD_HEALTH);
				uint32 maxhealth = GetUInt32Value(UNIT_FIELD_MAXHEALTH);
				uint32 healthdiff = maxhealth - health;

				SetUInt32Value(UNIT_FIELD_MAXHEALTH, (health+((pPlayer->GetTotalItemLevel())*(vehicleproto->healthunitfromitemlev))));
				SetUInt32Value(UNIT_FIELD_HEALTH, (health+((pPlayer->GetTotalItemLevel())*(vehicleproto->healthunitfromitemlev))) - healthdiff);
			}

			/* update target faction set */
			_setFaction();
			UpdateOppFactionSet();

			SendSpells(GetEntry(), pPlayer);
		}

		data.Initialize(SMSG_PET_DISMISS_SOUND);
		data << uint32(m_vehicleSeats[slot]->m_enterUISoundID);
		data << pPlayer->GetPosition();
		pPlayer->GetSession()->SendPacket(&data);
		++m_ppassengerCount;
	}
	else
	{
		pPassenger->m_CurrentVehicle = TO_VEHICLE(this);

		CreatureProtoVehicle* vehicleproto = CreatureProtoVehicleStorage.LookupEntry(GetEntry());
		if(vehicleproto != NULL)
			if(vehicleproto->accessories[slot].accessoryentry == pPassenger->GetEntry())
				if(vehicleproto->accessories[slot].unselectableaccessory == true)
					pPassenger->SetFlag(UNIT_FIELD_FLAGS, (UNIT_FLAG_UNKNOWN_5 | UNIT_FLAG_PREPARATION | UNIT_FLAG_NOT_SELECTABLE));
			else
				pPassenger->SetFlag(UNIT_FIELD_FLAGS, (UNIT_FLAG_UNKNOWN_5 | UNIT_FLAG_PREPARATION | UNIT_FLAG_NOT_SELECTABLE));
		else
			pPassenger->SetFlag(UNIT_FIELD_FLAGS, (UNIT_FLAG_UNKNOWN_5 | UNIT_FLAG_PREPARATION | UNIT_FLAG_NOT_SELECTABLE));

		pPassenger->SetPosition(GetPositionX()+v.x, GetPositionY()+v.y, GetPositionZ()+v.z, GetOrientation());
		WorldPacket data(MSG_MOVE_HEARTBEAT, 32);
		BuildHeartBeatMsg(&data);
		SendMessageToSet(&data, false);
	}

	if(IsFull())
		RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
}

/* This function changes a vehicles position server side to
keep us in sync with the client, so that the vehicle doesn't
get dismissed because the server thinks its gone out of range
of its passengers*/
void Vehicle::MoveVehicle(float x, float y, float z, float o) //thanks andy
{
	SetPosition(x, y, z, o, false);
	for(uint8 i = 0; i < m_seatSlotMax; i++)
	{
		if(m_passengers[i] != NULL)
		{
			m_passengers[i]->SetPosition(x,y,z,o,false);
		}
	}
}

void Vehicle::setDeathState(DeathState s)
{
	Creature::setDeathState(s);
	CreatureProtoVehicle* vehicleproto = CreatureProtoVehicleStorage.LookupEntry(GetEntry());

	for (uint8 i = 0; i < m_seatSlotMax; i++)
	{
		if(m_passengers[i] != NULL)
		{
			if(m_passengers[i]->IsPlayer() || (vehicleproto && vehicleproto->accessories[i].ejectfromvehicleondeath))
				RemovePassenger(m_passengers[i]);
			else
				m_passengers[i]->setDeathState(s);
		}
	}

	if( s == JUST_DIED && m_CreatedFromSpell)
		SafeDelete();
}

/* To change a vehicles speed we must send a Force Speed
change packet to the client. SpeedType takes values from
the enum PlayerSpeedType located in Player.h
RUN	            = 1,
RUNBACK         = 2,
SWIM	        = 3,
SWIMBACK        = 4,
WALK	        = 5,
FLY	            = 6,
value is the new speed to set*/
void Vehicle::SetSpeed(uint8 SpeedType, float value)
{
	if( value < 0.1f )
		value = 0.1f;

	uint8 buf[200];
	StackPacket data(SMSG_FORCE_RUN_SPEED_CHANGE, buf, 200);

	if( SpeedType != SWIMBACK )
	{
		data << GetNewGUID();
		data << uint32(0);
		if( SpeedType == RUN )
			data << uint8(1);

		data << value;
	}
	else
	{
		data << GetNewGUID();
		data << uint32(0);
		data << uint8(0);
		data << uint32(getMSTime());
		data << m_position.x;
		data << m_position.y;
		data << m_position.z;
		data << m_position.o;
		data << uint32(0);
		data << value;
	}
	
	switch(SpeedType)
	{
	case RUN:
		{
			data.SetOpcode(SMSG_FORCE_RUN_SPEED_CHANGE);
			m_runSpeed = value;
		}break;
	case RUNBACK:
		{
			data.SetOpcode(SMSG_FORCE_RUN_BACK_SPEED_CHANGE);
			m_backWalkSpeed = value;
		}break;
	case SWIM:
		{
			data.SetOpcode(SMSG_FORCE_SWIM_SPEED_CHANGE);
			m_swimSpeed = value;
		}break;
	case SWIMBACK:
		{
			data.SetOpcode(SMSG_FORCE_SWIM_BACK_SPEED_CHANGE);
			m_backSwimSpeed = value;
		}break;
	case FLY:
		{
			data.SetOpcode(SMSG_FORCE_FLIGHT_SPEED_CHANGE);
			m_flySpeed = value;
		}break;
	default:return;
	}
	
	SendMessageToSet(&data , true);

}

/* This function handles the packet sent from the client when we
leave a vehicle, it removes us server side from our current
vehicle*/
void WorldSession::HandleVehicleDismiss(WorldPacket & recv_data)
{
	Player* plr = GetPlayer();
	if(plr == NULL || !plr->m_CurrentVehicle)
		return;

	plr->ExitingVehicle = true;
	if((recv_data.rpos() != recv_data.wpos()) && (/*So we don't get disconnected due to size checks.*/recv_data.size() <= 90))
		HandleMovementOpcodes(recv_data);

	plr->m_CurrentVehicle->RemovePassenger(GetPlayer());
	plr->ExitingVehicle = false;
}

/* This function handles the packet from the client which is
sent when we click on an npc with the flag UNIT_FLAG_SPELLCLICK
and checks if there is room for us then adds us as a passenger
to that vehicle*/
void WorldSession::HandleSpellClick( WorldPacket & recv_data )
{
	if (GetPlayer() == NULL || GetPlayer()->m_CurrentVehicle)
		return;

	CHECK_PACKET_SIZE(recv_data, 8);

	uint64 guid;
	recv_data >> guid;

	Vehicle* pVehicle = NULL;
	Unit* unit = GetPlayer()->GetMapMgr()->GetUnit(guid);
	Unit* pPlayer = TO_UNIT(GetPlayer());

	if(!unit)
		return;

	if(!unit->IsVehicle())
	{
		if(unit->IsCreature())
		{
			Creature* ctr = TO_CREATURE(unit);
			if(ctr->GetProto()->SpellClickid)
				ctr->CastSpell(pPlayer, ctr->GetProto()->SpellClickid, true);
			else
			{
				if(sLog.IsOutDevelopement())
					printf("[SPELL][CLICK]: Unknown spell click spell on creature %u\n", ctr->GetEntry());
				else
					OUT_DEBUG("[SPELL][CLICK]: Unknown spell click spell on creature %u", ctr->GetEntry());
			}
		}
		return;
	}
	else
	{
		pVehicle = TO_VEHICLE(unit);
	}

	if(!pVehicle->GetMaxPassengerCount())
		return;

	if(!pVehicle->GetMaxSeat())
		return;

	// just in case.
	if( sEventMgr.HasEvent( pVehicle, EVENT_VEHICLE_SAFE_DELETE ) )
		return;

	if(pVehicle->HasPassenger(pPlayer))
		pVehicle->RemovePassenger(pPlayer);

	pVehicle->AddPassenger(pPlayer);
}

/* This function handles the packet sent from the client when we
change a seat on a vehicle. If the seat has a unit passenger and unit
is a vehicle, we will enter the passenger.*/
void WorldSession::HandleRequestSeatChange( WorldPacket & recv_data )
{
	WoWGuid Vehicleguid;
	int8 RequestedSeat;
	Vehicle* cv = _player->m_CurrentVehicle;
	_player->ChangingSeats = true;

	if(recv_data.GetOpcode() == CMSG_REQUEST_VEHICLE_PREV_SEAT)
	{
		int8 seat = _player->GetSeatID();
		int8 newseat = seat;
		while(seat == _player->GetSeatID())
		{
			if(newseat < 1)
				break;

			--newseat;

			if(cv->m_vehicleSeats[newseat] && cv->seatisusable[newseat])
				break;
		}
		if(cv->m_vehicleSeats[newseat] && cv->seatisusable[newseat])
			cv->ChangeSeats(_player, newseat);

		_player->ChangingSeats = false;
		return;
	}
	else if(recv_data.GetOpcode() == CMSG_REQUEST_VEHICLE_NEXT_SEAT)
	{
		int8 seat = _player->GetSeatID();
		int8 newseat = seat;
		while(seat == _player->GetSeatID())
		{
			if(newseat > 7)
				break;

			++newseat;

			if(cv->m_vehicleSeats[newseat] && cv->seatisusable[newseat])
				break;
		}
		if(cv->m_vehicleSeats[newseat] && cv->seatisusable[newseat])
			cv->ChangeSeats(_player, newseat);

		_player->ChangingSeats = false;
		return;
	}
	else if(recv_data.GetOpcode() == CMSG_REQUEST_VEHICLE_SWITCH_SEAT)
	{
		recv_data >> Vehicleguid;
		recv_data >> RequestedSeat;
	}
	else
	{
		HandleMovementOpcodes(recv_data);
		recv_data >> Vehicleguid;
		recv_data >> RequestedSeat;
	}

	uint64 guid = Vehicleguid.GetOldGuid();
	Vehicle* vehicle = GetPlayer()->GetMapMgr()->GetVehicle(GET_LOWGUID_PART(guid));

	if(cv->GetPassengerSlot(vehicle) || cv->m_CurrentVehicle == vehicle)
	{
		cv->RemovePassenger(_player);
		vehicle->AddPassenger(_player);
		_player->ChangingSeats = false;
		return;
	}

	cv->ChangeSeats(_player, RequestedSeat);
	_player->ChangingSeats = false;
}

void Vehicle::ChangeSeats(Unit* unit, uint8 seatid)
{
	if(seatid == unit->GetSeatID())
	{
		OUT_DEBUG("Return, Matching Seats. Requsted: %u, current: %u", seatid, unit->GetSeatID());
		return;
	}

	if(unit->m_CurrentVehicle != NULL)
	{
		unit->m_CurrentVehicle->RemovePassenger(unit);
	}

	unit->m_TransporterGUID = GetGUID();
	AddPassenger(unit, seatid);
}
