/*
 * onyx MMORPG Server
 * Copyright (C) 2005-2008 onyx Team <http://www.onyxemu.com/>
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

#pragma once

#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <string>
//#include <fstream>

#include "Common.h"
#include <Network/Network.h>

#include "../onyx-shared/Log.h"
#include "../onyx-shared/Util.h"
#include "../onyx-shared/ByteBuffer.h"
#include "../onyx-shared/Config/ConfigEnv.h"
#include <zlib.h>

#include "../onyx-shared/Database/DatabaseEnv.h"
#include "../onyx-shared/Database/DBCStores.h"
#include "../onyx-shared/Database/dbcfile.h"

#include "../onyx-shared/Auth/BigNumber.h"
#include "../onyx-shared/Auth/Sha1.h"
#include "../onyx-shared/Auth/WowCrypt.h"
#include "../onyx-shared/CrashHandler.h"

#include "LogonOpcodes.h"
#include "../onyx-logonserver/Main.h"
#include "../onyx-world/NameTables.h"
#include "AccountCache.h"
#include "PeriodicFunctionCall_Thread.h"
#include "../onyx-logonserver/AutoPatcher.h"
#include "../onyx-logonserver/AuthSocket.h"
#include "../onyx-logonserver/AuthStructs.h"
#include "../onyx-logonserver/LogonOpcodes.h"
#include "../onyx-logonserver/LogonCommServer.h"
#include "../onyx-logonserver/LogonConsole.h"
#include "../onyx-shared/WorldPacket.h"

// database decl
extern Database* sLogonSQL;
