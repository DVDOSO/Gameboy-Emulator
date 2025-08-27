SDL_INCLUDE_PATH = -I src/include
SDL_LIB_PATH = -L src/lib
SDL_LIBS = -lmingw32 -lSDL2main -lSDL2

load: ppu.o cpu.o
	g++ $(SDL_LIB_PATH) ppu.o cpu.o -o load $(SDL_LIBS)

ppu.o: ppu.cpp
	g++ -c ppu.cpp $(SDL_INCLUDE_PATH)

cpu.o: cpu.cpp
	g++ -c cpu.cpp $(SDL_INCLUDE_PATH)

.PHONY: clean
clean:
	del *.o load.exe