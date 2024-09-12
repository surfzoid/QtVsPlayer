Name:           QtVsPlayer
Summary:        QtVsPlayer for Hikvision
Version:        1.0.52

%global Rel 6
%if 0%{?mageia}
Release:        %mkrel %{Rel}
%elif 0%{?fedora} > 36 || 0%{?rhel} > 6
Release:        %{Rel}.surf%{?dist}
%else
Release:        %{Rel}.surf.mlo
%endif

License:        GPLv3
Group:          Video/Players
URL:            https://github.com/surfzoid/QtVsPlayer
Source0:        https://github.com/surfzoid/QtVsPlayer/archive/%{version}-%{Rel}/%{name}-%{version}-%{Rel}.tar.gz
# List of additional build dependencies
BuildRequires:  pkgconfig(Qt5Network)
%if 0%{?mageia}
BuildRequires:  qtbase5-common-devel
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5NetworkAuth)
BuildRequires:  pkgconfig(Qt5Widgets)
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
Requires: lib64qt5network5
Requires: qtbase5-common
Requires: lib64openal1

%global debug_package %{nil}

# Mageia 9 don't have openssl 1.1 anymore, avoid conflict don't replace openssl of Mageia
%global __provides_exclude %{?__provides_exclude:%__provides_exclude|}^libcrypto|^libssl\\.so|^libopenal\\.so*|^libz\\.so*
%global __requires_exclude %{?__requires_exclude:%__requires_exclude|}^libcrypto|^libssl\\.so|^libopenal\\.so*|^libz\\.so*


%description
QtVsPlayer can read local video files of Hikvision and display blue, green and red vectors.
This is the end of an full network backup solution in combination with HikNetExtractor.
https://github.com/surfzoid/HikNetExtractor
It read real time live stream.
Join us on IRC https://matrix.to/#/#QtVsPlayer:libera.chat or https://matrix.to/#/#QtVsPlayer:matrix.org
 
This software use the PlayCtrl SDK libraries from HikVision, those librarys are under GPL-2 license.
You will find it in /usr/share/licenses/QtVsPlayer/Licenses_playctrl_linux.txt or in SDK archive from HikVision website :
https://www.hikvision.com/content/dam/hikvision/en/support/download/sdk/device-network-sdk/EN-HCNetSDKV6.1.9.4_build20220412_linux64.rar
Licences are in the doc dir.
 
%prep
%autosetup -n %{name}-%{version}-%{Rel}
sed -i 's/\(VERSION = \).*/\1         %{version}/' %{_builddir}/%{name}-%{version}-%{Rel}/QtVsPlayer.pro
sed -i 's/\(RELEASE = \).*/\1         %{Rel}/' %{_builddir}/%{name}-%{version}-%{Rel}/QtVsPlayer.pro
 
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
chrpath -d %{buildroot}%{_libdir}/QtVsPlayer/*.so*
chrpath -d %{buildroot}%{_libdir}/QtVsPlayer/HCNetSDKCom/*.so*
ln -sr %{_libdir}/libopenal.so.1 %{buildroot}%{_libdir}/QtVsPlayer/

if [ -f $(whereis "libssl.so.1.1" | sed "s|.* \(.*libssl.so.1.1\).*|\1|") ];
then
    echo -e "there is openssl 1.1, no need to embeded it and use symlink";
    rm -f %{buildroot}%{_libdir}/QtVsPlayer/libcrypto.so*;
    rm -f %{buildroot}%{_libdir}/QtVsPlayer/libssl.so*;
    ln -sr %{_libdir}/libcrypto.so.1.1 %{buildroot}%{_libdir}/QtVsPlayer/;
    ln -sr %{_libdir}/libcrypto.so %{buildroot}%{_libdir}/QtVsPlayer/;
    ln -sr %{_libdir}/libssl.so.1.1 %{buildroot}%{_libdir}/QtVsPlayer/;
    ln -sr %{_libdir}/libssl.so %{buildroot}%{_libdir}/QtVsPlayer/;
fi

ln -sr %{_libdir}/libz.so %{buildroot}%{_libdir}/QtVsPlayer/libz.so.1

mkdir -p %{buildroot}/etc/ld.so.conf.d

cat << 'EOF' > %{buildroot}/etc/ld.so.conf.d/QtVsPlayer.conf
/usr/lib64/QtVsPlayer/
/usr/lib64/QtVsPlayer/HCNetSDKCom/
EOF

%post
/sbin/ldconfig

%preun

%postun 
if [ $1 == 0 ];then
    if [ -d "%{_libdir}/QtVsPlayer" ];then
        rm -fr %{_libdir}/QtVsPlayer
    fi
fi
#/sbin/ldconfig

%clean
rm -rf %buildroot
chmod -R ug+rw %{_rpmdir}
chmod -R ug+rw %{_srcrpmdir}
 
%files
%license LICENSE
%license Licenses_playctrl_linux.txt
%doc README.md
%if 0%{?suse_version}
%{_datadir}/doc/QtVsPlayer/README.md
%endif
%{_bindir}/QtVsPlayer
%{_datadir}/applications/QtVsPlayer.desktop
%{_datadir}/icons/QtVsPlayer.png
%{_datadir}/QtVsPlayer/
%dir %{_libdir}/QtVsPlayer/
%{_sysconfdir}/ld.so.conf.d/QtVsPlayer.conf
%{_libdir}/QtVsPlayer/*

%changelog
* Wed May 01 2024 surfzoid@gmail.com
+ Fix, some devices reject custom resolution for jpeg captures.

* Fri Oct 20 2023 surfzoid@gmail.com
+ Smartest forward and backward slider..

* Sat Jul 29 2023 surfzoid@gmail.com
+ Config of multimedia shorcut key.

* Sat Jul 22 2023 surfzoid@gmail.com
- Mageia 9 use openssl3, use openssl 1.1 libs from mga8 in /usr/lib64/QtVsPlayer.
+ Use release number for about box and packages built at Github.

* Sat Jul 08 2023 surfzoid@gmail.com
+ Missing translation.
+ Playback, screenshot include one picture with VCA.
+ Live view reconnect option.

* Mon Jul 03 2023 surfzoid@gmail.com
+ Missing translation.
+ Settings form cosmetic.
+ Set OSD position.
+ Play/Pause live view.
- windows compatibility issue.

* Mon Jun 26 2023 surfzoid@gmail.com
+ PlayCtrl_V7.4.0.45_10252_Arm64_Build20221221

* Sat Jun 24 2023 surfzoid@gmail.com
+ Show XML response option in console.

* Wed Jun 21 2023 surfzoid@gmail.com
+ Design info dialog.
+ Display time play in hour.
+ Missing lib from PlayCtrl x86_64V7.4.0.41 SDK.

* Mon Jun 19 2023 surfzoid@gmail.com
+ PlayCtrl x86_64V7.4.0.41.
+ Live view Manage channel name and display.
+ NVR can now PTZ, NVR live view is ok.
+ Live view Force disable stream encrypt.

* Thu Jun 1 2023 surfzoid@gmail.com
+ Live view, devel in progress.

* Wed May 24 2023 surfzoid@gmail.com
+ Live view use Hikvision network SDK, no more Qtmultimedia.

* Tue May 09 2023 surfzoid@gmail.com
+ Add infos/metadata widget.

* Sat Apr 01 2023 surfzoid@gmail.com
+ Add debian files for launchpad construction.

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
