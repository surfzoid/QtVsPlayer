Name:           QtVsPlayer
Summary:        QtVsPlayer for Hikvision
Version:        1.0.39

%if 0%{?mageia}
Release:        %mkrel 1
%else
Release:        1
%endif

License:        GPLv3
Group:          Video/Players
URL:            https://github.com/surfzoid/QtVsPlayer
Source0:        https://github.com/surfzoid/QtVsPlayer/archive/%{version}/%{name}-%{version}.tar.gz
# List of additional build dependencies
BuildRequires:  pkgconfig(Qt5Multimedia)
BuildRequires:  pkgconfig(Qt5MultimediaWidgets)
%if 0%{?mageia}
BuildRequires:  qtbase5-common-devel
%else
%if 0%{?suse_version}
BuildRequires:  libqt5-qtbase-devel
%else
BuildRequires:  pkgconfig(Qt5)
%endif
%endif
%if 0%{?el7} || 0%{?ol7}
BuildRequires:  openal-soft-devel
%else
BuildRequires:  pkgconfig(openal)
%endif

BuildRequires: chrpath
Requires: lib64qt5multimedia5
Requires: lib64qt5multimediawidgets5
Requires: qtbase5-common
Requires: lib64openal1
 
%description
QtVsPlayer can read local video files of Hikvision and display blue, green and red vectors.
This is the end of an full network backup solution in combination with HikNetExtractor.
https://github.com/surfzoid/HikNetExtractor
 
This software use the PlayCtrl SDK libraries from HikVision, those librarys are under GPL-2 license.
You will find it in /usr/share/licenses/QtVsPlayer/Licenses_playctrl_linux.txt or in SDK archive from HikVision website :
https://www.hikvision.com/content/dam/hikvision/en/support/download/sdk/device-network-sdk/EN-HCNetSDKV6.1.9.4_build20220412_linux64.rar
Licences are in the doc dir.
 
%prep
%autosetup -n %{name}-%{version}
 
%build
%if 0%{?suse_version}
#find /|grep -i qmake
/usr/bin/qmake-qt5
%else
%qmake_qt5
%endif
%make_build
 
%install
%make_install INSTALL_ROOT=%{buildroot}%{_datadir}
mkdir -p %{buildroot}%{_libdir}/QtVsPlayer/
chrpath -d %{buildroot}%{_libdir}/QtVsPlayer/libAudioRender.so
chrpath -d %{buildroot}%{_libdir}/QtVsPlayer/libPlayCtrl.so
chrpath -d %{buildroot}%{_libdir}/QtVsPlayer/libSuperRender.so
ln -s %{_libdir}/libopenal.so.1 %{buildroot}%{_libdir}/QtVsPlayer/

%post
/sbin/ldconfig /usr/lib64/QtVsPlayer/


%clean
rm -rf %buildroot
chmod -R ug+rw %{_rpmdir}
chmod -R ug+rw %{_srcrpmdir}
 
%files
%license LICENSE
%license Licenses_playctrl_linux.txt
%doc README.md
%{_bindir}/QtVsPlayer
%{_datadir}/applications/QtVsPlayer.desktop
%{_datadir}/icons/QtVsPlayer.png
%{_datadir}/QtVsPlayer/
%dir %{_libdir}/QtVsPlayer/
%{_libdir}/QtVsPlayer/libAudioRender.so
%{_libdir}/QtVsPlayer/libPlayCtrl.so
%{_libdir}/QtVsPlayer/libSuperRender.so
%{_libdir}/QtVsPlayer/libopenal.so.1

%changelog
* Mon Feb 06 2023 surfzoid@gmail.com
+ Click + move scroll zoomed video.

* Thu Feb 02 2023 surfzoid@gmail.com
+ don't rm libopenal.so.1 when upgrade.
+ Playlist can be filtered by DateTime.
+ Clean shared memory when chrash.
+ Single instance Raise QtVsPlayer.

