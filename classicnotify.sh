#!/bin/bash
#special notify-send from https://github.com/vlevit/notify-send.sh
# need notify-action.sh and notify-send.sh in the same dir of classicnotify.sh
DAT=$(date +%Y%m%d)
CAM="NVR"
CHEMIN=/mnt/cams/cam4/$CAM/$DAT/
#CHEMIN=/home/eric/Musique/$DAT/
CAMLOW=$(echo "$CAM" | tr '[:upper:]' '[:lower:]')
CURPATH=/home/eric/scripts/hik/$CAMLOW

#dbus-send --dest=org.freedesktop.Notifications --print-reply  /org/freedesktop/Notifications org.freedesktop.Notifications.GetCapabilities

first="$CHEMIN"
if [ -d "$CHEMIN" ];then
	first=`ls -1rt $CHEMIN | tail -n 1`
fi
if [ -f /usr/bin/QtVsPlayernotif$CAM ]
then
	sudo sh -c "rm -f /usr/bin/QtVsPlayernotif$CAM"
fi
	sudo sh -c "echo '#!/bin/bash' >> /usr/bin/QtVsPlayernotif$CAM"
	sudo sh -c "echo -e '/usr/bin/QtVsPlayer \c' >> /usr/bin/QtVsPlayernotif$CAM"
	sudo chmod +Xx /usr/bin/QtVsPlayernotif$CAM

echo $first
    while true
    do 
if [ -d "$CHEMIN" ];then
       last=`ls -1rt $CHEMIN | tail -n 1`
       if [ "$first" !=  "$last" ]
       then
          first=$last
          echo $last created
          #sudo sed -i "2s|.*|QtVsPlayer $CHEMIN$last|" "/usr/bin/QtVsPlayernotif$CAM"
		sudo sh -c "echo -e '$CHEMIN$last \c' >> /usr/bin/QtVsPlayernotif$CAM"
          #notify-send -h string:desktop-entry:nautilus -c "transfer.complete" -u critical -i $PWD../QtVsPlayer.png $CAM $last"\n\r"$CHEMIN 
$CURPATH/notify-send.sh --icon=QtVsPlayer --app-name=QtVsPlayer-Notif --hint=string:sound-name:bark -o QtVsPlayer:"QtVsPlayer $CHEMIN$last > /dev/null" -d "QtVsPlayer $CHEMIN$last > /dev/null" QtVsPlayer "$CAM : $last$CHEMIN" &
          #reply=$(dunstify -h int:value:12 -a `QtVsPlayer "$CHEMIN$last"` -A 'open,ouvrir' -i "QtVsPlayer" "$CAM : $last$CHEMIN") &
            #if [[ "$reply" == "open" ]]; then
				#QtVsPlayer $CHEMIN$last
				#/usr/bin/QtVsPlayernotif$CAM
			#fi
       fi
       fi
       sleep 3m
    done
    

