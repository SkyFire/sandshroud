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

#ifdef WIN32
static HANDLE m_abortLacrimi = INVALID_HANDLE_VALUE;
#else
static pthread_cond_t abortcondLacrimi;
static pthread_mutex_t abortmutex;
#endif

Lacrimi::Lacrimi(ScriptMgr* mgr)
{
	first = true;
	config = false;
	LacrimiDB = NULL;
	database = false;
	dumpstats = false;
	m_threadRunning = true;
	sMgr = mgr;
}

Lacrimi::~Lacrimi()
{
#ifdef WIN32
	CloseHandle(m_abortLacrimi);
#else
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
#endif
}

void Lacrimi::terminate()
{
	m_threadRunning = false;
#ifdef WIN32
	SetEvent(m_abortLacrimi);
#else
	pthread_cond_signal(&abortcondLacrimi);
#endif
}

void SetThreadName(const char* format, ...)
{
	// This isn't supported on nix?
	va_list ap;
	va_start(ap, format);

#ifdef WIN32
	char thread_name[200];
	vsnprintf(thread_name, 200, format, ap);

	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.dwThreadID = GetCurrentThreadId();
	info.dwFlags = 0;
	info.szName = thread_name;
	DEBUG_LOG("ThreadMaker", "Creating thread %u with name %s", info.dwThreadID, info.szName);
	ThreadPool.SetThreadInfo(info.dwThreadID, info);

	__try
	{
#ifdef _WIN64
		RaiseException(0x406D1388, 0, sizeof(info)/sizeof(DWORD), (ULONG_PTR*)&info);
#else
		RaiseException(0x406D1388, 0, sizeof(info)/sizeof(DWORD), (DWORD*)&info);
#endif
	}
	__except(EXCEPTION_CONTINUE_EXECUTION)
	{

	}
#endif

	va_end(ap);
}

void Lacrimi::Delay(uint32 time)
{
#ifdef WIN32
	WaitForSingleObject(m_abortLacrimi, time);
#else
	Sleep(time);
#endif
}

bool Lacrimi::run()
{
	SetThreadName("Lacrimi");
	Log.Success("Lacrimi", "Lacrimi Engine Started");

	// Load our configs
#ifdef WIN32
	if(LacrimiConfig.SetSource("configs/Lacrimi.conf", true))
#else
	if(LacrimiConfig.SetSource((char*)CONFDIR "/Lacrimi.conf", true))
#endif
	{
		config = true;
		Log.Success("Lacrimi", "Config successfully set");
	}

	// Load our DBs
	if(_StartDB())
		database = true;

	dumpstats = GetConfigBool("StatDumper", "DumpStats", false);
	if(dumpstats)
	{
		Log.Success("Lacrimi", "Lacrimi Stat Dumper Initialized");
		strcpy(Filename, GetConfigString("StatDumper", "Filename", "stats.xml"));
	}

	SetupScripts();
	while(m_threadRunning)
	{
		if(dumpstats)
			DumpStats();

		Delay(15000);
		if(!m_threadRunning)
			break;
		Delay(15000);
		if(!m_threadRunning)
			break;
		Delay(15000);
		if(!m_threadRunning)
			break;
		Delay(15000);
		if(!m_threadRunning)
			break;

		Log.Debug("Lacrimi", "Running cleanup...");
	}

	if(database)
		_StopDB();
	return true;
}

