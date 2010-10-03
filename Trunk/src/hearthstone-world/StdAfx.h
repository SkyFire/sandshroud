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

#ifndef __STDAFX_H
#define __STDAFX_H

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#define _GAME // You just lost the game

#ifdef WIN32
#pragma warning(disable:4127)
#pragma warning(disable:4100)
#pragma warning(disable:4706)
#pragma warning(disable:4714)
#endif

#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <fstream>
#include <iosfwd>
#include <search.h>
#include <fcntl.h>
#include <signal.h>
//#include <fstream>

#include "../hearthstone-shared/Common.h"
#include "../hearthstone-shared/svn_revision.h"
#include "../hearthstone-shared/MersenneTwister.h"
#include "../hearthstone-shared/WorldPacket.h"
#include "../hearthstone-shared/Log.h"
#include "../hearthstone-shared/NGLog.h"
#include "../hearthstone-shared/ByteBuffer.h"
#include "../hearthstone-shared/StackBuffer.h"
#include "../hearthstone-shared/Config/ConfigEnv.h"
#include "../hearthstone-shared/crc32.h"
#include "../hearthstone-shared/LocationVector.h"
#include "../hearthstone-shared/hashmap.h"
#include "../hearthstone-shared/hearthstoneConfig.h"

#include "../hearthstone-shared/Collision/vmap/IVMapManager.h"
#include "../hearthstone-shared/Collision/vmap/VMapManager2.h"
#include "../hearthstone-shared/Collision/vmap/MapTree.h"

#include "../hearthstone-shared/Pathfinding/Recast/Recast.h"
#include "../hearthstone-shared/Pathfinding/Detour/DetourCommon.h"
#include "../hearthstone-shared/Pathfinding/Detour/DetourNavMesh.h"
#include "../hearthstone-shared/Pathfinding/Detour/DetourAlloc.h"
#include "../hearthstone-shared/Pathfinding/Detour/DetourNavMeshQuery.h"
#include "../hearthstone-shared/Pathfinding/Detour/DetourNavMeshBuilder.h"
#include "Path.h"
#include "PathFinder.h"

#include "../../dependencies/VC/include/zlib.h"
#include "Const.h"

#include "../hearthstone-shared/RC4Engine.h"
#include "../hearthstone-shared/Database/DatabaseEnv.h"
#include "../hearthstone-shared/Database/DBCStores.h"
#include "../hearthstone-shared/Database/dbcfile.h"
#include "../hearthstone-shared/Network/Network.h"

#include "../hearthstone-shared/AuthCodes.h"
#include "../hearthstone-shared/Auth/MD5.h"
#include "../hearthstone-shared/Auth/BigNumber.h"
#include "../hearthstone-shared/Auth/Sha1.h"
#include "../hearthstone-shared/Auth/WowCrypt.h"
#include "../hearthstone-shared/CrashHandler.h"
#include "../hearthstone-shared/FastQueue.h"
#include "../hearthstone-shared/CircularQueue.h"
#include "../hearthstone-shared/Threading/RWLock.h"
#include "../hearthstone-shared/Threading/Condition.h"
#include "../hearthstone-shared/hearthstone_getopt.h"
#include "../hearthstone-shared/BufferPool.h"
#include "../hearthstone-shared/CallBack.h"
#include "../hearthstone-shared/Storage.h"
#include "../hearthstone-logonserver/LogonOpcodes.h"
#include "../hearthstone-voicechat/hearthstone_opcodes.h"

#include "NameTables.h"
#include "UpdateFields.h"
#include "UpdateMask.h"
#include "Opcodes.h"
#include "WorldStates.h"

#ifdef CLUSTERING
#include "../hearthstone-realmserver/Structures.h"
#include "../hearthstone-realmserver/WorkerOpcodes.h"
#endif

#include "Packets.h"
#include "CallScripting.h"
#include "WordFilter.h"
#include "EventMgr.h"
#include "EventableObject.h"
#include "LootMgr.h"
#include "Object.h"
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
#include "SkillNameMgr.h"
#include "Chat.h"
#include "Corpse.h"
#include "Quest.h"
#include "QuestMgr.h"
#include "TerrainMgr.h"
#include "Map.h"
#include "Creature.h"
#include "DynamicObject.h"
#include "GameObject.h"
#include "Group.h"
#include "Guild.h"
#include "HonorHandler.h"
#include "ItemPrototype.h"
#include "Skill.h"
#include "Item.h"
#include "Container.h"
#include "AuctionHouse.h"
#include "AuctionMgr.h"
#include "LfgMgr.h"
#include "MailSystem.h"
#include "MapCell.h"
#include "MiscHandler.h"
#include "NPCHandler.h"
#include "Pet.h"
#include "WorldSocket.h"
#include "World.h"
#include "WorldSession.h"
#include "WorldStateManager.h"
#include "MapMgr.h"
#include "MapScriptInterface.h"
#include "DayWatcherThread.h"
#include "WintergraspInternal.h"
#include "Wintergrasp.h"
#include "Player.h"
#include "faction.h"
#include "SpellNameHashes.h"
#include "SpellFailure.h"
#include "Spell.h"
#include "SpellAuras.h"
#include "ClassSpells.h"
#include "TaxiMgr.h"
#include "TransporterHandler.h"
#include "WarsongGulch.h"
#include "WeatherMgr.h"
#include "ItemInterface.h"
#include "Stats.h"
#include "ObjectMgr.h"
#include "WorldCreator.h"
#include "ScriptMgr.h"
#include "Channel.h"
#include "ChannelMgr.h"
#include "ArenaTeam.h"
#include "LogonCommClient.h"
#include "LogonCommHandler.h"
#include "WorldRunnable.h"
#include "ObjectStorage.h"
#include "VoiceChatClientSocket.h"
#include "VoiceChatHandler.h"
#include "LocalizationMgr.h"
#include "Vehicle.h"
#include "AchievementInterface.h"
#include "ChainAggroEntity.h"
#include "StrandOfTheAncients.h"
#include "IsleOfConquest.h"

#ifdef CLUSTERING
	#include "WorkerServerClient.h"
	#include "ClusterInterface.h"
#endif

#include "CollideInterface.h"
#include "NavMeshInterface.h"

#include "Master.h"
#include "BaseConsole.h"
#include "CConsole.h"
#include "ConsoleCommands.h"
//#define COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS 1

//#include <vld.h>
#ifndef WIN32

#include <termios.h>
#include <sys/resource.h>
#include <sched.h>
#include <dlfcn.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <cstring>

#else

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#define DELTA_EPOCH_IN_USEC  11644473600000000ULL

#endif

extern "C" {
#include "../../dependencies/VC/include/pcre.h"
};

#endif
