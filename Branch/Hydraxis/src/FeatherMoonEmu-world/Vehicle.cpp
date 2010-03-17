/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#include "StdAfx.h"

Vehicle::Vehicle(uint64 guid, VehicleEntry const *vehInfo) : Creature(guid), m_vehicleInfo(vehInfo), m_usableSeatNum(0)
{
	m_isVehicle = true;

	for(uint32 i = 0; i < 8; ++i)
	{
		if(uint32 seatId = m_vehicleInfo->m_seatID[i])
		{
			if(VehicleSeatEntry const *veSeat = dbcVehicleSeat.LookupEntry(seatId))
			{
				m_Seats.insert(std::make_pair(i, VehicleSeat(veSeat)));
				if(veSeat->IsUsable())
					++m_usableSeatNum;
			}
		}
	}
	assert(!m_Seats.empty());
}

Vehicle::~Vehicle()
{
	for(SeatMap::const_iterator itr = m_Seats.begin(); itr != m_Seats.end(); ++itr)
		assert(!itr->second.passenger);
}

void Vehicle::Destructor()
{
	RemovePassenger(TO_PLAYER(this));
	delete this;
}

void Vehicle::Install()
{
	if(Creature *cre = TO_CREATURE(me))
	{
		if(m_vehicleInfo->m_powerType == POWER_STEAM)
		{
			me->SetPowerType(POWER_ENERGY);
			me->SetMaxPower(POWER_ENERGY, 100);
			me->SetPower(POWER_ENERGY, 100);
		}
		else if(m_vehicleInfo->m_powerType == POWER_PYRITE)
		{
			me->SetPowerType(POWER_ENERGY);
			me->SetMaxPower(POWER_ENERGY, 50);
			me->SetPower(POWER_ENERGY, 0);
		}
		else
		{
			for(uint32 i = 0; i < MAX_SPELL_VEHICLE; ++i)
			{
				if(!cre->m_spells[i])
					continue;

				SpellEntry const *spellInfo = dbcSpell.LookupEntry(cre->m_spells[i]);

				if(!spellInfo)
					continue;

				if(spellInfo->powerType == POWER_MANA)
					break;

				if(spellInfo->powerType == POWER_ENERGY)
				{
					me->SetPowerType(POWER_ENERGY);
					me->SetMaxPower(POWER_ENERGY, 100);
					break;
				}
			}
		}
	}

	Reset();
}

void Vehicle::InstallAllAccessories()
{
	switch(this->GetEntry())
	{
		//case 27850:InstallAccessory(27905,1);break;
	case 28782:
		InstallAccessory(28768,0,false);
		break; // Acherus Deathcharger
	case 28312:
		InstallAccessory(28319,7);
		break;
	case 32627:
		InstallAccessory(32629,7);
		break;
	case 32930:
		InstallAccessory(32933,0);
		InstallAccessory(32934,1);
		break;
	case 33109:
		InstallAccessory(33167,1);
		break;
	case 33060: // Salvaged Seige Engine
		InstallAccessory(33067,7);
		break;
	case 33113:
		InstallAccessory(33114,0);
		InstallAccessory(33114,1);
		InstallAccessory(33114,2);
		InstallAccessory(33114,3);
		InstallAccessory(33139,7);
		break;
	case 33114:
		InstallAccessory(33143,2); // Overload Control Device
		InstallAccessory(33142,1); // Leviathan Defense Turret
		break;
	case 33214:
		InstallAccessory(33218,1,false);
		break; // Mechanolift 304-A
	}
}

void Vehicle::Uninstall()
{
	sLog.outDebug("Vehicle::Uninstall %u", me->GetEntry());

	for (SeatMap::iterator itr = m_Seats.begin(); itr != m_Seats.end(); ++itr)
		if(Unit *passenger = itr->second.passenger)
			if(passenger->HasUnitTypeMask(UNIT_MASK_ACCESSORY))
				((TempSummon*)passenger)->UnSummon();

	RemoveAllPassengers();
}