bool Lacrimi::_StartDB()
{
	if(!config)
		return false;

	string hostname, username, password, database;
	int port = 0;
	// Configure Main Database
	bool result = LacrimiConfig.GetString( "LacrimiDatabase", "Username", &username );
	LacrimiConfig.GetString( "LacrimiDatabase", "Password", &password );
	result = !result ? result : LacrimiConfig.GetString( "LacrimiDatabase", "Hostname", &hostname );
	result = !result ? result : LacrimiConfig.GetString( "LacrimiDatabase", "Name", &database );
	result = !result ? result : LacrimiConfig.GetInt( "LacrimiDatabase", "Port", &port );
	if(result == false)
	{
		OUT_DEBUG( "sql: One or more parameters were missing from LacrimiDatabase directive." );
		return false;
	}

	if(username == "username" || password == "passwd" || hostname == "host")
	{
		OUT_DEBUG( "sql: One or more parameters were missing from LacrimiDatabase directive." );
		return false;
	}

	// Create the DB
	LacrimiDB = Database::Create();

	// Initialize it
	if( !LacrimiDB->Initialize(hostname.c_str(), uint(port), username.c_str(),
		password.c_str(), database.c_str(), LacrimiConfig.GetIntDefault( "LacrimiDatabase", "ConnectionCount", 3 ), 16384 ) )
	{
		OUT_DEBUG( "sql: Main database initialization failed. Exiting." );
		_StopDB(); // Kekeke
		return false;
	}

	return true;
}

void Lacrimi::_StopDB()
{
	delete LacrimiDB;
	LacrimiDB = NULL;
}

char* Lacrimi::GetConfigString(char* configfamily, char* configoption, char* cdefault)
{
	char* creturn = cdefault;
	if(config) // Crow: Lets just use MAX_PATH for this, it's a reasonable number...
		if( !LacrimiConfig.GetString( configfamily, Filename, configoption, cdefault, MAX_PATH ) )
			creturn = cdefault;
	return creturn;
}

float Lacrimi::GetConfigfloat(char* configfamily, char* configoption, float fdefault)
{
	float freturn = fdefault;
	if(config)
		if(!LacrimiConfig.GetFloat(configfamily, configoption, &freturn))
			freturn = fdefault;
	return freturn;
}

bool Lacrimi::GetConfigBool(char* configfamily, char* configoption, bool bdefault)
{
	bool breturn = bdefault;
	if(config)
		if(!LacrimiConfig.GetBool(configfamily, configoption, &breturn))
			breturn = bdefault;
	return breturn;
}

int Lacrimi::GetConfigInt(char* configfamily, char* configoption, int intdefault)
{
	int ireturn = intdefault;
	if(config)
		if(!LacrimiConfig.GetInt(configfamily, configoption, &ireturn))
			ireturn = intdefault;
	return ireturn;
}

// Use sMgr for Script Mgr.
void Lacrimi::SetupScripts()
{
	SetupZoneScripts();
	SetupCityScripts();
}

void Lacrimi::SetupZoneScripts()
{
	if(GetConfigBool("ZoneScripts", "EnableOutlandScripts", true))
		SetupOutlandScripts();
	if(GetConfigBool("ZoneScripts", "EnableKalimdorScripts", true))
		SetupKalimdorScripts();
	if(GetConfigBool("ZoneScripts", "EnableEbonHoldScripts", true))
		SetupEbonHoldScripts();
	if(GetConfigBool("ZoneScripts", "EnableNorthrendScripts", true))
		SetupNorthrendScripts();
	if(GetConfigBool("ZoneScripts", "EnableEasternKingdomScripts", true))
		SetupEasternKingdomScripts();
}

void Lacrimi::SetupEasternKingdomScripts()
{
	SetupAlteracMountains();
	SetupArathiHighlands();
	SetupBlastedLands();
	SetupBurningSteppes();
	SetupDunMorogh();
	SetupDuskwood();
	SetupEasternPlaguelands();
	SetupElwynnForest();
	SetupEversongWoods();
	SetupGhostlands();
	SetupHinterlands();
	SetupIsleOfQuelDanas();
	SetupLochModan();
	SetupSearingGorge();
	SetupSilverpineForest();
	SetupStranglethornVale();
	SetupTrisfalGlades();
	SetupWesternPlaguelands();
	SetupWestfall();
	SetupWetlands();
}

