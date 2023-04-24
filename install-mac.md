I don't have a Mac to test QtVsPlayer, i use GitHub to build releases and don't know if it work.  

To run QtVsPlayer under Mac you need:  
First, a QT runtime >= 5.15.2, for that you can use the official QT installer from QT website, this one need a QT account and seem "heavy" or my prefered solution, use aqtinstall : https://github.com/miurahr/aqtinstall or  
```
brew install qt5
#QtVsPlayer use DBus
brew services start dbus
```
If your Mac version isn't on [Release](https://github.com/surfzoid/QtVsPlayer/releases/latest) page.  
```
git clone https://github.com/surfzoid/QtVsPlayer.git  
cd ./QtVsPlayer  
qmake
make
cp lib/macX86_64/*dylib QtVsPlayer.app/Contents/MacOS/
cd QtVsPlayer.app/Contents/MacOS/
./QtVsPlayer
```


QtVsPlayer use D-Bus to manage single instance.  
