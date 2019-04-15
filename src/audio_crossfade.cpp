//
//  audio_crossfade.cpp
//  AudioCrossfadeML
//
//  Created by Ryan Elliott on 3/28/19.
//  Copyright © 2019 Ryan Elliott. All rights reserved.
//

#include <bitset>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <thread>
#include <vector>

#include "../include/wave.hpp"
#include "../include/utils.hpp"
#include "../include/audio_file_merger.hpp"

#define EXPECTED_NUM_ARGUMENTS 6

bool verify_arguments(const std::vector<std::string> arguments)
{
  if (arguments.size() < EXPECTED_NUM_ARGUMENTS) {
    const std::string error = get_red_error();
    std::cerr << error << " Invalid number of arguments. ./audio_crossfade [ audio_file1 ] [ audio_file2 ] [ timestamp1 ] [ timestamp2 ] [ crossfade_duration ]" << std::endl;
    exit(EXIT_FAILURE);
  } else if (arguments.size() > EXPECTED_NUM_ARGUMENTS) {
    // Change this to do it safely
    if (arguments[6] == "-o") {
      return true;
    }
  }
  return false;
}

void parse_file(const std::string read_file, const std::string write_file,
                const double start_timestamp, const bool should_output_crossfade,
                std::vector<Wave> &wave_files)
{
  std::cout << "Handling file " << write_file << std::endl;
  Wave wave_file;
  wave_file.parse_header(read_file.c_str(), write_file.c_str());
  if (!should_output_crossfade) {
    wave_file.write_song_to_file(read_file.c_str(), write_file.c_str(), start_timestamp);
  }

  #if defined(DEBUG)
  wave_file.print_header_contents();
  #endif
  wave_files.push_back(wave_file);
}

int main(int argc, char *argv[])
{
  std::vector<std::string> arguments;
  for (int i = 0; i < argc; i++) {
    arguments.push_back(std::string(argv[i]));
  }

  const double from_timestamp = std::stoi(arguments[3]);
  const double to_timestamp = std::stoi(arguments[4]);
  const double crossfade_duration = std::stoi(arguments[5]);
  const bool should_output_crossfade = verify_arguments(arguments);
  
  std::size_t index = std::string(arguments[1]).find_last_of("/");
  const std::string write_file1 = std::string("../output/").append(arguments[1].substr(index + 1));
  index = std::string(arguments[2]).find_last_of("/");
  const std::string write_file2 = std::string("../output/").append(arguments[2].substr(index + 1));
  std::vector<Wave> wave_files;

  /**
   * This isn't safe. We are assuming the first thread will finish first, but may 
   * not always be the case. Should add some logic here
   */
  // Use std::ref to pass a reference into a thread. By default, threads copy the data
  std::thread song_one_thread(parse_file, arguments[1], write_file1, from_timestamp, should_output_crossfade, std::ref(wave_files));
  std::thread song_two_thread(parse_file, arguments[2], write_file2, to_timestamp, should_output_crossfade, std::ref(wave_files));
  song_one_thread.join();
  song_two_thread.join();

  AudioFileMerger merger(wave_files[0], wave_files[1], from_timestamp, to_timestamp, crossfade_duration, should_output_crossfade);
  merger.merge_two_files();
  
  return 0;
}
