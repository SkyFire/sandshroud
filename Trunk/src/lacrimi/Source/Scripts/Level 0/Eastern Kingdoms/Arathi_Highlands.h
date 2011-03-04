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

#ifndef LACRIMI_ARATHI_HIGHLANDS_H
#define LACRIMI_ARATHI_HIGHLANDS_H

class SunkenTreasure : public QuestScript 
{ 
public:
	void OnQuestStart( Player *mTarget, QuestLogEntry * qLogEntry);
};

class Professor_Phizzlethorpe : public CreatureAIScript
{
public:
	static CreatureAIScript* Create(Creature* c) { return new Professor_Phizzlethorpe(c); }
	Professor_Phizzlethorpe(Creature *pCreature) : CreatureAIScript(pCreature) {}

	void OnReachWP(uint32 iWaypointId, bool bForwards);
};

#endif // LACRIMI_ARATHI_HIGHLANDS_H