void Vehicle::Die()
{
	sLog.outDebug("Vehicle::Die %u", me->GetEntry());

	for (SeatMap::iterator itr = m_Seats.begin(); itr != m_Seats.end(); ++itr)
		if(Unit *passenger = itr->second.passenger)
			if(passenger->HasUnitTypeMask(UNIT_MASK_ACCESSORY))
				passenger->setDeathState(JUST_DIED);

	RemoveAllPassengers();
}

void Vehicle::Reset()
{
	sLog.outDebug("Vehicle::Reset");

	InstallAllAccessories();

	if(m_usableSeatNum)
		me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
}

void Vehicle::Dismiss()
{
	sLog.outDebug("Vehicle::Dismiss %u", me->GetEntry());
	Uninstall();
}

void Vehicle::InstallAccessory(uint32 entry, int8 seatId, bool minion)
{
	if(Unit *passenger = GetPassenger(seatId))
	{
		// already installed
		if(passenger->GetEntry() == entry)
		{
			assert(passenger->GetTypeId() == TYPEID_UNIT);

			if(me->GetTypeId() == TYPEID_UNIT && TO_CREATURE(me)->IsInEvadeMode() && TO_CREATURE(passenger)->IsAIEnabled)
				TO_CREATURE(passenger)->AI()->SetAIState(STATE_EVADE);
			return;
		}
		passenger->ExitVehicle(); // this should not happen
	}

	//TODO: accessory should be minion
	if(Creature *accessory = me->SummonCreature(entry, me->GetPosition(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
	{
		if(minion)
			accessory->AddUnitTypeMask(UNIT_MASK_ACCESSORY);

		accessory->EnterVehicle(this, seatId);
		// This is not good, we have to send update twice
		accessory->SendMovementFlagUpdate();
	}
}

Unit *Vehicle::GetPassenger(int8 seatId) const
{
	SeatMap::const_iterator seat = m_Seats.find(seatId);
	if(seat == m_Seats.end()) return NULL;
	return seat->second.passenger;
}

void Vehicle::Init()
{
	Creature::Init();
	InstallAllAccessories();
}

void Vehicle::InitSeats(uint32 vehicleEntry, Player* pRider)
{
	m_maxPassengers = 0;
	SetVehicleEntry(vehicleEntry);
	VehicleEntry * ve = dbcVehicle.LookupEntry( vehicleEntry );
	if(!ve)
	{
		OUT_DEBUG("Attempted to create non-existant vehicle %u.\n", vehicleEntry);
		return;
	}

	for( uint32 i = 0; i < 8; ++i )
	{
		if( ve->m_seatID[i] )
		{
			m_vehicleSeats[i] = dbcVehicleSeat.LookupEntry( ve->m_seatID[i] );
			++m_maxPassengers;
		}
	}
	//InstallAllAccessories();

	Initialised = true;

	if( pRider != NULL)
		AddPassenger( pRider );
}

void Vehicle::Load(CreatureProto * proto_, float x, float y, float z, float o /* = 0.0f */)
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
		OUT_DEBUG("Attempted to create vehicle %u with invalid vehicle_entry, defaulting to 124, check your creature_proto table.\n", proto->Id);
	}

	m_maxPassengers = 0;
	VehicleEntry * ve = dbcVehicle.LookupEntry( m_vehicleEntry );
	if(!ve)
	{
		OUT_DEBUG("Attempted to create non-existant vehicle %u.\n", GetVehicleEntry());
		return;
	}

	for( uint32 i = 0; i < 8; ++i )
	{
		if( ve->m_seatID[i] )
		{
			m_vehicleSeats[i] = dbcVehicleSeat.LookupEntry( ve->m_seatID[i] );
			++m_maxPassengers;
		}
	}

	Initialised = true;
	//Vehicle::Install();

	Creature::Load(proto_, x, y, z, o);
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
		OUT_DEBUG("Attempted to create vehicle %u with invalid vehicle_entry, defaulting to 124, check your creature_proto table.\n", proto->Id);
	}

	m_maxPassengers = 0;
	VehicleEntry * ve = dbcVehicle.LookupEntry( m_vehicleEntry );
	if(!ve)
	{
		OUT_DEBUG("Attempted to create non-existant vehicle %u.\n", GetVehicleEntry());
		return false;
	}

	for( uint32 i = 0; i < 8; ++i )
	{
		if( ve->m_seatID[i] )
		{
			m_vehicleSeats[i] = dbcVehicleSeat.LookupEntry( ve->m_seatID[i] );
			++m_maxPassengers;
		}
	}

	Initialised = true;
	//Vehicle::Install();

	return Creature::Load(spawn, mode, info);
}

