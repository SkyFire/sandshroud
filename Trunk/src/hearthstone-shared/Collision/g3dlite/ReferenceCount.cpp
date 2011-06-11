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

#include "platform.h"
#include "ReferenceCount.h"

namespace G3D {

ReferenceCountedObject::ReferenceCountedObject() : 
	ReferenceCountedObject_refCount(0), 
	ReferenceCountedObject_weakPointer(0) {

	ASSERT(isValidHeapPointer(this));
}

void ReferenceCountedObject::ReferenceCountedObject_zeroWeakPointers() {
	// Tell all of my weak pointers that I'm gone.
	
	_WeakPtrLinkedList* node = ReferenceCountedObject_weakPointer;

	while (node != NULL) {
		// Notify the weak pointer that it is going away
		node->weakPtr->objectCollected();

		// Free the node and advance
		_WeakPtrLinkedList* tmp = node;
		node = node->next;
		delete tmp;
	}
}

ReferenceCountedObject::~ReferenceCountedObject() {}


ReferenceCountedObject::ReferenceCountedObject(const ReferenceCountedObject& notUsed) : 
	ReferenceCountedObject_refCount(0),
	ReferenceCountedObject_weakPointer(0) {
	(void)notUsed;
	ASSERT(isValidHeapPointer(this));
}

ReferenceCountedObject& ReferenceCountedObject::operator=(const ReferenceCountedObject& other) {
	(void)other;
	// Nothing changes when I am assigned; the reference count on
	// both objects is the same (although my super-class probably
	// changes).
	return *this;
}

} // G3D
