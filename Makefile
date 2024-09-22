# Makefile
# for SDL:
#   follow https://wiki.libsdl.org/SDL2/Installation
#  clone somewhere else :)
#   git clone https://github.com/libsdl-org/SDL.git -b SDL2
#   cd SDL
#   mkdir build
#   cd build
#   ../configure
#   make
#   sudo make install

CC=gcc
CFLAGS=-I/includes
DEPS = common.h
OBJ = main.o init.o map.o player.o collision.o terrain.o exit.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

play: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -lm `sdl2-config --cflags --libs`

clean:
	rm -f *.o play