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

#ifndef G3D_MemoryManager_h
#define G3D_MemoryManager_h

#include "platform.h"
#include "ReferenceCount.h"

namespace G3D {

/** 
   Abstraction of memory management.
   Default implementation uses G3D::System::malloc and is threadsafe.

   \sa CRTMemoryManager, AlignedMemoryManager, AreaMemoryManager */
class MemoryManager : public ReferenceCountedObject {
protected:

	MemoryManager();

public:

	typedef ReferenceCountedPointer<class MemoryManager> Ref;

	/** Return a pointer to \a s bytes of memory that are unused by
		the rest of the program.  The contents of the memory are
		undefined */
	virtual void* alloc(size_t s);

	/** Invoke to declare that this memory will no longer be used by
		the program.  The memory manager is not required to actually
		reuse or release this memory. */
	virtual void free(void* ptr);

	/** Returns true if this memory manager is threadsafe (i.e., alloc
		and free can be called asychronously) */
	virtual bool isThreadsafe() const;

	/** Return the instance. There's only one instance of the default
		MemoryManager; it is cached after the first creation. */
	static MemoryManager::Ref create();
};

/** 
   Allocates memory on 16-byte boundaries.
   \sa MemoryManager, CRTMemoryManager, AreaMemoryManager */
class AlignedMemoryManager : public MemoryManager {
protected:

	AlignedMemoryManager();

public:

	typedef ReferenceCountedPointer<class AlignedMemoryManager> Ref;

	
	virtual void* alloc(size_t s);

	virtual void free(void* ptr);

	virtual bool isThreadsafe() const;

	static AlignedMemoryManager::Ref create();
};


/** MemoryManager implemented using the C runtime. */
class CRTMemoryManager : public MemoryManager {
protected:
	CRTMemoryManager();

public:
	typedef ReferenceCountedPointer<class MemoryManager> Ref;
	virtual void* alloc(size_t s);
	virtual void free(void* ptr);
	virtual bool isThreadsafe() const;

	/** There's only one instance of this memory manager; it is 
		cached after the first creation. */
	static CRTMemoryManager::Ref create();
};

}

#endif
