/*
 * Sandshroud Zeon
 * Copyright (c) 2009 Mikko Mononen memon@inside.org
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

#ifndef RECAST_LOG_H
#define RECAST_LOG_H

enum rcLogCategory
{
	RC_LOG_PROGRESS = 1,
	RC_LOG_WARNING,
	RC_LOG_ERROR,
};

class rcLog
{
public:
	rcLog();
	~rcLog();
	
	void log(rcLogCategory category, const char* format, ...);
	inline void clear() { m_messageCount = 0; m_textPoolSize = 0; }
	inline int getMessageCount() const { return m_messageCount; }
	inline char getMessageType(int i) const { return *m_messages[i]; }
	inline const char* getMessageText(int i) const { return m_messages[i]+1; }

private:
	static const int MAX_MESSAGES = 1000;
	const char* m_messages[MAX_MESSAGES];
	int m_messageCount;
	static const int TEXT_POOL_SIZE = 8000;
	char m_textPool[TEXT_POOL_SIZE];
	int m_textPoolSize;
};

struct rcBuildTimes
{
	int rasterizeTriangles;
	int buildCompact;
	int buildContours;
	int buildContoursTrace;
	int buildContoursSimplify;
	int filterBorder;
	int filterWalkable;
	int filterMarkReachable;
	int buildPolymesh;
	int erodeArea;
	int buildDistanceField;
	int buildDistanceFieldDist;
	int buildDistanceFieldBlur;
	int buildRegions;
	int buildRegionsReg;
	int buildRegionsExp;
	int buildRegionsFlood;
	int buildRegionsFilter;
	int buildDetailMesh;
	int mergePolyMesh;
	int mergePolyMeshDetail;
};

void rcSetLog(rcLog* log);
rcLog* rcGetLog();

void rcSetBuildTimes(rcBuildTimes* btimes);
rcBuildTimes* rcGetBuildTimes();

#endif // RECAST_LOG_H
