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
	While it is up to the */

#ifndef CALL_SCRIPTING_H
#define CALL_SCRIPTING_H

class Unit;

class SERVER_DECL OnKillUnitScript
{
public:
	OnKillUnitScript() {};
	virtual ~OnKillUnitScript() {};

	virtual void OnKillUnit(Unit* unit,  Unit* uVictim) {}
};

class SERVER_DECL OnDeathScript
{
public:
	OnDeathScript() {};
	virtual ~OnDeathScript() {};

	virtual void OnDeath(Unit* unit) {}
};

class SERVER_DECL OnEnterCombatScript
{
public:
	OnEnterCombatScript() {};
	virtual ~OnEnterCombatScript() {};

	virtual void OnEnterCombat(Unit* unit, Unit* uTarget) {}
};

class SERVER_DECL OnCastSpellScript
{
public:
	OnCastSpellScript() {};
	virtual ~OnCastSpellScript() {};

	virtual void OnCastSpell(Unit* unit, SpellEntry * uSpell) {}
};

#endif // CALL_SCRIPTING_H
