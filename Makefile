BINARY_DAEMON = YAopenvfD
BINARY_GLYPH_TEST = glyph_test
BINARY ?= $(BINARY_DAEMON)
DIR_INCLUDE = include
DIR_SOURCE = src
DIR_OBJECT = obj
CC ?= gcc
STRIP ?= strip
CFLAGS = -I$(DIR_INCLUDE) -Wall -Wextra
STATIC ?= 0
DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CFLAGS += -g -DDEBUGGING
endif
ifeq ($(STATIC), 1)
	LDFLAGS += -static
endif

INCLUDES = $(wildcard $(DIR_INCLUDE)/*.h) $(wildcard $(DIR_INCLUDE)/glyphs/*.h) $(wildcard $(DIR_INCLUDE)/glyphs/char/*.h)

_OBJECTS_COMMON = $(wildcard $(DIR_SOURCE)/*.c)
_OBJECTS_COLLECTOR = $(wildcard $(DIR_SOURCE)/collector/*.c)
OBJECTS = $(patsubst $(DIR_SOURCE)/%.c,$(DIR_OBJECT)/%.o,$(_OBJECTS_COMMON)) $(patsubst $(DIR_SOURCE)/collector/%.c,$(DIR_OBJECT)/collector_%.o,$(_OBJECTS_COLLECTOR))

ifndef VERSION
	VERSION_GIT_TAG := $(shell git describe --abbrev=0 --tags ${TAG_COMMIT} 2>/dev/null || true)
	VERSION_GIT_TAG_NO_V := $(VERSION_GIT_TAG:v%=%)
	VERSION_GIT_COMMIT := $(shell git rev-list --abbrev-commit --tags --max-count=1)
	VERSION_GIT_DATE := $(shell git log -1 --format=%cd --date=format:"%Y%m%d")
	VERSION := $(VERSION_GIT_TAG_NO_V)-$(VERSION_GIT_COMMIT)-$(VERSION_GIT_DATE)
	GIT_STAT := $(shell git diff --stat)
	ifeq ($(VERSION),--)
		VERSION := unknown
	endif
	ifneq ($(GIT_STAT),)
		VERSION := $(VERSION)-DIRTY
	endif
endif

ifneq ($(BINARY), $(BINARY_GLYPH_TEST))
	BINARY := $(BINARY_DAEMON)
endif

.PHONY: clean prepare version fresh
# glyph_test: $(BINARY_GLYPH_TEST)

# OBJECTS_DAEMON_ONLY

# OBJECTS_GLYPHS_TEST_ONLY

$(BINARY_DAEMON): $(OBJECTS) | version
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
ifneq ($(DEBUG), 1)
	$(STRIP) $(BINARY_DAEMON)
endif

$(DIR_OBJECT)/version.o: $(DIR_SOURCE)/version.c $(INCLUDES) version | prepare
	$(CC) -c -o $@ $< $(CFLAGS) -DVERSION=\"$(VERSION)\"

$(DIR_OBJECT)/main.o: $(DIR_SOURCE)/main.c $(INCLUDES) | prepare
	$(CC) -c -o $@ $< $(CFLAGS) -DBINARY=YAopenvfD

$(DIR_OBJECT)/collector_%.o: $(DIR_SOURCE)/collector/%.c $(INCLUDES) | prepare
	$(CC) -c -o $@ $< $(CFLAGS)

$(DIR_OBJECT)/%.o: $(DIR_SOURCE)/%.c $(INCLUDES) | prepare
	$(CC) -c -o $@ $< $(CFLAGS)

version:

fresh: clean $(BINARY_DAEMON)

clean:
	rm -rf $(DIR_OBJECT) $(BINARY_DAEMON) $(BINARY_GLYPH_TEST)

prepare:
	$(shell [[ ! -d $(DIR_OBJECT) ]] && mkdir $(DIR_OBJECT))