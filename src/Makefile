CC = clang++
CXXFLAGS = -pedantic -Wall -Werror -std=c++11 -DDEBUG
OBJS = main.o wave.o
MAIN = audio_crossfade

audio_crossfade : $(OBJS)
	$(CC) $(OBJS) $(BFLAGS) -o $(MAIN)

main.o : main.cpp
	$(CC) $(CXXFLAGS) -c main.cpp

wave.o : wave.cpp wave.hpp
	$(CC) $(CXXFLAGS) -c wave.cpp
	
clean :
	rm *.o $(MAIN)
