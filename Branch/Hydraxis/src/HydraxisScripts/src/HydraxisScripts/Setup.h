/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#ifndef HYDRAXIS_SETUP_H
#define HYDRAXIS_SETUP_H

#include "StdAfx.h"

/* A hook defined to ALL_MAPS will be called regardless of the Map ID */
#define ALL_MAPS 0xFFFFFFFF

void SetupMainTeleporter(ScriptMgr * mgr);
void SetupWarmage(ScriptMgr * mgr);
void SetupSkillTrainer(ScriptMgr * mgr);
void SetupPvPSystem(ScriptMgr * mgr);
void SetupInnkeepers(ScriptMgr * mgr);
void SetupGuardGossip(ScriptMgr * mgr);
void SetupHunterTrainer(ScriptMgr * mgr);

#endif