void Vehicle::SendSpells(uint32 entry, Player* plr)
{
	list<uint32> avail_spells;
	for(list<AI_Spell*>::iterator itr = GetAIInterface()->m_spells.begin(); itr != GetAIInterface()->m_spells.end(); ++itr)
	{
		if((*itr)->agent == AGENT_SPELL)
			avail_spells.push_back((*itr)->spell->Id);
	}
	list<uint32>::iterator itr = avail_spells.begin();
	
	WorldPacket data(SMSG_PET_SPELLS, GetAIInterface()->m_spells.size() * 4 + 22);
	data << GetGUID();
	data << uint16(0x0000);//pet family
	data << uint32(0x00000000);//unk
	data << uint32(0x00000101);//bar type

	// Send the actionbar
	for(uint8 i = 1; i < 10; ++i)
	{
		if(itr != avail_spells.end())
		{
			data << uint16((*itr)) << uint8(0) << uint8(i+8);
			++itr;
		}
		else
			data << uint16(0) << uint8(0) << uint8(i+8);
	}

	// Send the rest of the spells.
	data << uint8(avail_spells.size());
	for(itr = avail_spells.begin(); itr != avail_spells.end(); ++itr)
		data << uint16(*itr) << uint16(DEFAULT_SPELL_STATE);
	
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
	sEventMgr.RemoveEvents(this);
	sEventMgr.AddEvent(TO_VEHICLE(this), &Vehicle::DeleteMe, EVENT_VEHICLE_SAFE_DELETE, 1000, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void Vehicle::DeleteMe()
{
	Vehicle* pThis = TO_VEHICLE(this);

	if(IsInWorld())
		RemoveFromWorld(false, true);

	Destructor();
}

void Vehicle::AddPassenger(Unit* pPassenger)
{
	if(!m_maxPassengers) //how is this happening?
	{
		sLog.outColor(TRED, "Vehicle was not correctly initialised, retrying");
		sLog.outColor(TNORMAL, "\n");
		InitSeats(m_vehicleEntry);
	}

	// Find an available seat
	for(uint8 i = 0; i < m_maxPassengers; ++i)
	{
		if( !m_passengers[i] && m_vehicleSeats[i] ) // Found a slot
		{
			_AddToSlot(pPassenger, i );
			break;
		}
	}
}

bool Vehicle::AddPassenger(Unit *unit, int8 seatId)
{
	if(unit->GetVehicle() != this)
		return false;

	SeatMap::iterator seat;
	if(seatId < 0) // no specific seat requirement
	{
		for(seat = m_Seats.begin(); seat != m_Seats.end(); ++seat)
			if(!seat->second.passenger && seat->second.seatInfo->IsUsable())
				break;

		if(seat == m_Seats.end()) // no available seat
			return false;
	}
	else
	{
		seat = m_Seats.find(seatId);
		if(seat == m_Seats.end())
			return false;

		if(seat->second.passenger)
			seat->second.passenger->ExitVehicle();

		assert(!seat->second.passenger);
	}

	sLog.outDebug("Unit tried to enter vehicle entry %u id %u dbguid %u seat %d", me->GetEntry(), m_vehicleInfo->m_ID, me->GetLowGUID(), (int32)seat->first);

	seat->second.passenger = unit;
	if(seat->second.seatInfo->IsUsable())
	{
		assert(m_usableSeatNum);
		--m_usableSeatNum;
		if(!m_usableSeatNum)
			me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
	}

	if(seat->second.seatInfo->m_flags && !(seat->second.seatInfo->m_flags & 0x400))
		unit->addUnitState(UNIT_STAT_ONVEHICLE);

	//SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);

	unit->AddUnitMovementFlag(MOVEFLAG_TAXI);
	VehicleSeatEntry const *veSeat = seat->second.seatInfo;
	unit->m_movementInfo.transX = veSeat->m_attachmentOffsetX;
	unit->m_movementInfo.transY = veSeat->m_attachmentOffsetY;
	unit->m_movementInfo.transZ = veSeat->m_attachmentOffsetZ;
	unit->m_movementInfo.transO = 0;
	unit->m_movementInfo.transTime = 0; // 1 for player
	unit->m_movementInfo.transSeat = seat->first;

	//if(unit->GetTypeId() == TYPEID_PLAYER && seat->first == 0 && seat->second.seatInfo->m_flags & 0x800) // not right
	//	if (!me->SetCharmedBy(unit, CHARM_TYPE_VEHICLE))
	//		assert(false);

	if(me->GetTypeId() == TYPEID_UNIT)
	{
		if(me->IsInWorld())
		{
			unit->SendMonsterMoveTransport(me);
			// move self = move all passengers
			//me->GetMap()->CreatureRelocation((Creature*)me, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
		}

		if(TO_CREATURE(me)->IsAIEnabled)
			TO_CREATURE(me)->AI()->PassengerBoarded(unit, seat->first, true);
	}

	//if(unit->GetTypeId() == TYPEID_PLAYER)
	//	((Player*)unit)->SendTeleportAckMsg();
	//unit->SendMovementFlagUpdate();

	return true;
}

uint8 Vehicle::GetPassengerSlot(Unit* pPassenger)
{
	for(uint8 i = 0; i < m_maxPassengers; ++i)
	{
		if( m_passengers[i] == pPassenger ) // Found a slot
		{
			return i;
			break;
		}
	}
	return 0xFF;
}

int8 Vehicle::GetNextEmptySeat(int8 seatId, bool next) const
{
	SeatMap::const_iterator seat = m_Seats.find(seatId);
	if(seat == m_Seats.end()) return -1;
	while(seat->second.passenger || !seat->second.seatInfo->IsUsable())
	{
		if(next)
		{
			++seat;
			if(seat == m_Seats.end())
				seat = m_Seats.begin();
		}
		else
		{
			if(seat == m_Seats.begin())
				seat = m_Seats.end();
			--seat;
		}
		if(seat->first == seatId)
			return -1; // no available seat
	}
	return seat->first;
}

void Vehicle::RemovePassenger(Unit* pPassenger)
{
	Vehicle* pThis = TO_VEHICLE(this);

	uint8 slot = pPassenger->m_inVehicleSeatId;

	if(slot >= 8)
		return;

	pPassenger->m_CurrentVehicle = NULL;
	pPassenger->m_inVehicleSeatId = 0xFF;

	pPassenger->RemoveFlag(UNIT_FIELD_FLAGS, (UNIT_FLAG_UNKNOWN_5 | UNIT_FLAG_PREPARATION));
	if( pPassenger->IsPlayer() )
		pPassenger->RemoveAura(TO_PLAYER(pPassenger)->m_MountSpellId);

	if( m_mountSpell )
		pPassenger->RemoveAura( m_mountSpell );

	WorldPacket data(MSG_MOVE_HEARTBEAT, 48);
	data << pPassenger->GetNewGUID();
	data << uint32(MOVEFLAG_ROOT);
	data << uint16(0x40);
	data << getMSTime();
	data << GetPositionX();
	data << GetPositionY();
	data << GetPositionZ();
	data << GetOrientation();
	data << uint32(0);
	pPassenger->SendMessageToSet(&data, false);

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

		plr->SetPlayerStatus(TRANSFER_PENDING);
		plr->m_sentTeleportPosition.ChangeCoords(GetPositionX(), GetPositionY(), GetPositionZ());
		plr->SetPosition(GetPositionX(), GetPositionY(), GetPositionZ(), GetOrientation());
		data.Initialize(MSG_MOVE_TELEPORT_ACK);
		data << plr->GetNewGUID();
		data << plr->m_teleportAckCounter;
		plr->m_teleportAckCounter++;
		data << uint32(MOVEFLAG_ROOT);
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
	}
		
	if(slot == 0)
	{
		m_redirectSpellPackets = NULLPLR;
		CombatStatus.Vanished();
		pPassenger->SetUInt64Value( UNIT_FIELD_CHARM, 0 );
		SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, GetCharmTempVal());
		SetUInt64Value(UNIT_FIELD_CHARMEDBY, 0);
		/* update target faction set */
		_setFaction();
		UpdateOppFactionSet();

		//GetAIInterface()->SetAIState(STATE_IDLE);
		GetAIInterface()->WipeHateList();
		GetAIInterface()->WipeTargetList();
		EnableAI();
		//Despawn(0,1000);

		//note: this is not blizz like we should despawn
		//and respawn at spawn point.
		//Well actually this is how blizz wanted it
		//but they couldnt get it to work xD
		if( m_spawn )
			GetAIInterface()->MoveTo(m_spawn->x, m_spawn->y, m_spawn->z, m_spawn->o);
		else //we're a temp spawn
			SafeDelete();

		m_controllingUnit = NULL;
		for(uint8 i = 0; i < m_maxPassengers; i++)
		{
			if(i != 0 && m_passengers[i])
			{
				RemovePassenger(m_passengers[i]);
			}
		}
	}

	m_passengers[slot] = NULL;
}

