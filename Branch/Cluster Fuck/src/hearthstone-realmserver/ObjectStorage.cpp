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

const char * gAchievementRewardFormat					= "uuuubuss";
const char * gAreaTriggerFormat							= "ucuusffffuu";
const char * gCreatureNameFormat						= "usssuuuuuuuuuuuffcc";
const char * gFishingFormat								= "uuu";
const char * gGameObjectNameFormat						= "uuusssuuuuuuuuuuuuuuuuuuuuuuuuu";
const char * gItemPageFormat							= "usu";
const char * gItemPrototypeFormat						= "uuuisuuuuuuuiiuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuffuffuuuuuuuuuufuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuusuuuuuuuuuuuuuuuuuuuuuuuiiiuuu";
const char * gNpcTextFormat								= "ufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuu";
const char * gQuestFormat								= "uuuuuuuuuuuuuuuuuuuussssssssssuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuffuuuuuuuuuuuuuuuuuuucuuu";
const char * gWorldMapInfoFormat						= "usbuuufffuuuuuuuuuub";

/** SQLStorage symbols
 */
SERVER_DECL SQLStorage<CreatureInfo, HashMapStorageContainer<CreatureInfo> >					CreatureNameStorage;
SERVER_DECL SQLStorage<GameObjectInfo, HashMapStorageContainer<GameObjectInfo> >				GameObjectNameStorage;
SERVER_DECL SQLStorage<ItemPage, HashMapStorageContainer<ItemPage> >							ItemPageStorage;
SERVER_DECL SQLStorage<ItemPrototype, ArrayStorageContainer<ItemPrototype> >					ItemPrototypeStorage;
SERVER_DECL SQLStorage<GossipText, HashMapStorageContainer<GossipText> >						NpcTextStorage;
SERVER_DECL SQLStorage<Quest, HashMapStorageContainer<Quest> >									QuestStorage;
SERVER_DECL SQLStorage<MapInfo, HashMapStorageContainer<MapInfo> >								WorldMapInfoStorage;

void Storage_Load()
{
	ItemPrototypeStorage.Load("items", gItemPrototypeFormat);
	CreatureNameStorage.Load("creature_names", gCreatureNameFormat);
	GameObjectNameStorage.Load("gameobject_names", gGameObjectNameFormat);
	ItemPageStorage.Load("itempages", gItemPageFormat);
	QuestStorage.Load("quests", gQuestFormat);
	WorldMapInfoStorage.Load("worldmap_info", gWorldMapInfoFormat);
}

void Storage_Cleanup()
{
	ItemPrototypeStorage.Cleanup();
	CreatureNameStorage.Cleanup();
	GameObjectNameStorage.Cleanup();
	ItemPageStorage.Cleanup();
	QuestStorage.Cleanup();
	NpcTextStorage.Cleanup();
	WorldMapInfoStorage.Cleanup();
}

bool Storage_ReloadTable(const char * TableName)
{
	// bur: mah god this is ugly :P
	if(!stricmp(TableName, "items"))					// Items
		ItemPrototypeStorage.Reload();
	else if(!stricmp(TableName, "creature_names"))		// Creature Names
		CreatureNameStorage.Reload();
	else if(!stricmp(TableName, "gameobject_names"))	// GO Names
		GameObjectNameStorage.Reload();
	else if(!stricmp(TableName, "itempages"))			// Item Pages
		ItemPageStorage.Reload();
	else if(!stricmp(TableName, "quests"))				// Quests
		QuestStorage.Reload();
	else if(!stricmp(TableName, "npc_text"))			// NPC Text Storage
		NpcTextStorage.Reload();
	else if(!stricmp(TableName, "worldmap_info"))		// WorldMapInfo
		WorldMapInfoStorage.Reload();
	else
		return false;
	return true;
}
