#!/bin/bash
SRC=/home/eric/Projets/qt/QtVsPlayer/QtVsPlayer.pro

Version=$(grep "VERSION =" $SRC  | tac -s' ' | head -1)

git tag $Version
git push git+ssh://surfzoid@git.launchpad.net/~surfzoid/+git/QtVsPlayer
git push --tags git+ssh://surfzoid@git.launchpad.net/~surfzoid/+git/QtVsPlayer
#git push --tags 
