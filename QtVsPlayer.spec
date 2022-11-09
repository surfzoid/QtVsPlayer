Name:           QtVsPlayer
# List of additional build dependencies
BuildRequires:  lib64qt5multimedia-devel
BuildRequires:  lib64qt5multimediawidgets-devel
BuildRequires:  qtbase5-common-devel
Version:        1.0.21
Release:        %mkrel 2
License:        GPL-3.0 license
Source:         %{name}-%{version}.tar.xz

Group:          Video/Players
URL: https://github.com/surfzoid/QtVsPlayer
Summary:        QtVsPlayer for Hikvision
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Requires:  lib64qt5multimedia5
Requires:  lib64qt5multimediawidgets5

%define distsuffix .surf.mlo8
%define dist %distsuffix%distro_release




%description
QtVsPlayer can read local video files of Hikvision and display blue, green and red vector.
This is the end of an full network backup solution in combination with HikNetExtractor.
https://github.com/surfzoid/HikNetExtractor

This software use the PlayCtrl SDK libraries from HikVision, those librarys are under GPL-2 license.
You will find it in /usr/share/licenses/QtVsPlayer/Licenses_playctrl_linux.txt or in SDK archive from HikVision website :
https://www.hikvision.com/content/dam/hikvision/en/support/download/sdk/device-network-sdk/EN-HCNetSDKV6.1.9.4_build20220412_linux64.rar
Licences are in the doc dir.

Authors:
--------
    Eric Petit <surfzoid@gmail.com>

%prep
%setup -q -n %{name}
%qmake_qt5

%build
qmake
#make_install

%install
qmake

make INSTALL_ROOT=%{buildroot}/usr/share install

%clean
rm -rf %buildroot
chmod -R ug+rw %{_rpmdir}
chmod -R ug+rw %{_srcrpmdir}

%files
%defattr(755,root,root)
/usr/bin/QtVsPlayer
/usr/share/applications/QtVsPlayer.desktop
/usr/share/icons/QtVsPlayer.png
/usr/share/QtVsPlayer/translations/*.qm
%license LICENSE
%license Licenses_playctrl_linux.txt
/usr/share/doc/QtVsPlayer/README.md
/usr/lib64/libAudioRender.so
/usr/lib64/libPlayCtrl.so
/usr/lib64/libSuperRender.so

%changelog
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
