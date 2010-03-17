INCLUDES += -I$(srcdir) -I$(srcdir)/../FeatherMoonEmu-shared

bin_PROGRAMS = FeatherMoonEmu-realmserver

FeatherMoonEmu_realmserver_SOURCES = \
ClientManager.h \
ClusterManager.h \
LogonCommClient.h \
LogonCommHandler.h \
Master.h \
ObjectStorage.h \
RStdAfx.h \
Session.h \
Structures.h \
WorkerOpcodes.h \
WorkerServer.h \
WorkerServerSocket.h \
WorldSocket.h \
CharacterHandler.cpp \
ClientManager.cpp \
ClusterManager.cpp \
LogonCommClient.cpp \
LogonCommHandler.cpp \
Master.cpp \
ObjectStorage.cpp \
QueryHandler.cpp \
RStdAfx.cpp \
Session.cpp \
WorkerServer.cpp \
WorkerServerSocket.cpp \
WorldSocket.cpp 
	
FeatherMoonEmu_realmserver_LDADD = -lshared -lz

FeatherMoonEmu_realmserver_LDFLAGS = -L$(srcdir)/../FeatherMoonEmu-shared \
               -L/usr/lib
               
