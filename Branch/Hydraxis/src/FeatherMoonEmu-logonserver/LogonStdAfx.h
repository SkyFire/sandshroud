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

#include "Common.h"
#include <Network/Network.h>

#include "../FeatherMoonEmu-shared/Log.h"
#include "../FeatherMoonEmu-shared/Util.h"
#include "../FeatherMoonEmu-shared/ByteBuffer.h"
#include "../FeatherMoonEmu-shared/Config/ConfigEnv.h"
#include <zlib.h>

#include "../FeatherMoonEmu-shared/Database/DatabaseEnv.h"
#include "../FeatherMoonEmu-shared/Database/DBCStores.h"
#include "../FeatherMoonEmu-shared/Database/dbcfile.h"

#include "../FeatherMoonEmu-shared/Auth/BigNumber.h"
#include "../FeatherMoonEmu-shared/Auth/Sha1.h"
#include "../FeatherMoonEmu-shared/Auth/WowCrypt.h"
#include "../FeatherMoonEmu-shared/CrashHandler.h"

#include "LogonOpcodes.h"
#include "../FeatherMoonEmu-logonserver/Main.h"
#include "../FeatherMoonEmu-world/NameTables.h"
#include "AccountCache.h"
#include "PeriodicFunctionCall_Thread.h"
#include "../FeatherMoonEmu-logonserver/AutoPatcher.h"
#include "../FeatherMoonEmu-logonserver/AuthSocket.h"
#include "../FeatherMoonEmu-logonserver/AuthStructs.h"
#include "../FeatherMoonEmu-logonserver/LogonOpcodes.h"
#include "../FeatherMoonEmu-logonserver/LogonCommServer.h"
#include "../FeatherMoonEmu-logonserver/LogonConsole.h"
#include "../FeatherMoonEmu-shared/WorldPacket.h"

// database decl
extern Database* sLogonSQL;
