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

//HEADER GUARDS!
#ifndef __RSTDAFX_H
#define __RSTDAFX_H

#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <signal.h>
//#include <fstream>

#include "../hearthstone-shared/AuthCodes.h"
#include "../hearthstone-shared/svn_revision.h"
#include "../hearthstone-shared/Common.h"
#include "../hearthstone-shared/MersenneTwister.h"
#include "../hearthstone-shared/WorldPacket.h"
#include "../hearthstone-shared/Log.h"
#include "../hearthstone-shared/NGLog.h"
#include "../hearthstone-shared/ByteBuffer.h"
#include "../hearthstone-shared/Config/ConfigEnv.h"
#include "../hearthstone-shared/crc32.h"
#include "../hearthstone-shared/LocationVector.h"

#include "../../dependencies/VC/include/zlib.h"

#include "../hearthstone-shared/Database/DatabaseEnv.h"
#include "../hearthstone-shared/Database/DBCStores.h"
#include "../hearthstone-shared/Database/dbcfile.h"
#include "../hearthstone-shared/FastQueue.h"
#include "../hearthstone-shared/Threading/RWLock.h"

#include <Network/Network.h>

#include "../hearthstone-shared/Auth/MD5.h"
#include "../hearthstone-shared/Auth/BigNumber.h"
#include "../hearthstone-shared/Auth/Sha1.h"
#include "../hearthstone-shared/Auth/WowCrypt.h"
#include "../hearthstone-shared/CrashHandler.h"

#include "../hearthstone-world/NameTables.h"
#include "../hearthstone-world/Opcodes.h"
#include "../hearthstone-shared/CallBack.h"

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
#include "../hearthstone-shared/Storage.h"
#include "ObjectStorage.h"
#include "Player.h"

#endif