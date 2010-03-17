/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#ifndef _CONTAINER_H
#define _CONTAINER_H

// Maximum 36 Slots ( (CONTAINER_END - CONTAINER_FIELD_SLOT_1)/2
#define MAX_BAG_SIZE 36										// 2.0.12

class SERVER_DECL Container : public Item
{
public:
	friend class WorldSession;
	Container(uint32 high, uint32 low);
	~Container();
	virtual void Init();
	virtual void Destructor();

	void Create( uint32 itemid, Player* owner );
	void LoadFromDB( Field*fields);

	bool AddItem(int8 slot, Item* item);
	bool AddItemToFreeSlot(Item* pItem, uint32 * r_slot);
	Item* GetItem(int8 slot)
	{
		if(slot >= 0 && (uint8)slot < GetProto()->ContainerSlots)
			return m_Slot[slot];
		else
			return NULLITEM;
	}
	
	int8 FindFreeSlot();
	bool HasItems();
	
	void SwapItems(int8 SrcSlot,int8 DstSlot);
	Item* SafeRemoveAndRetreiveItemFromSlot(int8 slot, bool destroy); //doesnt destroy item from memory
	bool SafeFullRemoveItemFromSlot(int8 slot); //destroys item fully

	void SaveBagToDB(int8 slot, bool first, QueryBuffer * buf);
	uint32 GetBagSize() const { return GetUInt32Value(CONTAINER_FIELD_NUM_SLOTS); }

protected:
	// Bag Storage space
	Item* m_bagslot[MAX_BAG_SIZE];
	Item* m_Slot[72];
	uint32 __fields[CONTAINER_END];
};

#endif