bool Vehicle::HasPassenger(Unit* pPassenger)
{
	for(uint8 i = 0; i < m_maxPassengers; ++i)
	{
		if( m_passengers[i] == pPassenger )
			return true;
	}
	return false;
}

void Vehicle::_AddToSlot(Unit* pPassenger, uint8 slot)
{
	assert( slot < m_maxPassengers );
	m_passengers[ slot ] = pPassenger;
	pPassenger->m_inVehicleSeatId = slot;
	/* pPassenger->m_transportGuid = GetGUID(); */
	LocationVector v;
	v.x = /* pPassenger->m_TransporterX = */ m_vehicleSeats[slot]->m_attachmentOffsetX;
	v.y = /* pPassenger->m_TransporterY = */ m_vehicleSeats[slot]->m_attachmentOffsetY;
	v.z = /* pPassenger->m_TransporterZ = */ m_vehicleSeats[slot]->m_attachmentOffsetZ;
	v.o = /* pPassenger->m_TransporterO = */ 0;

	if( m_mountSpell )
		pPassenger->CastSpell( pPassenger, m_mountSpell, true );

	TO_PLAYER(pPassenger)->Dismount();
	TO_PLAYER(pPassenger)->RemoveAurasByType(SPELL_AURA_MOUNTED);

	if(GetTypeId() == TYPEID_PLAYER)
	{
		WorldPacket data(SMSG_ON_CANCEL_EXPECTED_RIDE_VEHICLE_AURA, 0);
		TO_PLAYER(pPassenger)->GetSession()->SendPacket(&data);
	}

	RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SELF_RES);
	// This is where the real magic happens
	if( pPassenger->IsPlayer() )
	{
		Player* pPlayer = TO_PLAYER(pPassenger);
		//pPlayer->Root();

		if(pPlayer->m_CurrentCharm)
			return;

		//Dismount
		if(pPlayer->m_MountSpellId)
			pPlayer->RemoveAura(pPlayer->m_MountSpellId);
	
		//Remove morph spells
		if(pPlayer->GetUInt32Value(UNIT_FIELD_DISPLAYID)!= pPlayer->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID))
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

		pPlayer->SetFlag(UNIT_FIELD_FLAGS, (UNIT_FLAG_UNKNOWN_5 | UNIT_FLAG_PREPARATION));

		//pPlayer->ResetHeartbeatCoords();
		pPlayer->SetUInt64Value(PLAYER_FARSIGHT, GetGUID());

		//WorldPacket data3(SMSG_CONTROL_VEHICLE, 0);
		//pPlayer->GetSession()->SendPacket(&data3);

		pPlayer->SetPlayerStatus(TRANSFER_PENDING);
		pPlayer->m_sentTeleportPosition.ChangeCoords(GetPositionX(), GetPositionY(), GetPositionZ());

		SeatMap::iterator seat;
		if(slot < 0) // no specific seat requirement
		{
			for(seat = m_Seats.begin(); seat != m_Seats.end(); ++seat)
				if(!seat->second.passenger && seat->second.seatInfo->IsUsable())
					break;

			if(seat == m_Seats.end()) // no available seat
				return;
		}
		else
		{
			seat = m_Seats.find(slot);

			if(seat == m_Seats.end())
				return;

			if(seat->second.passenger)
				seat->second.passenger->ExitVehicle();

			assert(!seat->second.passenger);
		}

		pPlayer->AddUnitMovementFlag(MOVEFLAG_TAXI);
		VehicleSeatEntry const *veSeat = seat->second.seatInfo;
		pPlayer->m_movementInfo.transX = veSeat->m_attachmentOffsetX;
		pPlayer->m_movementInfo.transY = veSeat->m_attachmentOffsetY;
		pPlayer->m_movementInfo.transZ = veSeat->m_attachmentOffsetZ;
		pPlayer->m_movementInfo.transO = 0;
		pPlayer->m_movementInfo.transTime = 0; // 1 for player
		pPlayer->m_movementInfo.transSeat = seat->first;

		WorldPacket data(MSG_MOVE_TELEPORT, 68);
		data << pPlayer->GetNewGUID();
		data << pPlayer->m_teleportAckCounter;					// counter
		data << uint32(MOVEFLAG_TAXI);							// transport
		data << uint16(0);										// special flags
		data << uint32(getMSTime());							// time
		data << GetPositionX();									// position vector 
		data << GetPositionY();									// position vector 
		data << GetPositionZ();									// position vector 
		data << GetOrientation();								// orientaion
		// transport part
		data << GetNewGUID();									// transport guid
		data << v.x;											// transport offsetX
		data << v.y;											// transport offsetY
		data << v.z;											// transport offsetZ
		data << uint32(0);										// transport time
		data << uint8(slot);									// seat
		// end of transport part
		data << uint32(0);
		pPlayer->SendMessageToSet(&data, false);

		data.Initialize(MSG_MOVE_TELEPORT_ACK);
		data << pPlayer->GetNewGUID();
		data << pPlayer->m_teleportAckCounter;					// ack counter
		pPlayer->m_teleportAckCounter++;
		data << uint32(MOVEFLAG_TAXI);							// transport
		data << uint16(0);										// special flags
		data << uint32(getMSTime());							// time
		data << GetPositionX();									// position vector 
		data << GetPositionY();									// position vector 
		data << GetPositionZ();									// position vector 
		data << GetOrientation();								// orientaion
		// transport part
		data << GetNewGUID();									// transport guid
		data << v.x;											// transport offsetX
		data << v.y;											// transport offsetY
		data << v.z;											// transport offsetZ
		data << uint32(0);										// transport time
		data << uint8(slot);									// seat
		// end of transport part
		data << uint32(0);										// fall time
		pPlayer->GetSession()->SendPacket(&data);

		DisableAI();

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

			pPlayer->m_CurrentCharm = TO_VEHICLE(this);
			pPlayer->SetUInt64Value(UNIT_FIELD_CHARM, GetGUID());
			SetCharmTempVal(GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, pPlayer->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			SetUInt64Value(UNIT_FIELD_CHARMEDBY, pPlayer->GetGUID());
			SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE);

			/* update target faction set */
			_setFaction();
			UpdateOppFactionSet();

			SendSpells(GetEntry(), pPlayer);
			pPlayer->UpdateVisibility();
			pPlayer->SetControlled(true, UNIT_STAT_ROOT);
		}
	}

    SeatMap::iterator seat;
	seat->second.passenger = pPassenger;
	if(seat->second.seatInfo->m_flags && !(seat->second.seatInfo->m_flags & 0x400))
		pPassenger->addUnitState(UNIT_STAT_ONVEHICLE);

	UpdatePassengers();
}

