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

#ifndef LACRIMI_H
#define LACRIMI_H

Database* LacrimiDB;
#define LacrimiDatabase (*LacrimiDB)

class SCRIPT_DECL Lacrimi : public ThreadContext
{
private:
	ScriptMgr* sMgr;

public:
	Lacrimi(ScriptMgr* mgr);
	~Lacrimi();

	bool run();
	void terminate();
	void Delay(uint32 time);

public: // Script Related
	void SetupScripts();
};

#endif