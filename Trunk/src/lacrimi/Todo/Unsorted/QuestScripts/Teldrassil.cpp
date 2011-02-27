/*
 * Sun++ Scripts for Sandshroud MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2009-2011 Sandshroud <http://www.sandshroud.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptStdAfx.h"


class Zenn_Foulhoof : public QuestScript
{ 
public:

  void OnQuestComplete(Player *mTarget, QuestLogEntry *qLogEntry)
  {
    if(!mTarget)
		return;

    Creature *creat = mTarget->GetMapMgr()->GetSqlIdCreature(43727);
    if(creat == NULL)
      return;

    creat->SetUInt32Value(UNIT_FIELD_DISPLAYID,901);
	creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Ribbit! No! This cannot...ribbit...be! You have duped me with...ribbit..your foul trickery! Ribbit!");

	sEventMgr.AddEvent(TO_OBJECT(creat), &Object::EventSetUInt32Value, (uint32)UNIT_FIELD_DISPLAYID, (uint32)10035, EVENT_UNK, 50000, 0, 1);
  }
};

void SetupTeldrassil(ScriptMgr *mgr)
{	
  QuestScript *Zenn_FoulhoofQuest = (QuestScript*) new Zenn_Foulhoof();
  mgr->register_quest_script(489, Zenn_FoulhoofQuest);
}