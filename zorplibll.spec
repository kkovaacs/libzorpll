Summary: libzorpll3.9-0 library needed by various Zorp components
Name: libzorpll3.9-0
Version: 3.9.0.1
Release: 1
License: GPL
Group: Libraries
Source: libzorpll_%{version}.tar.gz
URL: http://www.balabit.com/
Packager: ZorpOS Maintaners <zorpos@balabit.com>
Vendor: Balabit IT Ltd.
#Requires:
BuildRoot: %{_tmppath}/%{name}-root
BuildRequires: openssl-devel >= 0.9.7a, glib2-devel >= 2.4.0, libcap-devel
#BuildConflicts:
Exclusivearch: i386

%define prefix /usr

%description
Zorp is a new generation firewall. It is essentially a transparent proxy
firewall, with strict protocol analyzing proxies, a modular architecture,
and fine-grained control over the mediated traffic. Configuration decisions
are scriptable with the Python based configuration language.

This package contains low level library functions needed by Zorp and
associated programs.

%package -n libzorpll-dev
Summary: libzorpll3.9-0 development package
Group: Development/Libraries
Requires: libzorpll3.9-0 = %{version}
Provides: libzorpll-devel
%description -n libzorpll-dev
Zorp is a new generation firewall. It is essentially a transparent proxy
firewall, with strict protocol analyzing proxies, a modular architecture,
and fine-grained control over the mediated traffic. Configuration decisions
are scriptable with the Python based configuration language.

This package contains the development files necessary to create programs
based on libzorpll.

%prep
%setup -q -n libzorpll-%{version}

%build
./configure --prefix=%{prefix} --mandir=%{_mandir} --infodir=%{_infodir} \
	--sysconfdir=/etc --disable-dmalloc
make

%install
make install DESTDIR="$RPM_BUILD_ROOT"
#gzip -9f $RPM_BUILD_ROOT/%{_mandir}/*/*
#gzip -9f $RPM_BUILD_ROOT/%{_infodir}/*info*

%files
%defattr(-,root,root)
%doc README NEWS
%{prefix}/lib/libzorpll-*.so*
#%{_infodir}/*
#%{_mandir}/*/*
%files -n libzorpll-dev
%defattr(-,root,root)
%{prefix}/include/zorp/*.h
%{prefix}/lib/libzorpll.a
%{prefix}/lib/libzorpll.la
%{prefix}/lib/libzorpll*.so
%{prefix}/lib/pkgconfig/zorplibll.pc

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%check
%clean
[ $RPM_BUILD_ROOT == / ] || rm -rf $RPM_BUILD_ROOT

%changelog
* Wed Feb 23 2005 ZorpOS Maintaners <zorpos@balabit.com>
- renamed the libzorpll-devel package to libzorpll-dev 
- finished branching
* Mon Feb 20 2005 ZorpOS Maintaners <zorpos@balabit.com>
- fixed devel package references
- changed package descriptions to match their Debian counterparts
* Mon Jan 10 2005 ZorpOS Maintaners <zorpos@balabit.com>
- updated the spec file to use substitutable version numbers
- included spec file to the upstream tarball
* Tue Dec 14 2004 ZorpOS Maintaners <zorpos@balabit.com>
- new pre-release version
- updated RPM packaging
* Thu Oct 14 2004 ZorpOS Maintaners <zorpos@balabit.com>
- initial RPM packaging
