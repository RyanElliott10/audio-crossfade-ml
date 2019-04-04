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

#define EXPECT_NUM_ARGUMENTS 6

void verify_arguments(const std::vector<std::string> arguments)
{
  if (arguments.size() != EXPECT_NUM_ARGUMENTS) {
    const std::string error = get_red_error();
    std::cerr << error << " Invalid number of arguments. ./audio_crossfade [ audio_file1 ] [ audio_file2 ] [ timestamp1 ] [ timestamp2 ] [ crossfade_duration ]" << std::endl;
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[])
{
  std::vector<std::string> arguments;
  for (int i = 0; i < argc; i++) {
    arguments.push_back(std::string(argv[i]));
  }

  #if !defined(DEBUG)
  verify_arguments(arguments);
  #endif

  const char *write_file1 = "sample_audio_out1.wav";
  const char *write_file2 = "sample_audio_out2.wav";

  std::cout << "\n\n===== Handling file " << arguments[2] << " =====" << std::endl;
  Wave wave_file1;
  wave_file1.parse_header(arguments[1].c_str(), write_file1);
  wave_file1.write_song_to_file(arguments[1].c_str(), write_file1);
  wave_file1.print_header_contents();

  std::cout << "\n\n===== Handling file " << arguments[2] << " =====" << std::endl;
  Wave wave_file2;
  wave_file2.parse_header(arguments[2].c_str(), write_file2);
  wave_file2.write_song_to_file(arguments[2].c_str(), write_file2);
  wave_file2.print_header_contents();
  
  return 0;
}
