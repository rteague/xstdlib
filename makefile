################################################################
# Name: makefile
# Version: <version>
# Author: rashaudteague
# Date: 08/22/2012
# License: GNU LGPL <http://www.gnu.org/licenses/>
# Copy: Copyright (c) 2012 Maris Labs
# Description: Extension functions to the standard C Library
#################################################################

CC = gcc
CCFLAGS = -c -Wall -fpic
LIBFLAG = -shared
LINKS = -lm
INCLUDE_FILE = xstdlib.h
OBJECT_FILES = numbers.o strings.o file.o io.o os.o lists.o vector.o hash.o

all: libxstdlib.so

libxstdlib.so: $(OBJECT_FILES)
	$(CC) $(LIBFLAG) -o $@ $^ $(LINKS)

%.o: %.c
	$(CC) $(CCFLAGS) $< -o $@

%.c: $(INCLUDE_FILE)

.PHONY: clean install uninstall

install:
	cp libxstdlib.so /usr/lib && cp xstdlib.h /usr/include

uninstall:
	rm /usr/lib/libxstdlib.so && rm /usr/include/xstdlib.h

clean:
	rm *.o && rm *.so
