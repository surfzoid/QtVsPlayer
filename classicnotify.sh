#!/bin/bash
DAT=$(date +%Y%m%d)
CAM="cam1 "
CHEMIN=/mnt/cams/cam1/$DAT/

first="$CHEMIN"
if [ -d "$CHEMIN" ];then
	first=`ls -1rt $CHEMIN | tail -n 1`
fi

echo $first
    while true
    do 
if [ -d "$CHEMIN" ];then
       last=`ls -1rt $CHEMIN | tail -n 1`
       if [ $first !=  $last ]
       then
          first=$last
          echo $last created
          #cp /usr/share/applications/QtVsPlayer.desktop /usr/share/applications/QtVsPlayernotif.desktop
          #then edit it and replace exe by the script  /usr/bin/QtVsPlayernotif
          sudo sed -i "2s|.*|QtVsPlayer $CHEMIN$last|" "/usr/bin/QtVsPlayernotif"
          #notify-send -h string:desktop-entry:nautilus -c "transfer.complete" -u critical -i $PWD../QtVsPlayer.png $CAM $last"\n\r"$CHEMIN 
          reply=$(dunstify -h int:value:12 -a QtVsPlayernotif -A 'open,ouvrir' -i "QtVsPlayer" "$CAM $last"\n\r"$CHEMIN")
            if [[ "$reply" == "open" ]]; then
				QtVsPlayer $CHEMIN$last
			fi
       fi
       fi
       sleep 3s
    done
    