/* This function changes a vehicles position server side to
keep us in sync with the client, so that the vehicle doesn't
get dismissed because the server thinks its gone out of range
of its passengers*/
void Vehicle::MoveVehicle(float x, float y, float z, float o) //thanks andy
{
	SetPosition(x, y, z, o, false);
	for (uint8 i=0; i<m_maxPassengers; ++i)
	{
		if (m_passengers[i] != NULL)
		{
			m_passengers[i]->SetPosition(x,y,z,o,false);
		}
	}
	Vehicle::UpdatePassengers();
}

void Vehicle::setDeathState(DeathState s)
{
	Creature::setDeathState(s);

	for (uint8 i=0; i<m_maxPassengers; ++i)
		if (m_passengers[i] != NULL)
			RemovePassenger(m_passengers[i]);

	if( s == JUST_DIED && m_CreatedFromSpell)
		SafeDelete();
}

/* To change a vehicles speed we must send a Force Speed
change packet to the client. SpeedType takes values from
the enum PlayerSpeedType located in Player.h
RUN				=	1,
RUNBACK			=	2,
SWIM			=	3,
SWIMBACK		=	4,
WALK			=	5,
FLY				=	6,
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

	default:
		return;
	}
	
	SendMessageToSet(&data , true);

}

/* This function handles the packet sent from the client when we
leave a vehicle, it removes us server side from our current
vehicle*/
void WorldSession::HandleVehicleDismiss(WorldPacket & recv_data)
{
	if (GetPlayer() == NULL || !GetPlayer()->m_CurrentVehicle)
		return;

	GetPlayer()->GetVehicleKit()->RemovePassenger(GetPlayer());
}

