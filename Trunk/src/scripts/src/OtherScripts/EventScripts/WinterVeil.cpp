/*
 * Sun++ Scripts for Sandshroud MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2009-2011 Sandshroud <http://www.sandshroud.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
Event : Winter Veil
Author: Angelis
Team  : Sun++
*/

#include "StdAfx.h"
#include "../Setup.h"
#include "../../Common/EasyFunctions.h"

class PX238WinterWondervolt : public GameObjectAIScript
{
public:
	PX238WinterWondervolt(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject *GO) { return new PX238WinterWondervolt(GO); }

	void OnSpawn()
	{
		RegisterAIUpdateEvent( 1 );
	}

	void AIUpdate()
	{
		Player *plr = _gameobject->GetMapMgr()->GetInterface()->GetPlayerNearestCoords( _gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ() );
		if( !plr )
			return;

		if( _gameobject->CalcDistance( _gameobject, plr ) <= 1.050000f && !plr->HasAura( 26274 ) ) // aura given by the PX-238 Winter Wondervolt
		{
			plr->CastSpell( plr, 26275 , true ); // Spell that change into random gnome dispalyid (respect male & female)
		}
	}
};

void WinterReveler(Player *pPlayer, Unit *pUnit )
{
	if( pUnit->GetEntry() == 15760 )
	{
		uint32 Winteritem = 0;
		SlotResult slotresult;

		uint32 chance = RandomUInt(2);
		switch( chance )
		{
		case 0:
			{
			ItemPrototype *proto = ItemPrototypeStorage.LookupEntry( 21212 );
			if( !proto )
				return;

			slotresult = pPlayer->GetItemInterface()->FindFreeInventorySlot( proto );
			Winteritem = 21212;
			} break;

		case 1:
			{
			ItemPrototype *proto = ItemPrototypeStorage.LookupEntry( 21519 );
			if( !proto )
				return;

			slotresult = pPlayer->GetItemInterface()->FindFreeInventorySlot( proto );
			Winteritem = 21519;
			} break;

		case 2:
			{
			ItemPrototype *proto = ItemPrototypeStorage.LookupEntry( 34191 );
			if( !proto )
				return;

			slotresult = pPlayer->GetItemInterface()->FindFreeInventorySlot( proto );
			Winteritem = 34191;
			} break;

		}

		if( !slotresult.Result )
		{
			pPlayer->GetItemInterface()->BuildInventoryChangeError(NULLITEM, NULLITEM, INV_ERR_INVENTORY_FULL);
			return;
		}
		else
		{
			Item *itm = objmgr.CreateItem( Winteritem, pPlayer );
			itm->SetUInt32Value(ITEM_FIELD_STACK_COUNT, 5);
			pPlayer->GetItemInterface()->SafeAddItem( itm, slotresult.ContainerSlot, slotresult.Slot);
			pUnit->CastSpell( pPlayer, 26218, true );
		}
	}
}

void WinterVeilEmote(Player *pPlayer, uint32 Emote, Unit *pUnit)
{
	pUnit = pPlayer->GetMapMgr()->GetUnit(pPlayer->GetSelection());
	if (!pUnit || !pUnit->isAlive() || pUnit->GetAIInterface()->GetNextTarget())
		return;

	if( Emote == EMOTE_ONESHOT_KISS )
	{
		if( !pPlayer->HasAura( 26218 ) )
			WinterReveler( pPlayer, pUnit );
	}
}

void SetupWinterVeil(ScriptMgr * mgr)
{
	mgr->register_gameobject_script(180796, &PX238WinterWondervolt::Create);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_EMOTE, (void *)&WinterVeilEmote);
}