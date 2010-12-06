/*
 * Lacrimi Scripts Copyright 2010 - 2011
 *
 * ############################################################
 * # ##            #       ####### ####### ##    #    #    ## #
 * # ##           ###      ##      ##   ## ##   ###  ###   ## #
 * # ##          ## ##     ##      ##   ## ##   ###  ###   ## #
 * # ##         #######    ##      ####### ##  ## #### ##  ## #
 * # ##        ##     ##   ##      #####   ##  ## #### ##  ## #
 * # ##       ##       ##  ##      ##  ##  ## ##   ##   ## ## #
 * # ####### ##         ## ####### ##   ## ## ##   ##   ## ## #
 * # :::::::.::.........::.:::::::.::...::.::.::...::...::.:: #
 * ############################################################
 *
 */

#include "LacrimiStdAfx.h"

Creature* CreateAndLoadCreature(MapMgr* mgr, uint32 entry, uint32 instancemode, float x, float y, float z, float o, int32 phase, bool push)
{
	if(mgr == NULL)
		return NULL;

	if(entry == 0)
		return NULL;

	if(x == 0.0f && y == 0.0f)
		return NULL;

	CreatureInfo* cn = CreatureNameStorage.LookupEntry(entry);
	if(cn == NULL)
		return NULL;

	CreatureProto* cp = CreatureProtoStorage.LookupEntry(entry);
	if(cp == NULL)
		return NULL;

	if(instancemode > 4)
		instancemode = 0;

	Creature* ctr = mgr->CreateCreature(entry);
	ctr->Load(cp, instancemode, x, y, z, o);
	ctr->SetPhase(phase);
	if(push == true)
		ctr->PushToWorld(mgr);

	return ctr;
}

GameObject* CreateAndLoadGameObject( MapMgr* mgr, uint32 entry, float x, float y, float z, float o, int32 phase, bool push)
{
	if(mgr == NULL)
		return NULL;

	if(entry == 0)
		return NULL;

	if(x == 0.0f && y == 0.0f)
		return NULL;

	GameObject* GO = mgr->CreateGameObject(entry);
	GO->SetInstanceID(mgr->GetInstanceID());
	if(!GO->CreateFromProto(entry, mgr->GetMapId(), x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f))
		return NULL;

	GO->SetRotation(o);
	GO->SetPhase(phase);
	if(push)
		GO->PushToWorld(mgr);

	return GO;
}
