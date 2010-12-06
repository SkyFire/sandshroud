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

#ifndef FUNCTIONS_H

SCRIPT_DECL Creature* CreateAndLoadCreature(
	MapMgr* mgr, uint32 entry, uint32 instancemode, float x, float y, float z, float o, int32 phase = 1, bool push = true);

SCRIPT_DECL GameObject* CreateAndLoadGameObject(MapMgr* mgr, uint32 entry, float x, float y, float z, float o,
												int32 phase = 1, bool push = true);

#endif // FUNCTIONS_H