void Lacrimi::SetupKalimdorScripts()
{
	SetupAshenvale();
	SetupAzshara();
	SetupAzuremystIsle();
	SetupBloodmystIsle();
	SetupDarkshore();
	SetupDesolace();
	SetupDustwallowMarsh();
	SetupFelwood();
	SetupFeralas();
	SetupMoonglade();
	SetupMulgore();
	SetupSilithus();
	SetupStonetalonMountains();
	SetupTanaris();
	SetupTeldrassil();
	SetupTheBarrens();
	SetupThousandNeedles();
	SetupUngoroCrater();
	SetupWinterspring();
}

void Lacrimi::SetupOutlandScripts()
{
	SetupBladesEdgeMountains();
	SetupHellfirePeninsula();
	SetupNagrand();
	SetupNetherstorm();
	SetupShadowmoonValley();
	SetupTerrokarForest();
	SetupZangarmarsh();
}

void Lacrimi::SetupNorthrendScripts()
{
	SetupBoreanTundra();
	SetupDragonblight();
	SetupGrizzlyHills();
	SetupHowlingFjord();
	SetupIcecrown();
	SetupSholazarBasin();
	SetupStormPeaks();
	SetupZulDrak();
}

void Lacrimi::SetupCityScripts()
{
	SetupIronforge();
	SetupSilvermoon();
	SetupStormwind();
	SetupUndercity();
	SetupDarnassus();
	SetupExodar();
	SetupOrgrimmar();
	SetupThunderbluff();
	SetupShattrath();
	SetupDalaran();
}

void Lacrimi::GenerateUptimeString(char * Dest)
{
	if(first)
	{
		sprintf(Dest, "0 days, 0 hours, 0 minutes, 0 seconds");
		first = false;
		return;
	}

	uint32 seconds = sWorld.GetUptime();
	uint32 mins = 0;
	uint32 hours = 0;
	uint32 days = 0;
	if(seconds >= 60)
	{
		mins = seconds / 60;
		if(mins)
		{
			seconds -= mins*60;
			if(mins >= 60)
			{
				hours = mins / 60;
				if(hours)
				{
					mins -= hours*60;
					if(hours >= 24)
					{
						days = hours/24;
						if(days)
							hours -= days*24;
					}
				}
			}
		}
	}

	sprintf(Dest, "%d days, %d hours, %d minutes, %d seconds", (int)days, (int)hours, (int)mins, (int)seconds);
}

void Lacrimi::FillOnlineTime(uint32 Time, char * Dest)
{
	uint32 seconds = Time;
	uint32 mins=0;
	uint32 hours=0;
	uint32 days=0;
	if(seconds >= 60)
	{
		mins = seconds / 60;
		if(mins)
		{
			seconds -= mins*60;
			if(mins >= 60)
			{
				hours = mins / 60;
				if(hours)
				{
					mins -= hours*60;
					if(hours >= 24)
					{
						days = hours/24;
						if(days)
							hours -= days*24;
					}
				}
			}
		}
	}
	sprintf(Dest, "%d hours, %d minutes, %d seconds", (int)hours, (int)mins, (int)seconds);
}

