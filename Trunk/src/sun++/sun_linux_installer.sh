#!/bin/sh

#script made by devnull

echo "sun++ installer for linux v. 0.2"

line=`grep -n "src/hearthstone-realmserver/Makefile" ../../configure.ac | cut -d: -f1`

echo "Removing scripts entries from config.ac ..."
cat ../../configure.ac | grep -v 'src/scripts' > ../../config.sun.new

echo "Inserting Sun++ scripts on config file ..."
sed -e "$(( line + 1 ))"i'	src/sun++/src/Makefile' ../../config.sun.new > ../../config.sun.new1
sed -e "$(( line + 1 ))"i'	src/sun++/src/EventScripts/Makefile' ../../config.sun.new1 > ../../config.sun.new2
sed -e "$(( line + 1 ))"i'	src/sun++/src/ExtraScripts/Makefile' ../../config.sun.new2 > ../../config.sun.new3
sed -e "$(( line + 1 ))"i'	src/sun++/src/GossipScripts/Makefile' ../../config.sun.new3 > ../../config.sun.new4
sed -e "$(( line + 1 ))"i'	src/sun++/src/InstanceScripts/Makefile' ../../config.sun.new4 > ../../config.sun.new5
sed -e "$(( line + 1 ))"i'	src/sun++/src/LUAScripting/Makefile' ../../config.sun.new5 > ../../config.sun.new6
sed -e "$(( line + 1 ))"i'	src/sun++/src/MiscScripts/Makefile' ../../config.sun.new6 > ../../config.sun.new7
sed -e "$(( line + 1 ))"i'	src/sun++/src/QuestScripts/Makefile' ../../config.sun.new7 > ../../config.sun.new8
sed -e "$(( line + 1 ))"i'	src/sun++/src/ServerStatusPlugin/Makefile' ../../config.sun.new8 > ../../config.sun.new9
sed -e "$(( line + 1 ))"i'	src/sun++/src/SpellHandlers/Makefile' ../../config.sun.new9 > ../../config.sun.new10
sed -e "$(( line + 1 ))"i'	src/sun++/src/WorldPvPScripts/Makefile' ../../config.sun.new10 > ../../config.sun.new11

mv ../../config.sun.new11 ../../configure.ac
rm -f ../../config.sun.*

echo "Replacing the scripts entry in Makefile.am for sun++ entry (you have to use sun++ directory name to work) ..."
sed "s/scripts/sun++\/src/g" ../Makefile.am > makefile.sun.tmp && mv makefile.sun.tmp ../Makefile.am

echo "Finished"
echo "Now, just go to Hearthstone's root dir and run make && make to finish"
