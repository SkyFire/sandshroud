INCLUDES = -I$(top_srcdir)/src/hearthstone-logonserver \
   -I$(top_srcdir)/src/hearthstone-realmserver -I$(top_srcdir)/src/hearthstone-shared -I$(top_srcdir)/src/hearthstone-voicechat \
   -I$(top_srcdir)/src/hearthstone-world -I$(srcdir) -I$(srcdir)/../../../../dep/include \
   -I$(srcdir)/../../../../dep/src -I$(srcdir)/../../../../src
INCLUDES += -I$(srcdir)/../../../hearthstone-shared/g3dlite -I$(srcdir)/../../../hearthstone-shared/vmap

AM_CXXFLAGS = -DSCRIPTLIB

lib_LTLIBRARIES = libOtherScripts.la

libSunMiscScripts_la_SOURCES = Miscellaneous/Creatures.cpp \
Miscellaneous/GameObjects.cpp \
Miscellaneous/RandomScripts.cpp \
Miscellaneous/QuelDanasPortal.cpp \
Miscellaneous/DarkmoonFaireObjects.cpp \
EventScripts/Event_DarkPortal.cpp \
EventScripts/Event_DarkPortal.h \
EventScripts/Halloween.cpp \
EventScripts/WinterVeil.cpp \
GossipScripts/Gossip_Battlemaster.cpp \
GossipScripts/Gossip_BoreanTundra.cpp \
GossipScripts/Gossip_Dalaran.cpp \
GossipScripts/Gossip_DarkmoonFaire.cpp \
GossipScripts/Gossip_Guard.cpp \
GossipScripts/Gossip_Innkeepers.cpp \
GossipScripts/Gossip_Moonglade.cpp \
GossipScripts/Gossip_Mulgore.cpp \
GossipScripts/Gossip_Shattrath.cpp \
GossipScripts/Gossip_Stormwind.cpp \
GossipScripts/Gossip_Tanaris.cpp \
GossipScripts/Gossip_Teldrassil.cpp \
GossipScripts/Gossip_Theramore.cpp \
GossipScripts/Gossip_Trainer.cpp \
GossipScripts/Gossip_WyrmrestTemple.cpp \
WorldPvP/ZoneHellfirePeninsula.cpp \
WorldPvP/ZoneSilithus.cpp \
WorldPvP/ZoneTerokkarForest.cpp \
WorldPvP/ZoneZangarmarsh.cpp \
NeutralGuards.cpp \
ScriptStdAfx.cpp \
Setup.cpp