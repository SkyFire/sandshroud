/*
 * Sandshroud Onyx by Thetruecrow@Sandshroud
 *
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

#ifndef __STDAFX_H
#define __STDAFX_H

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#define _GAME

#include <cstring>
#include <cstdlib>
#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <string>
//#include <fstream>

#include "../onyx-shared/Common.h"
#include "../onyx-shared/MersenneTwister.h"
#include "../onyx-shared/WorldPacket.h"
#include "../onyx-shared/Log.h"
#include "../onyx-shared/NGLog.h"
#include "../onyx-shared/ByteBuffer.h"
#include "../onyx-shared/StackBuffer.h"
#include "../onyx-shared/Config/ConfigEnv.h"
#include "../onyx-shared/crc32.h"
#include "../onyx-shared/LocationVector.h"

#include <zlib.h>

#include "../onyx-shared/Database/DatabaseEnv.h"
#include "../onyx-shared/Database/DBCStores.h"
#include "../onyx-shared/Database/dbcfile.h"

#include <Network/Network.h>

#include "../onyx-shared/Auth/MD5.h"
#include "../onyx-shared/Auth/BigNumber.h"
#include "../onyx-shared/Auth/Sha1.h"
#include "../onyx-shared/Auth/WowCrypt.h"
#include "../onyx-shared/CrashHandler.h"
#include "../onyx-shared/FastQueue.h"
#include "../onyx-shared/CircularQueue.h"
#include "../onyx-shared/Threading/RWLock.h"
#include "../onyx-shared/Threading/Condition.h"
#include "../onyx-shared/onyx_getopt.h"

#include "UpdateFields.h"
#include "UpdateMask.h"
#include "Opcodes.h"

#ifdef CLUSTERING
	#include "../onyx-realmserver/WorkerOpcodes.h"
#endif

#include "Packets.h"
#include "PacketEncrypt.h"

#include "../onyx-shared/CallBack.h"
#include "WordFilter.h"
#include "EventMgr.h"
#include "EventableObject.h"
#include "Object.h"
#include "LootMgr.h"
#include "Unit.h"

#include "AddonMgr.h"
#include "AIInterface.h"
#include "AreaTrigger.h"
#include "BattlegroundMgr.h"
#include "AlteracValley.h"
#include "ArathiBasin.h"
#include "EyeOfTheStorm.h"
#include "Arenas.h"
#include "CellHandler.h"
#include "Chat.h"
#include "Corpse.h"
#include "Quest.h"
#include "QuestMgr.h"
#include "Creature.h"
#include "DynamicObject.h"
#include "GameObject.h"
#include "Group.h"
#include "Guild.h"
#include "HonorHandler.h"
#include "ItemPrototype.h"
#include "Item.h"
#include "Container.h"
#include "AuctionHouse.h"
#include "AuctionMgr.h"
#include "LfgMgr.h"
#include "MailSystem.h"
#include "Map.h"
#include "MapCell.h"
#include "TerrainMgr.h"
#include "MiscHandler.h"
#include "NameTables.h"
#include "NPCHandler.h"
#include "Pet.h"
#include "WorldSocket.h"
#include "WorldSession.h"
#include "MapMgr.h"
#include "MapScriptInterface.h"
#include "Player.h"
#include "faction.h"
#include "Skill.h"
#include "SkillNameMgr.h"
#include "SpellNameHashes.h"
#include "Spell.h"
#include "SpellAuras.h"
#include "TaxiMgr.h"
#include "TransporterHandler.h"
#include "WarsongGulch.h"
#include "WeatherMgr.h"
#include "World.h"
#include "ItemInterface.h"
#include "Stats.h"
#include "WorldCreator.h"
#include "ObjectPool.h"
#include "Strike.h"
#include "WorldStateHandler.h"

#include "ObjectMgr.h"

#include "CThreads.h"
#include "ScriptMgr.h"

#include "Channel.h"
#include "ChannelMgr.h"
#include "ArenaTeam.h"

#include "LogonCommClient.h"
#include "LogonCommHandler.h"
#include "MainServerDefines.h"
#include "WorldRunnable.h"

#include "../onyx-shared/Storage.h"
#include "ObjectStorage.h"
#include "DatabaseCleaner.h"
#include "DayWatcherThread.h"
#include "VoiceChatHandler.h"
#include "LocalizationMgr.h"

#ifdef CLUSTERING
	#include "WorkerServerClient.h"
	#include "ClusterInterface.h"
#endif

#ifdef COLLISION
#include "CollideInterface.h"
#endif

#include "Master.h"
#include "BaseConsole.h"
#include "CConsole.h"
//#define COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS 1

#endif
