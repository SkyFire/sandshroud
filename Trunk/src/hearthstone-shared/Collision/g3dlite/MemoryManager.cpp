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

#include "MemoryManager.h"
#include "System.h"

namespace G3D {

MemoryManager::MemoryManager() {}


void* MemoryManager::alloc(size_t s) {
	return System::malloc(s);
}


void MemoryManager::free(void* ptr) {
	System::free(ptr);
}


bool MemoryManager::isThreadsafe() const {
	return true;
}


MemoryManager::Ref MemoryManager::create() {
	static MemoryManager::Ref m = new MemoryManager();
	return m;
}


///////////////////////////////////////////////////

AlignedMemoryManager::AlignedMemoryManager() {}


void* AlignedMemoryManager::alloc(size_t s) {
	return System::alignedMalloc(s, 16);
}


void AlignedMemoryManager::free(void* ptr) {
	System::alignedFree(ptr);
}


bool AlignedMemoryManager::isThreadsafe() const {
	return true;
}


AlignedMemoryManager::Ref AlignedMemoryManager::create() {
	static AlignedMemoryManager::Ref m = new AlignedMemoryManager();
	return m;
}


///////////////////////////////////////////////////

CRTMemoryManager::CRTMemoryManager() {}


void* CRTMemoryManager::alloc(size_t s) {
	return ::malloc(s);
}


void CRTMemoryManager::free(void* ptr) {
	return ::free(ptr);
}


bool CRTMemoryManager::isThreadsafe() const {
	return true;
}


CRTMemoryManager::Ref CRTMemoryManager::create() {
	static CRTMemoryManager::Ref m = new CRTMemoryManager();
	return m;
}
}
