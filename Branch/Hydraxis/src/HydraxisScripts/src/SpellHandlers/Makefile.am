INCLUDES = -I$(top_srcdir)/src/hearthstone-logonserver \
   -I$(top_srcdir)/src/hearthstone-realmserver -I$(top_srcdir)/src/hearthstone-shared -I$(top_srcdir)/src/hearthstone-voicechat \
   -I$(top_srcdir)/src/hearthstone-world -I$(srcdir) -I$(srcdir)/../../../../dep/include \
   -I$(srcdir)/../../../../dep/src -I$(srcdir)/../../../../src
INCLUDES += -I$(srcdir)/../../../hearthstone-shared/g3dlite -I$(srcdir)/../../../hearthstone-shared/vmap
AM_CXXFLAGS = -DSCRIPTLIB

lib_LTLIBRARIES = libSpellHandlers.la

libSpellHandlers_la_SOURCES = DruidSpells.cpp HunterSpells.cpp ItemSpells_1.cpp \
MageSpells.cpp PaladinSpells.cpp PriestSpells.cpp OldSpellCPPStuff.cpp RogueSpells.cpp \
ShamanSpells.cpp WarlockSpells.cpp WarriorSpells.cpp Raid_BlackTemple.cpp QIspells.cpp Setup.cpp
