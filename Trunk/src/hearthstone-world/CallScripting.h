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

/*	Crow: This style of scripting can be used with C++ scripts to add temporary hooks to what the player/creature is doing.
	While it is up to the creater of the script to clean these up, the core will automatically delete it when a player changes maps. */

#ifndef CALL_SCRIPTING_H
#define CALL_SCRIPTING_H

class Unit;

class SERVER_DECL OnKillUnitScript
{
public:
	OnKillUnitScript() { links = 0; };
	virtual ~OnKillUnitScript() {};

	virtual void OnKillUnit(Unit* unit,  Unit* uVictim) {}
	uint64 links;
};

class SERVER_DECL OnDeathScript
{
public:
	OnDeathScript() {};
	virtual ~OnDeathScript() { links = 0; };

	virtual void OnDeath(Unit* unit) {}
	uint64 links;
};

class SERVER_DECL OnEnterCombatScript
{
public:
	OnEnterCombatScript() { links = 0; };
	virtual ~OnEnterCombatScript() {};

	virtual void OnEnterCombat(Unit* unit, Unit* uTarget) {}
	uint64 links;
};

class SERVER_DECL OnCastSpellScript
{
public:
	OnCastSpellScript() { links = 0; };
	virtual ~OnCastSpellScript() {};

	virtual void OnCastSpell(Unit* unit, SpellEntry * uSpell) {}
	uint64 links;
};

#endif // CALL_SCRIPTING_H
