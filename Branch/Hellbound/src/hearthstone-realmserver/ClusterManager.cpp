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

#include "RStdAfx.h"

initialiseSingleton(ClusterMgr);

ClusterMgr::ClusterMgr()
{
	MasterControlServer = NULL;
	m_maxInstanceId = 0;
	m_maxWorkerServer = 0;
	Log.Success("ClusterMgr", "Interface Created");

	MasterServer::InitHandlers();
}

MasterServer* ClusterMgr::CreateMasterServer(MasterServerSocket * s)
{
	if(MasterControlServer != NULL)
		return NULL;

	uint32 port = s->GetRemotePort();
	Log.Notice("ClusterMgr", "Allocating Master Server to %s:%u", s->GetRemoteIP().c_str(), s->GetRemotePort());
	MasterControlServer = new MasterServer(s, port);
	return MasterControlServer;
}

void ClusterMgr::OnMasterServerDisconnect()
{
	delete MasterControlServer;
	MasterControlServer = NULL;
}

void ClusterMgr::Update()
{
	if(MasterControlServer != NULL)
		MasterControlServer->Update();
}

void ClusterMgr::SendPacketToMasterControl(WorldPacket * data)
{
	if(MasterControlServer != NULL)
		MasterControlServer->SendPacket(data);
}
