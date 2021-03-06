%global debug_package %{nil}
Name:           QtVsPlayer
# List of additional build dependencies
BuildRequires:  lib64qt5multimedia-devel
BuildRequires:  lib64qt5multimediawidgets-devel
BuildRequires:  qtbase5-common-devel
BuildRequires:  lib64qt5opengl-devel
Version:        1.0.9
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
%define debug_package %{name}-%{version}-%distsuffix%distro_release.debug

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

# %global debug_package %{nil}

%prep
%setup -n %{name}
# %autosetup -n %{name} -p1
	
%build
qmake
# %qmake_qt5
	
# %make_build debug
make

	
%install
# %make_install INSTALL_ROOT=%{buildroot} debug-install
qmake

make INSTALL_ROOT=%{buildroot} install

%clean
rm -rf %buildroot

%files
%defattr(755,root,root)
/usr/bin/QtVsPlayer
/usr/lib/debug/usr/bin/QtVsPlayer.debug
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
