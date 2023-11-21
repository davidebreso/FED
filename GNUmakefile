################ FED makefile for GNU make ################

.PHONY: all default clean zip badtarget

################ what version to build? ################

ifdef FEDTARGET
TARGET = $(FEDTARGET)
else
ifdef MSVCDIR
TARGET = win
else
ifdef DJGPP
TARGET = djgpp
else
ifdef WATCOM
TARGET = watcom
else
TARGET = curses
endif
endif
endif
endif

################ set the platform defines ################

ifeq ($(TARGET),djgpp)
fed$(EXE): CFLAGS += -DTARGET_DJGPP
fed$(EXE): LDFLAGS +=
else
ifeq ($(TARGET),win)
fed$(EXE): CFLAGS += -DTARGET_WIN
fed$(EXE): LDFLAGS += user32.lib gdi32.lib shell32.lib winmm.lib advapi32.lib
else
ifeq ($(TARGET),curses)
fed$(EXE): CFLAGS += -DTARGET_CURSES
ifdef DJGPP
fed$(EXE): LDFLAGS += -lcurso
else
fed$(EXE): LDFLAGS += -lncurses
endif
else
ifeq ($(TARGET),alleg)
fed$(EXE): CFLAGS += -DTARGET_ALLEG
fed$(EXE): LDFLAGS += -lalleg
else
ifeq ($(TARGET),watcom)
fed$(EXE): CFLAGS += -dTARGET_WATCOM
fed$(EXE): LDFLAGS +=
else
badtarget:
	@echo Unknown compile target $(TARGET)! (expecting djgpp, curses, msvc, or alleg)
endif
endif
endif
endif
endif

################ choose compiler switches ################

ifeq ($(TARGET),win)

CC = cl
EXEO = -Fe
OBJO = -Fo

CFLAGS = -nologo -W3 -WX -Gd -Ox -GB -MT
LDFLAGS = -nologo

ifdef DEBUGMODE
CFLAGS += -Zi
LDFLAGS += -Zi
endif

else
ifeq ($(TARGET),watcom)

CC = wcl386
EXEO = -fe=
OBJO = -fo=

CFLAGS = -s -bcl=dos4g -dTARGET_WATCOM
LDFLAGS = -s -bcl=dos4g

else

CC = gcc
EXEO = -o # trailing space
OBJO = -o # trailing space

ifdef DEBUGMODE
CFLAGS = -g
LDFLAGS =
else
CFLAGS = -Wall -O3 -fomit-frame-pointer
LDFLAGS = -s
endif

endif

endif

################ choose file extensions ################

ifeq ($(TARGET),win)
MXE = .exe
EXE = .exe
else
ifdef DJGPP
MXE = .exe
EXE = .exe
else
ifeq ($(TARGET),watcom)
MXE =
EXE = .exe
else
MXE =
EXE =
endif
endif
endif

ifeq ($(TARGET),win)
OBJ = obj
else
OBJ = o
endif

################ list of what to build ################

SRCS = buffer.c \
       config.c \
       dialog.c \
       disp.c \
       fed.c \
       gui.c \
       help.c \
       kill.c \
       line.c \
       menu.c \
       misc.c \
       search.c \
       tetris.c \
       util.c \
       io$(TARGET).c

OBJS := $(SRCS:.c=.$(OBJ))

################ the actual build rules ################

default: wccfed$(EXE)

wccfed$(EXE) : $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(EXEO)$@

help.c: help.txt makehelp$(MXE)
	./makehelp$(MXE) help.txt help.c

makehelp$(MXE): makehelp.c
ifeq ($(TARGET),watcom)
	gcc -Wall -O3 -fomit-frame-pointer -g -o makehelp makehelp.c
else
	$(CC) $(CFLAGS) -g $(LDFLAGS) $(EXEO)makehelp$(MXE) makehelp.c
endif

%.$(OBJ) : %.c fed.h io.h io$(TARGET).h
	$(CC) $(CFLAGS) -c $< $(OBJO)$@

fed.res: fed.rc fed.ico wnd.ico
	rc -fofed.res fed.rc

clean : ; $(RM) $(OBJS) *.res *.pdb *.ilk help.c makehelp$(MXE)

################ build distribution zips ################

zip: default
ifeq ($(TARGET),win)
	cd ..
	rm -f fed/fed.zip fed/fed.mft
	zip -9 fed/fed.zip fed/fed.exe fed/readme.txt fed/COPYING fed/makefile fed/*.c fed/*.h fed/*.rc fed/*.ico fed/help.txt fed/fed.syn -x fed/help.c
	unzip -Z -1 fed/fed.zip > fed\fed.mft
	echo fed/fed.mft >> fed\fed.mft
	zip -9 fed/fed.zip fed/fed.mft
	cd fed
else
ifdef DJGPP
	cp $(DJDIR)/bin/cwsdpmi.* .
	cd ..
	rm -f fed/fed.zip fed/fed.mft
	zip -9 fed/fed.zip fed/fed.exe fed/readme.txt fed/COPYING fed/makefile fed/*.c fed/*.h fed/*.rc fed/*.ico fed/help.txt fed/fed.syn fed/cwsdpmi.* -x fed/help.c
	unzip -Z -1 fed/fed.zip > fed\fed.mft
	echo fed/fed.mft >> fed\fed.mft
	zip -9 fed/fed.zip fed/fed.mft
	cd fed
	rm -f cwsdpmi.*
else
	rm -f fed.tar.gz fed.mft
	cd .. ; tar -c -f fed/fed.tar fed/fed fed/readme.txt fed/COPYING fed/makefile fed/*.c fed/*.h fed/*.rc fed/*.ico fed/help.txt fed/fed.syn --exclude fed/help.c
	tar -t -f fed.tar > fed.mft
	echo fed/fed.mft >> fed.mft
	cd .. ; tar -r -f fed/fed.tar fed/fed.mft
	gzip fed.tar
endif
endif
