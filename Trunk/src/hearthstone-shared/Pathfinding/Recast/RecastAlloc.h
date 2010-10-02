/*
 * Sandshroud Hearthstone
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

#ifndef RECASTALLOC_H
#define RECASTALLOC_H

enum rcAllocHint
{
	RC_ALLOC_PERM,		// Memory persist after a function call.
	RC_ALLOC_TEMP		// Memory used temporarily within a function.
};

typedef void* (rcAllocFunc)(int size, rcAllocHint hint);
typedef void (rcFreeFunc)(void* ptr);

void rcAllocSetCustom(rcAllocFunc *allocFunc, rcFreeFunc *freeFunc);

void* rcAlloc(int size, rcAllocHint hint);
void rcFree(void* ptr);



// Simple dynamic array ints.
class rcIntArray
{
	int* m_data;
	int m_size, m_cap;
	inline rcIntArray(const rcIntArray&);
	inline rcIntArray& operator=(const rcIntArray&);
public:
	inline rcIntArray() : m_data(0), m_size(0), m_cap(0) {}
	inline rcIntArray(int n) : m_data(0), m_size(0), m_cap(0) { resize2(n); }
	inline ~rcIntArray() { rcFree(m_data); }
	void resize2(int n);
	inline void push(int item) { resize2(m_size+1); m_data[m_size-1] = item; }
	inline int pop() { if (m_size > 0) m_size--; return m_data[m_size]; }
	inline const int& operator[](int i) const { return m_data[i]; }
	inline int& operator[](int i) { return m_data[i]; }
	inline int size() const { return m_size; }
};

// Simple internal helper class to delete array in scope
template<class T> class rcScopedDelete
{
	T* ptr;
	inline T* operator=(T* p);
public:
	inline rcScopedDelete() : ptr(0) {}
	inline rcScopedDelete(T* p) : ptr(p) {}
	inline ~rcScopedDelete() { rcFree(ptr); }
	inline operator T*() { return ptr; }
};

#endif
