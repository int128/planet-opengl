CC=cc
CFLAGS=-Wall -pipe -DGL_SILENCE_DEPRECATION
LDLIBS=-framework GLUT -framework OpenGL

.PHONY: all clean

all: main

main: main.o glwrapper.o tentai.o draw.o

clean:
	-rm -f *.o gltentai
