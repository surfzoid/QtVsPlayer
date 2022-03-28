#!/bin/bash

qmake
make
#some distro ask for root access
sudo mkdir /opt/QtVsPlayer & sudo chown -R $USER /opt/QtVsPlayer
make install
cp -f ./lib/$(uname -m)/* /opt/QtVsPlayer/
cp -f ./QtVsPlayer_fr_FR.qm /opt/QtVsPlayer/
cp -f ./images/QtVsPlayer.png $HOME/.local/share/icons/
cp -f ./QtVsPlayer.desktop $HOME/.local/share/applications
cd /opt/QtVsPlayer/
./QtVsPlayer
