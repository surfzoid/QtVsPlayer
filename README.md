# QtVsPlayer for Hikvision.
an read local video files of Hikvision and display blue, green and red vectors.

# Tips
Double click on the video toggle fullscreen
Midle click on the control move them


# install
git clone https://github.com/surfzoid/QtVsPlayer.git
cd ./QtVsPlayer
qmake
make
sudo make install

# TODO
Copy lib/$ARCH/* to the same dir of the executable durring make install
check with 
ldd /opt/QtVsPlayer/QtVsPlayer 
and
cd /opt/QtVsPlayer && ldd ./QtVsPlayer



Enjoy.
