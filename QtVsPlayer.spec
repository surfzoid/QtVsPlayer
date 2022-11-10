Name:           QtVsPlayer
Summary:        QtVsPlayer for Hikvision
Version:        1.0.21
Release:        %mkrel 2
License:        GPLv3
Group:          Video/Players
URL: https://github.com/surfzoid/QtVsPlayer
Source:         %{name}-%{version}.tar.xz
# List of additional build dependencies
BuildRequires:  pkgconfig(Qt5Multimedia)
BuildRequires:  pkgconfig(Qt5MultimediaWidgets)
BuildRequires:  qtbase5-common-devel

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
%autosetup -p1 -n %{name}
 
%build
%qmake_qt5
%make_build
 
%install
%make_install INSTALL_ROOT=%{buildroot}%{_datadir}
rm -rf %{buildroot}%{_datadir}/licenses/%{name}-%{version}

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
%{_libdir}/libAudioRender.so
%{_libdir}/libPlayCtrl.so
%{_libdir}/libSuperRender.so

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
