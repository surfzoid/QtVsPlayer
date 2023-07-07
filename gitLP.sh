#!/bin/bash
SRC=/home/eric/Projets/qt/QtVsPlayer/QtVsPlayer.pro

Version=$(grep "VERSION =" $SRC  | tac -s' ' | head -1)

git tag $Version
git push origin
git push --tags origin
#git push --tags 
