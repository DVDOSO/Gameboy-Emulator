SDL_INCLUDE = -I src/include
SDL_LIB     = -L src/lib
SDL_LIBS    = -lmingw32 -lSDL2main -lSDL2
CXXFLAGS    = -O2 $(SDL_INCLUDE)
# Static libstdc++/libgcc so the binary doesn't depend on whichever mingw
# runtime DLL happens to be first in PATH (self-contained, like the SDL2.dll).
LDFLAGS     = -static-libstdc++ -static-libgcc

SRC = src/main.cpp src/cpu.cpp src/memory.cpp src/timer.cpp src/ppu.cpp
OBJ = $(SRC:.cpp=.o)

load: $(OBJ)
	g++ $(LDFLAGS) $(SDL_LIB) $(OBJ) -o load $(SDL_LIBS)

%.o: %.cpp
	g++ -c $< -o $@ $(CXXFLAGS)

# Headless CPU conformance harness (SingleStepTests).
sst_test: tests/sst_test.cpp src/cpu.cpp src/memory.cpp src/timer.cpp
	g++ -O2 -DSST_TEST -I src tests/sst_test.cpp src/cpu.cpp src/memory.cpp src/timer.cpp -o sst_test

.PHONY: clean
clean:
	del /Q src\*.o load.exe sst_test.exe
