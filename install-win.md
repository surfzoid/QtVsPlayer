To run QtVsPlayer under Windows 10 you need:  
First, a QT runtime >= 5.15.2, for that you can use the official QT installer from QT website, this one need a QT account and seem "heavy" or my prefered solution, use aqtinstall : https://github.com/miurahr/aqtinstall  

Then you need QtVsplayer.exe and the 3 dll from https://github.com/surfzoid/QtVsPlayer/tree/main/lib/w86_64 in the same dir, for example c:/QtVsPlayer, you can also check the release page, then run QtVsPlayer.exe  

QtVsPlayer use D-Bus to manage single instance, the windows version don't work correctly, it is normal to see dbus error at startup in the console.  
