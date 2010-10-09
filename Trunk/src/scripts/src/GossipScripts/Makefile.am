INCLUDES = -I$(top_srcdir)/src/hearthstone-logonserver \
   -I$(top_srcdir)/src/hearthstone-realmserver -I$(top_srcdir)/src/hearthstone-shared -I$(top_srcdir)/src/hearthstone-voicechat \
   -I$(top_srcdir)/src/hearthstone-world -I$(srcdir) -I$(srcdir)/../../../../dep/include \
   -I$(srcdir)/../../../../dep/src -I$(srcdir)/../../../../src
INCLUDES += -I$(srcdir)/../../../hearthstone-shared/g3dlite -I$(srcdir)/../../../hearthstone-shared/vmap \
            -I../Common/

AM_CXXFLAGS = -DSCRIPTLIB

lib_LTLIBRARIES = libGossipScripts.la

libGossipScripts_la_SOURCES = \
Gossip_Battlemaster.cpp \
Gossip_BoreanTundra.cpp \
Gossip_Dalaran.cpp \
Gossip_DarkmoonFaire.cpp \
Gossip_Guard.cpp \
Gossip_Innkeepers.cpp \
Gossip_Moonglade.cpp \
Gossip_Mulgore.cpp \
Gossip_Shattrath.cpp \
Gossip_Stormwind.cpp \
Gossip_Tanaris.cpp \
Gossip_Teldrassil.cpp \
Gossip_Theramore.cpp \
Gossip_Trainer.cpp \
Gossip_WyrmrestTemple.cpp \
ScriptStdAfx.cpp \
Setup.cpp