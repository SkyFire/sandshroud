#!/bin/sh
echo Extracting svn_revision...
sh extract_svn_revision.sh
echo Building Precompiled Header...
cd src/FeatherMoonEmu-world
g++ -pthread -Wall -DHAVE_CONFIG_H -I. -I../.. -I/usr/include/mysql -I/usr/include/openssl -I/usr/local/include/openssl -I/usr/local/include -I. -I./../FeatherMoonEmu-shared -o StdAfx.h.gch StdAfx.h

echo Done!
echo You should run this again every time a .h file changes.