/* This function handles the packet from the client which is
sent when we click on an npc with the flag UNIT_FLAG_SPELLCLICK
and checks if there is room for us then adds us as a passenger
to that vehicle*/
void WorldSession::HandleSpellClick( WorldPacket & recv_data )
{
	uint64 guid;
	recv_data >> guid;

	Vehicle* pVehicle = GetPlayer()->GetMapMgr()->GetVehicle(GET_LOWGUID_PART(guid));
	Unit* pPlayer = TO_UNIT(GetPlayer());

	if(!pVehicle)
		return;

	if(!pVehicle->IsInWorld())
	{
		sLog.outError("Spell click target %u is not in world!", pVehicle->GetEntry());
		assert(false);
		return;
	}

	if(!pVehicle->GetMaxPassengerCount())
		return;

	// just in case.
	if( sEventMgr.HasEvent( pVehicle, EVENT_VEHICLE_SAFE_DELETE ) )
		return;

	if(pVehicle->HasPassenger(pPlayer))
		pVehicle->RemovePassenger(pPlayer);

	pVehicle->AddPassenger(pPlayer);
}

bool Vehicle::HasEmptySeat(int8 seatId) const
{
	SeatMap::const_iterator seat = m_Seats.find(seatId);
	if(seat == m_Seats.end()) return false;
	return !seat->second.passenger;
}