* Sat Jan 28 2023 surfzoid@gmail.com
+ Buil-Rpm-action have now a post section.
+ "quit" in file menu.
+ More shortcuts.
+ "%post" and "%preun" sections in spec file.


* Wed Jan 25 2023 surfzoid@gmail.com
+ Finish missing translations.
+ Tool tip on sound volume slider.
- bug in show/hide video controls.

* Mon Jan 23 2023 surfzoid@gmail.com
+ Make links to libopenal and provide sound.

* Tue Jan 17 2023 surfzoid@gmail.com
- Full screen bug.
+ Better pan tilt window command.

* Tue Jan 17 2023 surfzoid@gmail.com
+ ptz command more accurate.
+ Use scrollbars to move zoomed video rather mouse.
+ Prefer Widget rather CentralWidget to play video.
+ RtspPlayer: cosmetic and auto hide menu.
+ Save RtspPlayer windows size and position in settings.

* Tue Jan 10 2023 surfzoid@gmail.com
+ Pressets are loaded and saved from/to the device.
+ Encrypt password in settings.
+ Save video controls position in settings.
+ Save Main window size and position in settings.
+ Save display menu items state in settings.

* Thu Dec 15 2022 surfzoid@gmail.com
+ Manage VCA displayed.

* Mon Dec 12 2022 surfzoid@gmail.com
+ Audio and Video decoder callback work together!

* Thu Dec 8 2022 surfzoid@gmail.com
+ Another callback worarround.
+ Audio mute button.

* Mon Dec 5 2022 surfzoid@gmail.com
+ Regession bug in DBus get file instance.
+ Various typo corections.
+ More verbose info in the console.
+ Prevent crash when clickinfo

* Thu Dec 1 2022 surfzoid@gmail.com
+ Always hell of callbacks

* Mon Nov 28 2022 surfzoid@gmail.com
+ Display video infos in the terminal

* Wed Nov 23 2022 surfzoid@gmail.com
+ Regression bug.
+ Don't pause when move slide.
+ Mac support.

* Wed Nov 9 2022 surfzoid@gmail.com
+ RTSP Video resolution info.
+ MS Windows support.
+ Add a github PPA(see README.md).

* Sat Oct 1 2022 surfzoid@gmail.com
+ Use DBus for single instance.

* Fri Sep 16 2022 surfzoid@gmail.com
+ Detach sharedMemory after one crash.

* Mon Sep 12 2022 surfzoid@gmail.com
+ No middle click when use seek slider.
+ Single instance
+ IRC and Matrix chanels

* Fri Aug 26 2022 surfzoid@gmail.com
+ Set PTZSpeed default value to 3.
+ Put FsList on top
+ Dont zoom if VideoCtrls has mouse focus
+ Solve a minor regression bug

* Tue Aug 9 2022 surfzoid@gmail.com
+ TimeSlider wheel mouse make pause
+ Auto_hide_controls bug fixed

* Mon Aug 8 2022 surfzoid@gmail.com
+ Auto hide controls and mousse curssor

*Thu Aug 4 2022 surfzoid@gmail.com
+ Pause when TimeSlider move

*Wed Aug 3 2022 surfzoid@gmail.com
+ Really TimeSlider more acurate

*Mon Aug 1 2022 surfzoid@gmail.com
+ TimeSlider more acurate
 
*Sat Jul 30 2022 surfzoid@gmail.com
+ Add ISAPI record manual
+ Better rtsp status/feedback
 
*Fri Jun 17 2022 surfzoid@gmail.com
+ Update README.md for MLO Repo and git bug
+ Some cosmetic

*Mon Jun 6 2022 surfzoid@gmail.com
+ More translation
+ Set default filename of snapshot
+ PTZ>Reboot
+ Save preset and patrol names
+ Sound is now managed

*Thu May 12 2022 surfzoid@gmail.com
+ PlayCtrl SDK licence
+ README.md

*Fri May 6 2022 surfzoid@gmail.com
+ First Release
