/*
 * Sandshroud Hearthstone
 * Copyright (C) 2008 - 2009 - 2009 AspireDev <http://www.aspiredev.org/>
 * Copyright (C) 2009 - 2010 Sandshroud <http://www.sandshroud.org/>
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

#ifndef SHARED_STDAFX
#define SHARED_STDAFX

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <algorithm>
#include "Common.h"

#include "Database/dbcfile.h"
#include "Database/DBC.h"
#include "NGLog.h"
#include "Log.h"
#include "Database/DBCStores.h"
#include "Database/DataStore.h"
#include "Config/ConfigEnv.h"
#include "ByteBuffer.h"
#include "Auth/BigNumber.h"
#include "Auth/RSA1.h"
#include "Auth/RSA2.h"
#include "../../dependencies/VC/include/openssl/bn.h"
#include "crc32.h"
#include "hashmap.h"
#include "../../dependencies/VC/include/openssl/md5.h"
#include "Auth/MD5.h"
#include "int24.h"

#ifdef WIN32
#include <windows.h>
#include <tchar.h>
#include "StackWalker.h"
#pragma comment(lib, "version.lib")  // for "VerQueryValue"
#pragma warning(disable:4826)
#endif

#include "PerfCounters.h"

#endif // SHARED_STDAFX