# QtVsPlayer for Hikvision.
Read local video files of Hikvision devices and display blue, green and red vectors.

# Tips
Double click on the video toggle fullscreen

Midle click on the control move them

use https://github.com/surfzoid/HikNetExtractor in an shelde task to automaticaly download video.

# install
git clone https://github.com/surfzoid/QtVsPlayer.git

cd ./QtVsPlayer

qmake

make

sudo make install

# TODO
Copy lib/$ARCH/* to the same dir of the executable durring make install.

check with

ldd /opt/QtVsPlayer/QtVsPlayer 

and

cd /opt/QtVsPlayer && ldd ./QtVsPlayer

Add sound managment.



Enjoy.
