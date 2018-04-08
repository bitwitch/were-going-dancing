game:
	clang main.c -o dance `sdl2-config --libs --cflags` -I include -L lib -l SDL2-2.0.0
