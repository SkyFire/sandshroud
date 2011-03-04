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

#ifndef LACRIMI_STORMWIND_H
#define LACRIMI_STORMWIND_H

class StormwindCityGuard : public GossipScript
{
public:
	void GossipHello(Object *pObject, Player *Plr, bool AutoSend);
	void GossipSelectOption(Object *pObject, Player *Plr, uint32 Id, uint32 IntId, const char * Code);
	void Destroy() { delete this; };
};

#endif // LACRIMI_STORMWIND_H
