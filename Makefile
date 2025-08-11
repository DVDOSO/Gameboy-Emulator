load: constants.o cartridge.o
	g++ constants.o cartridge.o -o load

constants.o: constants.cpp
	g++ -c constants.cpp

cartridge.o: cartridge.cpp
	g++ -c cartridge.cpp

.PHONY: clean
clean:
	del *.o load.exe