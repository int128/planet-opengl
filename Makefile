CC=gcc
CFLAGS=-Wall -pipe -DWIN32
LDLIBS=-lglut32 -lglu32 -lopengl32 #-mno-cygwin

.PHONY: all clean

all: main

main: main.o glwrapper.o tentai.o draw.o

clean:
	-rm -f *.o gltentai
