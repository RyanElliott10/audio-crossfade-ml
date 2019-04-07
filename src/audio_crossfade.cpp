//
//  audio_crossfade.cpp
//  AudioCrossfadeML
//
//  Created by Ryan Elliott on 3/28/19.
//  Copyright © 2019 Ryan Elliott. All rights reserved.
//

#include <bitset>
#include <cstdio>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <string.h>

#include "../include/wave.hpp"
#include "../include/utils.hpp"
#include "../include/merge_audio_files.hpp"

#define EXPECT_NUM_ARGUMENTS 6

void verify_arguments(const std::vector<std::string> arguments)
{
  if (arguments.size() != EXPECT_NUM_ARGUMENTS) {
    const std::string error = get_red_error();
    std::cerr << error << " Invalid number of arguments. ./audio_crossfade [ audio_file1 ] [ audio_file2 ] [ timestamp1 ] [ timestamp2 ] [ crossfade_duration ]" << std::endl;
    exit(EXIT_FAILURE);
  }
}

Wave parse_file(std::string read_file, std::string write_file)
{
  std::cout << "===== Handling file " << write_file << " =====" << std::endl;
  Wave wave_file;
  wave_file.parse_header(read_file.c_str(), write_file.c_str());
  wave_file.write_song_to_file(read_file.c_str(), write_file.c_str());
  // wave_file.print_header_contents();
  return wave_file;
}

int main(int argc, char *argv[])
{
  std::vector<std::string> arguments;
  for (int i = 0; i < argc; i++) {
    arguments.push_back(std::string(argv[i]));
  }

  const double from_timestamp = std::stoi(arguments[3]);
  const double to_timestamp = std::stoi(arguments[4]);

  #if !defined(DEBUG)
  verify_arguments(arguments);
  #endif

  std::size_t index = std::string(arguments[1]).find_last_of("/");
  const std::string write_file1 = std::string("../output/").append(arguments[1].substr(index + 1));
  index = std::string(arguments[2]).find_last_of("/");
  const std::string write_file2 = std::string("../output/").append(arguments[2].substr(index + 1));

  Wave wave_file1 = parse_file(arguments[1], write_file1);
  std::cout << std::endl << std::endl;
  Wave wave_file2 = parse_file(arguments[2], write_file2);

  MergeAudioFiles merger;
  merger.merge_two_files(wave_file1, wave_file2, from_timestamp, to_timestamp);
  
  return 0;
}
