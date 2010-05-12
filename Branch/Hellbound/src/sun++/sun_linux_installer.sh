#!/bin/sh

#script made by devnull

echo "sun++ installer for linux v. 0.2"

line=`grep -n "src/hearthstone-realmserver/Makefile" ../../configure.ac | cut -d: -f1`

echo "Removing scripts entries from config.ac ..."
cat ../../configure.ac | grep -v 'src/scripts' > ../../config.sun.new

echo "Inserting Sun++ scripts on config file ..."
sed -e "$(( line + 1 ))"i'	src/sun++/src/ServerStatusPlugin/Makefile' ../../config.sun.new8 > ../../config.sun.new9

mv ../../config.sun.new11 ../../configure.ac
rm -f ../../config.sun.*

echo "Replacing the scripts entry in Makefile.am for sun++ entry (you have to use sun++ directory name to work) ..."
sed "s/scripts/sun++\/src/g" ../Makefile.am > makefile.sun.tmp && mv makefile.sun.tmp ../Makefile.am

echo "Finished"
echo "Now, just go to Hearthstone's root dir and run make && make to finish"
