CC = clang++
CXXFLAGS = -Wall -Werror -std=c++11
OBJS = audio_crossfade.o wave.o merge_audio_files.o utils.o
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

merge_audio_files.o : merge_audio_files.cpp ../include/merge_audio_files.hpp ../include/header_structs.hpp ../include/utils.hpp
	$(CC) $(CXXFLAGS) -c merge_audio_files.cpp

utils.o : utils.cpp ../include/utils.hpp
	$(CC) $(CXXFLAGS) -c utils.cpp
	
clean :
	rm *.o $(MAIN)
