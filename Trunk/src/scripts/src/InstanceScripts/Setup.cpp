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

#include "StdAfx.h"
#include "Setup.h"
#include <ScriptSetup.h>

extern "C" SCRIPT_DECL uint32 _exp_get_script_type()
{
	return SCRIPT_TYPE_MISC;
}

extern "C" SCRIPT_DECL void _exp_script_register(ScriptMgr* mgr)	// Comment any script to disable it
{
	#ifdef Classic
	SetupBlackfathomDeeps(mgr);
	SetupBlackrockDepths(mgr);
	SetupBlackrockSpire(mgr);
	SetupDireMaul(mgr);
	//SetupGnomeregan(mgr);
	SetupMaraudon(mgr);
	SetupRagefireChasm(mgr);
	SetupRazorfenDowns(mgr);
	//SetupRazorfenKraul(mgr);
	SetupScarletMonastery(mgr);
	SetupScholomance(mgr);
	SetupShadowfangKeep(mgr);
	//SetupStratholme(mgr);
	//SetupSunkenTemple(mgr);
	SetupDeadmines(mgr);
	SetupTheStockade(mgr);
	SetupUldaman(mgr);
	SetupWailingCaverns(mgr);
	SetupZulFarrak(mgr);
	SetupBlackwingLair(mgr);
	SetupMoltenCore(mgr);
	//SetupRuinsOfAhnQiraj(mgr);
	//SetupTempleOfAhnQiraj(mgr);
	SetupZulGurub(mgr);
	#endif

	#ifdef BurningCrusade
	SetupAuchenaiCrypts(mgr);
	SetupManaTombs(mgr);
	SetupShadowLabyrinth(mgr);
	SetupSethekkHalls(mgr);	
	SetupBlackTemple(mgr);	
	SetupHyjalSummit(mgr);	
	SetupTheBlackMorass(mgr);
	SetupOldHillsbradFoothills(mgr);
	SetupSerpentshrineCavern(mgr);
	SetupTheUnderbog(mgr);
	SetupTheSteamvault(mgr);
	SetupTheSlavePens(mgr);
	SetupGruulsLair(mgr);
	SetupHellfireRamparts(mgr);
	SetupMagtheridonsLair(mgr);
	SetupTheShatteredHalls(mgr);
	SetupTheBloodFurnace(mgr);
	SetupKarazhan(mgr);
	SetupMagistersTerrace(mgr);
	SetupSunwellPlateau(mgr);
	SetupTheMechanar(mgr);
	SetupArcatraz(mgr);
	SetupBotanica(mgr);
	SetupTheEye(mgr);
	SetupZulAman(mgr);
	#endif

	#ifdef LichKing
	//SetupTheOldKingdom(mgr);
	SetupAzjolNerub(mgr);
	//SetupTrialOfTheCrusader(mgr);
	//SetupTrialOfTheChampion(mgr);
	SetupDrakTharonKeep(mgr);
	//SetupGundrak(mgr);
	//SetupHallsOfReflection(mgr);
	//SetupIcecrownCitadel(mgr);
	//SetupTheForgeOfSouls(mgr);
	//SetupPitOfSaron(mgr);
    SetupICC(mgr);
	SetupNaxxramas(mgr);
	SetupOnyxiasLair(mgr);
	SetupCullingOfStratholme(mgr);
	//SetupTheEyeOfEternity(mgr);
	//SetupTheOculus(mgr);
	SetupNexus(mgr);
	//SetupTheVioletHold(mgr);
	SetupHallsOfStone(mgr);
	SetupHallsofLightning(mgr);
	SetupUlduar(mgr);
	//SetupUtgardePinnacle(mgr);
	SetupUtgardeKeep(mgr);
	//SetupTheRubySanctum(mgr);
	//SetupTheObsidianSanctum(mgr);
	#endif

	SetupWorldBosses(mgr);
	//SetupGenericAI(mgr);
}

#ifdef WIN32

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    return TRUE;
}

#endif
