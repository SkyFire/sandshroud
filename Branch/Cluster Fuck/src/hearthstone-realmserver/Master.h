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

#ifndef _R_MASTER_H
#define _R_MASTER_H

#ifdef CATACLYSM
#define CL_BUILD_SUPPORT 13205
#else
#define CL_BUILD_SUPPORT 12340
#endif

extern Database * Database_World;
extern Database * Database_Character;

#define WorldDatabase (*Database_World)
#define CharacterDatabase (*Database_Character)

#define DEFAULT_LOG_LEVEL 0
#define DEFAULT_HOST "0.0.0.0"

#ifdef WIN32
	static const char* default_cluster_config_file = "hearthstone-realmserver.conf";
	static const char* default_realm_config_file = "hearthstone-realms.conf";
#else
	static const char* default_cluster_config_file = CONFDIR "/hearthstone-realmserver.conf";
	static const char* default_world_config_file = CONFDIR "/hearthstone-world.conf";
	static const char* default_realm_config_file = CONFDIR "/hearthstone-realms.conf";
#endif

class SERVER_DECL Master : public Singleton<Master>
{
public:
	Master();
	~Master();
	bool Run(int argc, char ** argv);

	static volatile bool m_stopEvent;

private:
	bool _StartDB();
	void _StopDB();

	void _HookSignals();
	void _UnhookSignals();

	static void _OnSignal(int s);
};

#define sMaster Master::getSingleton()

#endif
