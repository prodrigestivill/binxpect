CC?=gcc
STRIP?=strip
DESTDIR?=/
PREFIX?=/usr
CFLAGS=-Wall
LDFLAGS=

BINDIR=${DESTDIR}${PREFIX}/bin

.PHONY: all clean

all: binxpect binxpect-ns

binxpect: binxpect.o speeds.o serial.o
	${CC} binxpect.o serial.o speeds.o -o binxpect

binxpect-ns: binxpect.o speeds.o noserial.o
	${CC} binxpect.o noserial.o speeds.o -o binxpect-ns

clean:
	rm -f binxpect.o speeds.o serial.o noserial.o binxpect binxpect-ns

install: all
	mkdir -p ${BINDIR}
	install -m 755 binxpect ${BINDIR}/binxpect
	install -m 755 binxpect-ns ${BINDIR}/binxpect-ns