void Vehicle::UpdatePassengers()
{
	for(SeatMap::iterator itr = m_Seats.begin(); itr != m_Seats.end();++itr)
	{
		if(Unit *passenger = itr->second.passenger)
		{
			if(passenger->GetVehicle() != this)
				continue;

			if(itr->second.passenger && passenger->GetTypeId()==TYPEID_PLAYER)
			{
				Player* pl= ((Player*)passenger);
				if( me->CalcDistance(passenger) > (World::GetMaxVisibleDistanceOnContinents()/2))
				{
					pl->SetPosition(
						me->GetPositionX()-pl->GetTransOffsetX(),
						me->GetPositionY()-pl->GetTransOffsetY(),
						me->GetPositionZ()-pl->GetTransOffsetZ(),
						pl->GetOrientation());

					pl->m_movementInfo.x = me->GetPositionX()-pl->GetTransOffsetX();
					pl->m_movementInfo.y = me->GetPositionY()-pl->GetTransOffsetY();
					pl->m_movementInfo.z = me->GetPositionZ()-pl->GetTransOffsetZ();

					WorldPacket data(MSG_MOVE_HEARTBEAT, 32);
					data.append(pl->GetPackGUID());
					pl->BuildMovementPacket(&data);
					pl->GetSession()->SendPacket(&data);
				}
			}
		}
	}
}

