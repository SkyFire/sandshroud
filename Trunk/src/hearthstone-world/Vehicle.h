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

#ifndef _VEHICLE_H
#define _VEHICLE_H

class SERVER_DECL Vehicle : public Creature
{
public:
	Vehicle(uint64 guid);
	~Vehicle();

	void Init();
	void InitSeats(uint32 vehicleEntry, Player* pRider = NULLPLR);
	virtual void Update(uint32 time);
	bool Load(CreatureSpawn *spawn, uint32 mode, MapInfo *info);
	void Load(CreatureProto * proto_, float x, float y, float z, float o = 0.0f);
	void Despawn(uint32 delay, uint32 respawntime);
	void DeleteMe();
	void SafeDelete();
	void MoveVehicle(float x, float y, float z, float o);
	void AddPassenger(Unit* pPassenger);
	void AddPassenger(Unit* pPassenger, uint8 requestedseat, bool force = false);
	void RemovePassenger(Unit* pPassenger);
	bool HasPassenger(Unit* pPassenger);
	void SendSpells(uint32 entry, Player* plr);
	void setDeathState(DeathState s);
	void SetSpeed(uint8 SpeedType, float value);
	void ChangeSeats(Unit* pPassenger, uint8 seatid);

	//---------------------------------------
	// Accessors
	//---------------------------------------
	uint32 GetMaxPassengerCount() { return m_maxPassengers; }
	uint32 GetPassengerCount() { return m_passengerCount; }

	uint32 GetVehicleEntry() { return m_vehicleEntry; }
	void SetVehicleEntry(uint32 entry) { m_vehicleEntry = entry; }

	Unit* GetControllingUnit() { return m_passengers[0]; }
	void SetControllingUnit(Unit* pUnit) { m_controllingUnit = pUnit; }

	int8 GetMaxSeat() { return m_seatSlotMax; }
	Unit* GetPassenger(uint8 seat)
	{
		if(seat >= 8)
			return NULL;

		return m_passengers[seat] ? m_passengers[seat] : NULL;
	}

	uint8 GetPassengerSlot(Unit* pPassenger);
	void InstallAccessories();
	//---------------------------------------
	// End accessors
	//---------------------------------------

	bool IsFull() { return m_passengerCount >= m_maxPassengers; }

	VehicleSeatEntry* m_vehicleSeats[8];
	bool seatisusable[8];
	bool Initialised;
	bool m_CreatedFromSpell;
	uint32 m_mountSpell;

private:
	void _AddToSlot(Unit* pPassenger, uint8 slot);

protected:
	uint64 vehicleguid;
	Unit* m_controllingUnit;
	Unit* m_passengers[8];

	uint8 m_passengerCount;
	uint8 m_maxPassengers;
	int8 m_seatSlotMax;
	uint32 m_vehicleEntry;
};
class Mount
{
public:
	Mount(Unit * owner, uint32 vehicle_entry, uint32 spellId);
	~Mount();
	VehicleSeatEntry* m_vehicleSeats[8];
	void ClearPassengers();
	void SendRidePacket(Unit * passenger );
	void SendHeartbeatPacket(Unit *passenger);
	HEARTHSTONE_INLINE uint32 Getvehicle_entry() { return m_vehicle_entry; }

	/************************************************************************/
	/* Passenger Info Functions												*/
	/************************************************************************/

	Unit * GetOwner() { return m_mountowner; }

	Unit * GetPassenger(uint8 seat)
	{
		ASSERT(seat < 4);
		return m_passengers[seat];
	}

	int8 GetFreeSeat();

	/************************************************************************/
	/* Passenger Control Functions                                          */
	/************************************************************************/

	void AddPassenger(Unit * passenger, int8 seat = -1);
	void RemovePassenger(Unit * passenger);
	void RemovePassenger(uint8 seat);

	/************************************************************************/
	/* Vehicle Mount Packet Functions                                       */
	/************************************************************************/
	void ConvertToVehicle(uint32 vehicle_entry);

private:
	Unit * m_mountowner;							
	Unit * m_passengers[4];		
	uint32 m_vehicle_entry;
	uint8 m_capacity;
	uint32 m_spellId;
 };
 
#endif
