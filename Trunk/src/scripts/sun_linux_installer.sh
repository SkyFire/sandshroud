#!/bin/sh

#script made by devnull

echo "sun++ installer for linux v. 0.3"

line=`grep -n "src/hearthstone-realmserver/Makefile" ../../configure.ac | cut -d: -f1`

echo "Removing scripts entries from config.ac ..."
cat ../../configure.ac | grep -v 'src/scripts' > ../../config.sun.new

echo "Inserting Sun++ scripts on config file ..."
sed -e "$(( line + 1 ))"i'	src/scripts/src/Makefile' ../../config.sun.new > ../../config.sun.new1
sed -e "$(( line + 1 ))"i'	src/scripts/src/InstanceScripts/Makefile' ../../config.sun.new2 > ../../config.sun.new3
sed -e "$(( line + 1 ))"i'	src/scripts/src/Lua/Makefile' ../../config.sun.new3 > ../../config.sun.new4
sed -e "$(( line + 1 ))"i'	src/scripts/src/OtherScripts/Makefile' ../../config.sun.new4 > ../../config.sun.new5
sed -e "$(( line + 1 ))"i'	src/scripts/src/QuestScripts/Makefile' ../../config.sun.new5 > ../../config.sun.new6
sed -e "$(( line + 1 ))"i'	src/scripts/src/ServerStatusPlugin/Makefile' ../../config.sun.new6 > ../../config.sun.new7

mv ../../config.sun.new11 ../../configure.ac
rm -f ../../config.sun.*

echo "Replacing the scripts entry in Makefile.am for sun++ entry (you have to use sun++ directory name to work) ..."
sed "s/scripts/scripts\/src/g" ../Makefile.am > makefile.sun.tmp && mv makefile.sun.tmp ../Makefile.am

echo "Finished"
echo "Now, just go to Hearthstone's root dir and run make && make to finish"
