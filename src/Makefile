CC = clang++
CXXFLAGS = -Wall -Werror -std=c++11
OBJS = audio_crossfade.o wave.o
MAIN = audio_crossfade

all: audio_crossfade

debug: CXXFLAGS += -DDEBUG
debug: audio_crossfade

audio_crossfade : $(OBJS)
	$(CC) $(OBJS) $(BFLAGS) -o $(MAIN)

audio_crossfade.o : audio_crossfade.cpp
	$(CC) $(CXXFLAGS) -c audio_crossfade.cpp

wave.o : wave.cpp wave.hpp
	$(CC) $(CXXFLAGS) -c wave.cpp
	
clean :
	rm *.o $(MAIN)
