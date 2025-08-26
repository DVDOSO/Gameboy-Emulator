load: ppu.o cpu.o
	g++ ppu.o cpu.o -o load

ppu.o: ppu.cpp
	g++ -c ppu.cpp

cpu.o: cpu.cpp
	g++ -c cpu.cpp

.PHONY: clean
clean:
	del *.o load.exe