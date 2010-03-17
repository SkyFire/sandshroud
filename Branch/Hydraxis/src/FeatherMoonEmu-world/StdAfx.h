/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
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

#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <string>
//#include <fstream>

#include "../FeathermoonEmu-shared/Common.h"
#include "../FeathermoonEmu-shared/SharedPtr.h"
#include "../FeathermoonEmu-shared/MersenneTwister.h"
#include "../FeathermoonEmu-shared/WorldPacket.h"
#include "../FeathermoonEmu-shared/Log.h"
#include "../FeathermoonEmu-shared/NGLog.h"
#include "../FeathermoonEmu-shared/ByteBuffer.h"
#include "../FeathermoonEmu-shared/StackBuffer.h"
#include "../FeathermoonEmu-shared/Config/ConfigEnv.h"
#include "../FeathermoonEmu-shared/crc32.h"
#include "../FeathermoonEmu-shared/LocationVector.h"
#include "../FeathermoonEmu-shared/hashmap.h"

#include "../FeathermoonEmu-shared/Collision/vmap/IVMapManager.h"
#include "../FeathermoonEmu-shared/Collision/vmap/VMapManager.h"

#include <zlib.h>

#include "../FeathermoonEmu-shared/Database/DatabaseEnv.h"
#include "../FeathermoonEmu-shared/Database/DBCStores.h"
#include "../FeathermoonEmu-shared/Database/dbcfile.h"

#include <Network/Network.h>

#include "../FeathermoonEmu-shared/Auth/MD5.h"
#include "../FeathermoonEmu-shared/Auth/BigNumber.h"
#include "../FeathermoonEmu-shared/Auth/Sha1.h"
#include "../FeathermoonEmu-shared/Auth/WowCrypt.h"
#include "../FeathermoonEmu-shared/CrashHandler.h"
#include "../FeathermoonEmu-shared/FastQueue.h"
#include "../FeathermoonEmu-shared/CircularQueue.h"
#include "../FeathermoonEmu-shared/Threading/RWLock.h"
#include "../FeathermoonEmu-shared/Threading/Condition.h"
#include "../FeathermoonEmu-shared/FeathermoonEmu_getopt.h"
#include "../FeathermoonEmu-shared/BufferPool.h"

#include "Const.h"
#include "NameTables.h"
#include "UpdateFields.h"
#include "UpdateMask.h"
#include "Opcodes.h"
#include "WorldStates.h"

#ifdef CLUSTERING
	#include "../FeathermoonEmu-realmserver/WorkerOpcodes.h"
#endif

#include "Packets.h"

#include "../FeathermoonEmu-shared/CallBack.h"
#include "WordFilter.h"
#include "SharedDefines.h"
#include "EventMgr.h"
#include "EventableObject.h"
#include "Object.h"
#include "LootMgr.h"
#include "Unit.h"

#include "MainServerDefines.h"
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
#include "NPCHandler.h"
#include "Pet.h"
#include "WorldSocket.h"
#include "World.h"
#include "WorldSession.h"
#include "WorldStateManager.h"
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
#include "MainServerDefines.h"
#include "WorldRunnable.h"
#include "../FeathermoonEmu-shared/Storage.h"
#include "ObjectStorage.h"
#include "DayWatcherThread.h"
#include "VoiceChatHandler.h"
#include "LocalizationMgr.h"
#include "Vehicle.h"
#include "AchievementInterface.h"
#include "ChainAggroEntity.h"
#include "StrandOfTheAncients.h"

#ifdef CLUSTERING
	#include "WorkerServerClient.h"
	#include "ClusterInterface.h"
#endif

#include "CollideInterface.h"

#include "Master.h"
#include "BaseConsole.h"
#include "CConsole.h"
#include "Wintergrasp.h"
#include "IsleOfConquest.h"
//#define COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS 1

#endif
