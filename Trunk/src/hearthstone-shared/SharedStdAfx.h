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

#include "Database/dbcfile.h"
#include <stdio.h>
#include "Database/DBC.h"
#include "NGLog.h"
#include "Log.h"
#include <math.h>
#include "Common.h"
#include "Database/DBCStores.h"
#include "Database/DataStore.h"
#include "Config/ConfigEnv.h"
#include "ByteBuffer.h"
#include "Auth/BigNumber.h"
#include "../../dependencies/VC/include/openssl/bn.h"
#include <algorithm>
#include "crc32.h"
#include <stdlib.h>
#include <stdio.h>
#include "hashmap.h"
#include "../../dependencies/VC/include/openssl/md5.h"
#include "Auth/MD5.h"

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#pragma comment(lib, "version.lib")  // for "VerQueryValue"
#pragma warning(disable:4826)

#include "StackWalker.h"

#endif // SHARED_STDAFX