void Lacrimi::DumpStats()
{
	if( Filename[0] == '\0' )
		return;
	FILE* f = fopen( Filename, "w" );
	if( !f )
		return;

	Log.Notice("Lacrimi", "Dumping stats...");

	// Dump Header
	fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(f, "<?xml-stylesheet type=\"text/xsl\" href=\"server_stats.xsl\"?>\n");
	fprintf(f, "<serverpage>\n");
	fprintf(f, "  <status>\n");

	uint32 races[RACE_TROLL+1];
	uint32 classes[DRUID+1];
	memset(&races[0], 0, sizeof(uint32)*(RACE_TROLL+1));
	memset(&classes[0], 0, sizeof(uint32)*(DRUID+1));
	std::deque<Player*> gms;
	{
		// Dump server information.
#ifdef WIN32
		fprintf(f, "	<platform>Sandshroud Hearthstone r%u %s-Win-%s</platform>\n", BUILD_REVISION, CONFIG, ARCH);		
#else
		fprintf(f, "	<platform>Sandshroud Hearthstone r%u %s-%s </platform>\n", BUILD_REVISION, PLATFORM_TEXT, ARCH);
#endif

		char uptime[80];
		GenerateUptimeString(uptime);
		time_t t = (time_t)UNIXTIME;
		float AvgLat;
		uint32 GMCount;
		int gm = 0;
		int count = 0;
		int avg = 0;

		// lock players reader
		objmgr._playerslock.AcquireReadLock();

		HM_NAMESPACE::hash_map<uint32, Player*>::const_iterator itr;
		for (itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
		{
			if(itr->second->GetSession() && itr->second->IsInWorld())
			{
				count++;
				avg += itr->second->GetSession()->GetLatency();
				if(itr->second->GetSession()->GetPermissionCount())
				{
					gm++;
					gms.push_back(itr->second);
				}
				classes[itr->second->getClass()]++;
				races[itr->second->getRace()]++;
			}			
		}
		objmgr._playerslock.ReleaseReadLock();

		AvgLat = count ? (float)((float)avg / (float)count) : 0;
		GMCount = gm;

		fprintf(f, "    <uptime>%s</uptime>\n", uptime);
		fprintf(f, "    <oplayers>%u</oplayers>\n", (unsigned int)sWorld.GetSessionCount());
		fprintf(f, "    <cpu>%2.2f</cpu>\n", sWorld.GetCPUUsage(true));
		fprintf(f, "    <qplayers>%u</qplayers>\n", (unsigned int)sWorld.GetQueueCount());
		fprintf(f, "    <ram>%.3f</ram>\n", sWorld.GetRAMUsage(true));
		fprintf(f, "    <avglat>%.3f</avglat>\n", AvgLat);
		fprintf(f, "    <threads>%u</threads>\n", (unsigned int)ThreadPool.GetActiveThreadCount());
		fprintf(f, "    <gmcount>%u</gmcount>\n", (unsigned int)GMCount);
		fprintf(f, "    <lastupdate>%s</lastupdate>\n", asctime(localtime(&t)));
		fprintf(f, "    <alliance>%u</alliance>\n", (unsigned int)sWorld.AlliancePlayers);
		fprintf(f, "    <horde>%u</horde>\n", (unsigned int)sWorld.HordePlayers);
		fprintf(f, "    <acceptedconns>%u</acceptedconns>\n", (unsigned int)sWorld.mAcceptedConnections);
		fprintf(f, "    <peakcount>%u</peakcount>\n", (unsigned int)sWorld.PeakSessionCount);
		fprintf(f, "    <wdbquerysize>%u</wdbquerysize>\n", WorldDatabase.GetQueueSize());
		fprintf(f, "    <cdbquerysize>%u</cdbquerysize>\n", CharacterDatabase.GetQueueSize());
	}

	fprintf(f, "  </status>\n");
	static const char * race_names[RACE_TROLL+1] = {
		NULL,
		"human",
		"orc",
		"dwarf",
		"nightelf",
		"undead",
		"tauren",
		"gnome",
		"troll",
	};

	static const char * class_names[DRUID+1] = {
		NULL,
		"warrior",
		"paladin",
		"hunter",
		"rogue",
		"priest",
		"deathknight",
		"shaman",
		"mage",
		"warlock",
		NULL,
		"druid",
	};
	
	fprintf(f, "  <statsummary>\n");
	uint32 i;
	for(i = 0; i <= RACE_TROLL; ++i)
	{
		if( race_names[i] != NULL )
			fprintf(f, "    <%s>%u</%s>\n", race_names[i], races[i], race_names[i]);
	}

	for(i = 0; i <= DRUID; ++i)
	{
		if( class_names[i] != NULL )
			fprintf(f, "    <%s>%u</%s>\n", class_names[i], classes[i], class_names[i]);
	}
	fprintf(f, "  </statsummary>\n");

	Player*  plr;
	uint32 t = (uint32)time(NULL);

	char otime[100];
	{
		fprintf(f, "  <instances>\n");
		
		// need a big buffer..
		static char buf[500000];
		memset(buf, 0, 500000);

		// Dump Instance Information
		//sWorldCreator.BuildXMLStats(buf);
		sInstanceMgr.BuildXMLStats(buf);
		fprintf(f, buf);
		fprintf(f, "  </instances>\n");
	}

	{
		// GM Information
		fprintf(f, "  <gms>\n");
		while(!gms.empty())
		{
			plr = gms.front();
			gms.pop_front();

			if(plr->bGMTagOn)
			{
				FillOnlineTime(t - plr->OnlineTime, otime);
				fprintf(f, "    <gmplr>\n");
				fprintf(f, "      <name>%s%s</name>\n", (plr->bGMTagOn ? "{GM}" : ""), plr->GetName());
				fprintf(f, "      <race>%u</race>\n", plr->getRace());
				fprintf(f, "      <class>%u</class>\n", (unsigned int)plr->getClass());
				fprintf(f, "      <gender>%u</gender>\n", (unsigned int)plr->getGender());
				fprintf(f, "      <pvprank>%u</pvprank>\n", (unsigned int)plr->GetPVPRank());
				fprintf(f, "      <level>%u</level>\n", (unsigned int)plr->GetUInt32Value(UNIT_FIELD_LEVEL));
				fprintf(f, "      <map>%u</map>\n", (unsigned int)plr->GetMapId());
				fprintf(f, "      <areaid>%u</areaid>\n", (unsigned int)plr->GetAreaID());
				fprintf(f, "      <ontime>%s</ontime>\n", otime);
				fprintf(f, "      <latency>%u</latency>\n", (unsigned int)plr->GetSession()->GetLatency());
				fprintf(f, "      <permissions>%s</permissions>\n", plr->GetSession()->GetPermissions());
				fprintf(f, "    </gmplr>\n");
			}
		}

		fprintf(f, "  </gms>\n");
	}

	{
	fprintf(f, "  <sessions>\n");
		// Dump Player Information
		objmgr._playerslock.AcquireReadLock();
		HM_NAMESPACE::hash_map<uint32, Player*>::const_iterator itr;

		for (itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
		{
			plr = itr->second;
			if(itr->second->GetSession() && itr->second->IsInWorld())
			{
				FillOnlineTime(t - plr->OnlineTime, otime);

				fprintf(f, "    <plr>\n");
				fprintf(f, "      <name>%s</name>\n", plr->GetName());
				fprintf(f, "      <race>%u</race>\n", (unsigned int)plr->getRace());
				fprintf(f, "      <class>%u</class>\n", (unsigned int)plr->getClass());
				fprintf(f, "      <gender>%u</gender>\n", (unsigned int)plr->getGender());
				fprintf(f, "      <pvprank>%u</pvprank>\n", (unsigned int)plr->GetPVPRank());
				fprintf(f, "      <level>%u</level>\n", (unsigned int)plr->GetUInt32Value(UNIT_FIELD_LEVEL));
				fprintf(f, "      <map>%u</map>\n", (unsigned int)plr->GetMapId());
				fprintf(f, "      <areaid>%u</areaid>\n", (unsigned int)plr->GetAreaID());
				//requested by Zdarkside for he's online map. I hope it does not screw up any parser. If so, then make a better one :P
				fprintf(f, "      <xpos>%f</xpos>\n", plr->GetPositionX ());
				fprintf(f, "      <ypos>%f</ypos>\n", plr->GetPositionY());
				fprintf(f, "      <ontime>%s</ontime>\n", otime);
				fprintf(f, "      <latency>%u</latency>\n", (unsigned int)plr->GetSession()->GetLatency());
				fprintf(f, "    </plr>\n");
				if(plr->GetSession()->GetPermissionCount() > 0)
					gms.push_back(plr);
			}
		}

		objmgr._playerslock.ReleaseReadLock();
		fprintf(f, "  </sessions>\n");

		
	}

	fprintf(f, "</serverpage>\n");
	fclose(f);
}
