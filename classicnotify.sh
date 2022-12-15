#!/bin/bash
#special notify-send from https://github.com/vlevit/notify-send.sh
# need notify-action.sh and notify-send.sh in the same dir of classicnotify.sh
DAT=$(date +%Y%m%d)
CAM="NVR"
CHEMIN=/mnt/cams/cam4/$CAM/$DAT/
#CHEMIN=/home/eric/Musique/$DAT/
CAMLOW=$(echo "$CAM" | tr '[:upper:]' '[:lower:]')
CURPATH=/home/eric/scripts/hik/$CAMLOW
first="$CHEMIN"

if [ -d "$CHEMIN" ];then
	first=`ls -1rt $CHEMIN | tail -n 1`
fi

echo $first
    while true
    do 
if [ -d "$CHEMIN" ];then
       last=`ls -1rt $CHEMIN | tail -n 1`
       if [ "$first" !=  "$last" ]
       then
          first=$last
          echo $last created
          sudo sh -c "echo -e '$CHEMIN$last \c' >> /usr/bin/QtVsPlayernotif$CAM"
          $CURPATH/notify-send.sh --icon=QtVsPlayer --app-name=QtVsPlayer-Notif --hint=string:sound-name:bark -o QtVsPlayer:"QtVsPlayer $CHEMIN$last > /dev/null" -d "QtVsPlayer $CHEMIN$last > /dev/null" QtVsPlayer "$CAM : $last$CHEMIN" &
       fi
       fi
       sleep 3m
    done
    

