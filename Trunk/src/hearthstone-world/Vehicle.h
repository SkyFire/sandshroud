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
	virtual void Init();
	virtual void Destruct();

	void InitSeats(uint32 vehicleEntry, Player* pRider = NULLPLR);
	virtual void Update(uint32 time);
	bool Load(CreatureSpawn *spawn, uint32 mode, MapInfo *info);
	void Load(CreatureProto * proto_, uint32 mode, float x, float y, float z, float o = 0.0f);
	void OnPushToWorld();
	void Despawn(uint32 delay, uint32 respawntime);
	void DeleteMe();
	void SafeDelete();
	void AddPassenger(Unit* pPassenger, int8 requestedseat = -1, bool force = false);
	void RemovePassenger(Unit* pPassenger);
	bool HasPassenger(Unit* pPassenger);
	void SendSpells(uint32 entry, Player* plr);
	void VehicleSetDeathState(DeathState s);
	void ChangeSeats(Unit* pPassenger, uint8 seatid);
	void ChangePowerType();
	//---------------------------------------
	// Accessors
	//---------------------------------------
	uint32 GetMaxPassengerCount() { return m_maxPassengers; }
	uint32 GetPassengerCount() { return m_ppassengerCount; }

	int8 GetMaxSeat() { return m_seatSlotMax; }
	Unit* GetPassenger(uint8 seat)
	{
		if(seat >= 8)
			return NULL;

		return m_passengers[seat];
	}

	void InstallAccessories();
	//---------------------------------------
	// End accessors
	//---------------------------------------

	bool IsFull() { return m_ppassengerCount >= m_maxPassengers; }
	bool Initialised;
	bool m_CreatedFromSpell;
	uint32 m_CastSpellOnMount;

private:
	void _AddToSlot(Unit* pPassenger, uint8 slot);

protected:
	uint64 vehicleguid;
	uint8 m_ppassengerCount;
	uint8 m_maxPassengers;
	uint8 m_seatSlotMax;
};

#endif
