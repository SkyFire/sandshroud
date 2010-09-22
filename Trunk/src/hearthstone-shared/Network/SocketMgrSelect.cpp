/*
 * Sandshroud Hearthstone
 * Copyright (C) 2005 - 2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007 - 2008 Antrix Team
 * Copyright (C) 2008 - 2009 AspireDev <http://www.aspiredev.org/>
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

#include "SharedStdAfx.h"
#include "Network.h"
#ifdef CONFIG_USE_SELECT

initialiseSingleton(SocketMgr);

SocketMgr::SocketMgr()
{
#ifdef WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,0), &wsaData);
#endif
	FD_ZERO(&m_allSet);
	FD_ZERO(&m_readableSet);
	FD_ZERO(&m_writableSet);
	FD_ZERO(&m_exceptionSet);
	socket_count = 0;
}

SocketMgr::~SocketMgr()
{

}

void SocketMgr::AddSocket(Socket * s)
{
	if(socket_count >= FD_SETSIZE || fds.find(s->GetFd()) != fds.end())
	{
		s->Delete();
		return;
	}

	/* add it to the big set */
	m_setLock.Acquire();
	FD_SET(s->GetFd(), &m_allSet);
	fds[s->GetFd()] = s;
	socket_count++;
	m_setLock.Release();
}

void SocketMgr::RemoveSocket(Socket * s)
{
	map<int, Socket*>::iterator itr = fds.find(s->GetFd());
	if(itr == fds.end())
		return;

	m_setLock.Acquire();
	FD_CLR(s->GetFd(), &m_allSet);
	fds.erase(itr);
	socket_count--;
	m_setLock.Release();
}

void SocketMgr::WantWrite(int fd)
{
	m_setLock.Acquire();
	FD_SET(fd, &m_writableSet);
	m_setLock.Release();
}

void SocketMgr::thread_function()
{
	int r_size = sizeof(m_allSet);
	int nfds;
	timeval timeout;
	timeout.tv_usec = 20000;
	timeout.tv_sec = 0;
	map<int, Socket*>::iterator itr;
	FD_SET writable;
	Socket * s;

	for(;;)
	{
		/* socket loop, whee */
		m_setLock.Acquire();
		
		if(!socket_count)
		{
			m_setLock.Release();
			Sleep(50);
			continue;
		}
		/* copy the all set into the readable set */
		memcpy(&m_readableSet, &m_allSet, r_size);
		memcpy(&writable, &m_writableSet, r_size);

		/* clear the writable set for the next loop */
		FD_ZERO(&m_writableSet);

		/* call select(), w000t */
		m_setLock.Acquire();
		nfds = select(FD_SETSIZE, &m_readableSet, &writable, &m_exceptionSet, &timeout);
		m_setLock.Release();
		if(nfds < 0)
		{
			printf("Exception in select() somewhere.. :/ %u\n", WSAGetLastError());
		}
		else if(nfds > 0)
		{
			/* burlex: oh my god, this sucks cock. :P this is the reason they invented
			 * poll() :P
			 */
			for(itr = fds.begin(); itr != fds.end(); ++itr)
			{
				s = itr->second;

				if(FD_ISSET(s->GetFd(), &m_readableSet))
					s->ReadCallback(0);

				if(FD_ISSET(s->GetFd(), &writable))
				{
					s->BurstBegin();		// <<<<
					s->WriteCallback();
					if(s->GetWriteBufferSize() > 0)
						FD_SET(s->GetFd(), &m_writableSet);		// Still remaining data
					else
						s->DecSendLock();

					s->BurstEnd();
				}

				if(FD_ISSET(itr->second->GetFd(), &m_exceptionSet))
					s->Disconnect();
			}
		}

		/* clear the exception set for the next loop */
		FD_ZERO(&m_exceptionSet);
		m_setLock.Release();
	}
}

void SocketWorkerThread::run()
{
	sSocketMgr.thread_function();
}

void SocketMgr::CloseAll()
{

}

void SocketMgr::SpawnWorkerThreads()
{
	int tc = 1;
	for(int i = 0; i < tc; ++i)
		launch_thread(new SocketWorkerThread());
}

void SocketMgr::ShutdownThreads()
{

}

#endif
