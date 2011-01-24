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

/********************************************
 * NPC
 * Ghamoora
 * ID: 4887
 ********************************************/
class SCRIPT_DECL GhamooraAI : public MoonScriptCreatureAI
{
	typedef MoonScriptCreatureAI ParentClass;
public:
    GhamooraAI(Creature *pCreature);
	static CreatureAIScript * Create(Creature* c) { return new GhamooraAI(c); }
};
/********************************************/

/********************************************
 * NPC
 * Lady Sarevess
 * ID: 4831
 ********************************************/
class SCRIPT_DECL LadySarevessAI : public MoonScriptCreatureAI
{
	typedef MoonScriptCreatureAI ParentClass;
public:
    LadySarevessAI(Creature *pCreature);
	static CreatureAIScript * Create(Creature* c) { return new LadySarevessAI(c); }
};
/********************************************/

/********************************************
 * NPC
 * Gelihast
 * ID: 6243
 ********************************************/
class SCRIPT_DECL GelihastAI : public MoonScriptCreatureAI
{
	typedef MoonScriptCreatureAI ParentClass;
public:
    GelihastAI(Creature *pCreature);
	static CreatureAIScript * Create(Creature* c) { return new GelihastAI(c); }
};
/********************************************/

/********************************************
 * NPC
 * Lorgus Jett
 * ID: 12902
 ********************************************/
class SCRIPT_DECL LorgusJettAI : public MoonScriptCreatureAI
{
	typedef MoonScriptCreatureAI ParentClass;
public:
    LorgusJettAI(Creature *pCreature);
	static CreatureAIScript * Create(Creature* c) { return new LorgusJettAI(c); }
};
/********************************************/

/********************************************
 * NPC
 * Baron Aquanis
 * ID: 12876
 ********************************************/
class SCRIPT_DECL BaronAquanisAI : public MoonScriptCreatureAI
{
	typedef MoonScriptCreatureAI ParentClass;
public:
    BaronAquanisAI(Creature *pCreature);
	static CreatureAIScript * Create(Creature* c) { return new BaronAquanisAI(c); }
};
/********************************************/

/********************************************
 * NPC
 * Twilight Lord Kelris
 * ID: 4832
 ********************************************/
class SCRIPT_DECL TwilightLordKelrisAI : public MoonScriptCreatureAI
{
	typedef MoonScriptCreatureAI ParentClass;
public:
    TwilightLordKelrisAI(Creature *pCreature);
	static CreatureAIScript * Create(Creature* c) { return new TwilightLordKelrisAI(c); }
};
/********************************************/

/********************************************
 * NPC
 * Akumai
 * ID: 4829
 ********************************************/
class SCRIPT_DECL AkumaiAI : public MoonScriptCreatureAI
{
	typedef MoonScriptCreatureAI ParentClass;
public:
    AkumaiAI(Creature *pCreature);
	static CreatureAIScript * Create(Creature* c) { return new AkumaiAI(c); }
};
/********************************************/

/********************************************
 * NPC GOSSIP
 * Morridune
 * ID: 6729
 ********************************************/
class SCRIPT_DECL MorriduneGossip : public GossipScript
{
public:
	void GossipHello(Object *pObject, Player * plr, bool AutoSend);
    void GossipSelectOption(Object *pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code);
    void Destroy();
};
/********************************************/

/********************************************
 * GO
 * Fathom Stone
 * ID: 177964
 ********************************************/
class SCRIPT_DECL FathomStone : public GameObjectAIScript
{
public:
	FathomStone(GameObject * goinstance);
	static GameObjectAIScript *Create(GameObject * GO) { return new FathomStone(GO); }

	void OnActivate(Player * pPlayer);

protected:
	bool BaronAquanis;
};
/********************************************/
