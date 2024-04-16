# YAopenvfD/Makefile: Helper to make binary
# Copyright (C) 2023 Guoxin "7Ji" Pu <pugokushin@gmail.com>

# This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

# You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.


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
	VERSION:=$(shell bash scripts/version.sh)
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
	mkdir -p $(DIR_OBJECT)