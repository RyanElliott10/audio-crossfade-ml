CC = clang++
CXXFLAGS = -Wall -Werror -std=c++11
OBJS = audio_crossfade.o wave.o audio_file_merger.o utils.o
MAIN = audio_crossfade

all: audio_crossfade
release: audio_crossfade

debug: CXXFLAGS += -DDEBUG
debug: audio_crossfade

audio_crossfade : $(OBJS)
	$(CC) $(OBJS) $(BFLAGS) -o $(MAIN)

audio_crossfade.o : audio_crossfade.cpp ../include/utils.hpp
	$(CC) $(CXXFLAGS) -c audio_crossfade.cpp

wave.o : wave.cpp ../include/wave.hpp ../include/header_structs.hpp ../include/utils.hpp
	$(CC) $(CXXFLAGS) -c wave.cpp

audio_file_merger.o : audio_file_merger.cpp ../include/audio_file_merger.hpp ../include/header_structs.hpp ../include/utils.hpp
	$(CC) $(CXXFLAGS) -c audio_file_merger.cpp

utils.o : utils.cpp ../include/utils.hpp
	$(CC) $(CXXFLAGS) -c utils.cpp
	
clean :
	rm *.o $(MAIN)
