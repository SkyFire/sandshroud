/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#pragma once

#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <string>
//#include <fstream>

#include "../FeatherMoonEmu-shared/Common.h"
#include "../FeatherMoonEmu-shared/MersenneTwister.h"
#include "../FeatherMoonEmu-shared/WorldPacket.h"
#include "../FeatherMoonEmu-shared/Log.h"
#include "../FeatherMoonEmu-shared/NGLog.h"
#include "../FeatherMoonEmu-shared/ByteBuffer.h"
#include "../FeatherMoonEmu-shared/StackBuffer.h"
#include "../FeatherMoonEmu-shared/Config/ConfigEnv.h"
#include "../FeatherMoonEmu-shared/crc32.h"
#include "../FeatherMoonEmu-shared/LocationVector.h"

#include <zlib.h>

#include "../FeatherMoonEmu-shared/Database/DatabaseEnv.h"
#include "../FeatherMoonEmu-shared/Database/DBCStores.h"
#include "../FeatherMoonEmu-shared/Database/dbcfile.h"
#include "../FeatherMoonEmu-shared/FastQueue.h"

#include <Network/Network.h>

#include "../FeatherMoonEmu-shared/Auth/MD5.h"
#include "../FeatherMoonEmu-shared/Auth/BigNumber.h"
#include "../FeatherMoonEmu-shared/Auth/Sha1.h"
#include "../FeatherMoonEmu-shared/Auth/WowCrypt.h"
#include "../FeatherMoonEmu-shared/CrashHandler.h"

#include "../FeatherMoonEmu-world/NameTables.h"
#include "../FeatherMoonEmu-world/Opcodes.h"
#include "../FeatherMoonEmu-shared/CallBack.h"

#include "Master.h"
#include "WorkerOpcodes.h"
#include "WorkerServerSocket.h"
#include "Structures.h"
#include "WorldSocket.h"
#include "Session.h"
#include "WorkerServer.h"
#include "ClusterManager.h"
#include "ClientManager.h"
#include "LogonCommClient.h"
#include "LogonCommHandler.h"
#include "../FeatherMoonEmu-shared/Storage.h"
#include "ObjectStorage.h"