Name: QtVsPlayer
Summary: QtVsPlayer for Hikvision
Version:        1.0.0.5
Release: 1
License: GPL v3
Group: Video 
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Source0: QtVsPlayer-1.0.0.5.tar.xz
Packager: surfzoid
Distribution: Mageia
Url: https://github.com/surfzoid/QtVsPlayer
Vendor: surfzoid
Requires:  lib64qt5multimediawidgets5

%description
QtVsPlayer can read local video files of Hikvision and display blue, green and red vector.
This is the end of an full network backup solution in combination with HikNetExtractor.
https://github.com/surfzoid/HikNetExtractor


Authors:
--------
    Eric Petit <surfzoid@gmail.com>

%global debug_package %{nil}

%prep
%autosetup -n %{name} -p1

%setup -q

%build


%install
%make_install INSTALL_ROOT=%{buildroot} release-install

%clean
rm -rf %buildroot

# %files -f filelist

%qmake_qt5


%make_build release


%files
%defattr(-,root,root)
/usr/bin/QtVsPlayer
/usr/share/applications/QtVsPlayer.desktop
/usr/share/icons/QtVsPlayer.png
/usr/share/QtVsPlayer/translations/*.qm
/usr/lib64/libAudioRender.so
/usr/lib64/libPlayCtrl.so
/usr/lib64/libSuperRender.so
# This is a place for a proper filelist:
# /usr/bin/MonoOSC
# You can also use shell wildcards:
# /usr/share/MonoOSC/*
# This installs documentation files from the top build directory
# into /usr/share/doc/...
# %doc README COPYING
# The advantage of using a real filelist instead of the '-f filelist' trick is
# that rpmbuild will detect if the install section forgets to install
# something that is listed here

%changelog
*Fri May 6 2022 surfzoid@gmail.com
+ First Release
