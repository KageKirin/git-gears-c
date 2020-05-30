#
# Copyright 2011-2014 Branimir Karadzic. All rights reserved.
# License: http://www.opensource.org/licenses/BSD-2-Clause
#

UNAME := $(shell uname)
ifeq ($(UNAME),$(filter $(UNAME),Linux Darwin SunOS FreeBSD GNU/kFreeBSD NetBSD OpenBSD GNU))
ifeq ($(UNAME),$(filter $(UNAME),Darwin))
OS=darwin
else
ifeq ($(UNAME),$(filter $(UNAME),SunOS))
OS=solaris
else
ifeq ($(UNAME),$(filter $(UNAME),FreeBSD GNU/kFreeBSD NetBSD OpenBSD))
OS=bsd
else
OS=linux
endif
endif
endif
else
OS=windows
endif

.PHONY: release

## just provide genie on the command line
GENIE?=../genie/bin/$(OS)/genie

## main target
GITCGEARS=bin/$(OS)/git-cgears

SILENT?=@

GENIE_OPTIONS?=

PROJECT_TYPE?=ninja

$(GITCGEARS): projgen
	$(SILENT) $(MAKE) -C .build/$(PROJECT_TYPE).$(OS) GENIE_OPTIONS=$(GENIE_OPTIONS)

all: $(GITCGEARS)

clean:
	$(SILENT) $(MAKE) -C build/$(PROJECT_TYPE).$(OS) clean
	$(SILENT) -rm -rf bin

## 'projgen' rules default to $(OS) only
projgen: projgen-$(OS)

projgen-darwin:
	$(SILENT) $(GENIE) --to=../.build/$(PROJECT_TYPE).darwin  --os=macosx --platform=universal $(GENIE_OPTIONS) $(PROJECT_TYPE)

projgen-linux:
	$(SILENT) $(GENIE) --to=../.build/$(PROJECT_TYPE).linux   --os=linux $(GENIE_OPTIONS) $(PROJECT_TYPE)

projgen-windows:
	$(SILENT) $(GENIE) --to=../.build/$(PROJECT_TYPE).windows --os=windows $(GENIE_OPTIONS) $(PROJECT_TYPE)

projgen-freebsd:
	$(SILENT) $(GENIE) --to=../.build/$(PROJECT_TYPE).freebsd --os=bsd $(GENIE_OPTIONS) $(PROJECT_TYPE)

## extra 'projgen' rules targeting an IDE
projgen-xcode:
	$(SILENT) $(GENIE) --to=../.build/xcode.darwin  --os=macosx --platform=universal $(GENIE_OPTIONS) xcode10

projgen-vs:
	$(SILENT) $(GENIE) --to=../.build/vs.windows  --os=windows $(GENIE_OPTIONS) vs2017


rebuild:
	$(SILENT) $(MAKE) -C .build/$(PROJECT_TYPE).$(OS) clean all

## 'release' rules default to $(OS)
release: projgen-$(OS) release-$(OS)

release-darwin: prepare-release
	$(SILENT) $(MAKE) -C .build/$(PROJECT_TYPE).darwin  clean all CC=x86_64-apple-darwin15-clang

release-linux: prepare-release
	$(SILENT) $(MAKE) -C .build/$(PROJECT_TYPE).linux   clean all

release-windows: prepare-release
	$(SILENT) $(MAKE) -C .build/$(PROJECT_TYPE).windows clean all CC=x86_64-w64-mingw32-gcc

release-freebsd: prepare-release
	$(SILENT) $(MAKE) -C .build/$(PROJECT_TYPE).freebsd clean all

prepare-release:
	$(SILENT) git checkout src/version.h
	$(GENIE) release
refresh:
	$(GENIE) refresh
