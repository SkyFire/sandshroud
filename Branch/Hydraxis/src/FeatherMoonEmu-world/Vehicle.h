/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#ifndef _VEHICLE_H
#define _VEHICLE_H

struct VehicleEntry;
struct VehicleSeatEntry;
class Object;

enum PowerType
{
	POWER_STEAM		= 61,
	POWER_PYRITE	= 41,
};

struct VehicleSeat
{
	explicit VehicleSeat(VehicleSeatEntry const *_seatInfo) : seatInfo(_seatInfo), passenger(NULL) {}
	VehicleSeatEntry const *seatInfo;
	Unit* passenger;
};

typedef std::map<int8, VehicleSeat> SeatMap;

class SERVER_DECL Vehicle : public Creature
{
public:
	Vehicle(uint64 guid, VehicleEntry const *vehInfo);
	~Vehicle();
	virtual void Destructor();

	Unit *GetBase() const { return me; }
	void Install();
	void Uninstall();
	void Reset();
	void Die();
	void Dismiss();
	void UpdatePassengers();
	void RemoveAllPassengers();
	void InstallAllAccessories();
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
	bool AddPassenger(Unit *passenger, int8 seatId/* Default to -1 */);
	void RemovePassenger(Unit* pPassenger);
	bool HasPassenger(Unit* pPassenger);
	void SendSpells(uint32 entry, Player* plr);
	void setDeathState(DeathState s);
	void SetSpeed(uint8 SpeedType, float value);
	bool HasEmptySeat(int8 seatId) const;
	VehicleEntry const *GetVehicleInfo() { return m_vehicleInfo; }

	//---------------------------------------
	// Accessors
	//---------------------------------------
	uint32 GetMaxPassengerCount() { return m_maxPassengers; }
	uint32 GetPassengerCount() { return m_passengerCount; }

	uint32 GetVehicleEntry() { return m_vehicleEntry; }
	void SetVehicleEntry(uint32 entry) { m_vehicleEntry = entry; }

	Unit* GetControllingUnit() { return m_passengers[0]; }
	void SetControllingUnit(Unit* pUnit) { m_controllingUnit = pUnit; }

	uint8 GetPassengerSlot(Unit* pPassenger);
	int8 GetNextEmptySeat(int8 seatId, bool next) const;
	//---------------------------------------
	// End accessors
	//---------------------------------------

	Unit *GetPassenger(int8 seatId) const;
	bool IsFull() { return m_passengerCount == m_maxPassengers; }

	VehicleSeatEntry * m_vehicleSeats[8];
	bool Initialised;
	bool m_CreatedFromSpell;
	uint32 m_mountSpell;
	
	SeatMap m_Seats;

private:
	void _AddToSlot(Unit* pPassenger, uint8 slot);

protected:
	Unit *me;
	Object *obj;
	VehicleEntry const *m_vehicleInfo;
	uint32 m_usableSeatNum;

	Unit* m_controllingUnit;

	Unit* m_passengers[8];

	uint8 m_passengerCount;
	uint8 m_maxPassengers;
	uint32 m_vehicleEntry;
	
	void InstallAccessory(uint32 entry, int8 seatId, bool minion = true);
};

#endif
