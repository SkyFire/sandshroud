/*
 * Sandshroud Hearthstone
 * Copyright (C) 2004 - 2005 Antrix Team
 * Copyright (C) 2005 - 2007 Ascent Team <http://www.ascentemu.com/>
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

#ifndef RWLOCK_H
#define RWLOCK_H

#include "Condition.h"
#include "Mutex.h"

class RWLock
{
public: 
  
	HEARTHSTONE_INLINE void AcquireReadLock()
	{
		//_lock.Acquire();
		_cond.BeginSynchronized();
		_readers++;
		//_lock.Release();
		_cond.EndSynchronized();
	}
	
	HEARTHSTONE_INLINE void ReleaseReadLock()
	{
		//_lock.Acquire();
		_cond.BeginSynchronized();
		if(!(--_readers))
			if(_writers)
				_cond.Signal();
		//_lock.Release();
		_cond.EndSynchronized();
	}

	HEARTHSTONE_INLINE void AcquireWriteLock()
	{
		//_lock.Acquire();
		_cond.BeginSynchronized();
		_writers++;
		if(_readers)
			_cond.Wait();
	}

	HEARTHSTONE_INLINE void ReleaseWriteLock()
	{
		if(--_writers)
			_cond.Signal();
		//_lock.Release();
		_cond.EndSynchronized();
	}
	HEARTHSTONE_INLINE RWLock() : _cond(&_lock) {_readers=0;_writers=0;}
  
	private:
		Mutex _lock;
		Condition _cond;
		volatile unsigned int _readers;
		volatile unsigned int _writers;
   
}; 

#endif