void WorldSession::HandleChangeSeatsOnControlledVehicle(WorldPacket &recv_data)
{
	sLog.outDebug("WORLD: Recvd CMSG_CHANGE_SEATS_ON_CONTROLLED_VEHICLE");
	recv_data.hexlike();

	if(!GetPlayer()->GetVehicle())
		return;

	if(recv_data.GetOpcode() == CMSG_REQUEST_VEHICLE_PREV_SEAT)
	{
		GetPlayer()->ChangeSeat(-1, false);
		return;
	}
	else if(recv_data.GetOpcode() == CMSG_REQUEST_VEHICLE_NEXT_SEAT)
	{
		GetPlayer()->ChangeSeat(-1, true);
		return;
	}
	else if(recv_data.GetOpcode() == CMSG_CHANGE_SEATS_ON_CONTROLLED_VEHICLE)
		ReadMovementInfo(recv_data, &GetPlayer()->GetVehicleBase()->m_movementInfo);

	uint64 guid;
	if(!recv_data.readPackGUID(guid))
		return;

	int8 seatId;
	recv_data >> seatId;

	if(!guid)
		GetPlayer()->ChangeSeat(-1, seatId > 0); // prev/next
	else if(Unit *vehUnit = vehUnit->GetUnit(*GetPlayer(), guid))
		if(Vehicle *vehicle = vehUnit->GetVehicleKit())
			if(vehicle->HasEmptySeat(seatId))
				GetPlayer()->EnterVehicle(vehicle, seatId);
}

void WorldSession::HandleDismissControlledVehicle(WorldPacket &recv_data)
{
	sLog.outDebug("WORLD: Recvd CMSG_DISMISS_CONTROLLED_VEHICLE");
	recv_data.hexlike();

	uint64 vehicleGUID = _player->GetCharmGUID();

	uint64 guid;

	if(!recv_data.readPackGUID(guid))
		return;

	if(!vehicleGUID)	// something wrong here...
	{
		recv_data.rpos(recv_data.wpos());	// prevent warnings spam
		return;
	}

	//mi.guid = guid;
	ReadMovementInfo(recv_data, &_player->m_mover->m_movementInfo);
	_player->ExitVehicle();
}

void WorldSession::HandleRequestVehicleExit(WorldPacket &recv_data)
{
	sLog.outDebug("WORLD: Recvd CMSG_REQUEST_VEHICLE_EXIT");
	recv_data.hexlike();
	GetPlayer()->ExitVehicle();
}

void Vehicle::RemoveAllPassengers()
{
	sLog.outDebug("Vehicle::RemoveAllPassengers");
	for(SeatMap::iterator itr = m_Seats.begin(); itr != m_Seats.end(); ++itr)
	{
		if(Unit *passenger = itr->second.passenger)
		{
			if(passenger->IsVehicle())
				passenger->GetVehicleKit()->RemoveAllPassengers();
			if(passenger->GetVehicle() != this)
				sLog.outDebug("Vehicle %u has invalid passenger %u.", me->GetEntry(), passenger->GetEntry());
			passenger->ExitVehicle();
			if(itr->second.passenger)
			{
				sLog.outDebug("Vehicle %u cannot remove passenger %u. %u is still on it.", me->GetEntry(), passenger->GetEntry(), itr->second.passenger->GetEntry());
				itr->second.passenger = NULL;
			}
		}
	}
}
