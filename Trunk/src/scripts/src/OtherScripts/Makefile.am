INCLUDES = -I$(top_srcdir)/src/hearthstone-logonserver \
   -I$(top_srcdir)/src/hearthstone-realmserver -I$(top_srcdir)/src/hearthstone-shared -I$(top_srcdir)/src/hearthstone-voicechat \
   -I$(top_srcdir)/src/hearthstone-world -I$(srcdir) -I$(srcdir)/../../../../dep/include \
   -I$(srcdir)/../../../../dep/src -I$(srcdir)/../../../../src
INCLUDES += -I$(srcdir)/../../../hearthstone-shared/g3dlite -I$(srcdir)/../../../hearthstone-shared/vmap
			-I../Common/

AM_CXXFLAGS = -DSCRIPTLIB

lib_LTLIBRARIES = libOtherScripts.la

libOtherScripts_la_SOURCES = \
Miscellaneous/Creatures.cpp \
Miscellaneous/GameObjects.cpp \
Miscellaneous/RandomScripts.cpp \
Miscellaneous/QuelDanasPortal.cpp \
Miscellaneous/DarkmoonFaireObjects.cpp \
EventScripts/Event_DarkPortal.cpp \
EventScripts/Event_DarkPortal.h \
EventScripts/Halloween.cpp \
EventScripts/WinterVeil.cpp \
NeutralGuards.cpp \
../Common/Base.cpp \
../Common/Instance_Base.cpp \
ScriptStdAfx.cpp \
Setup.cpp