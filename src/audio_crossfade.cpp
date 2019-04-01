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

void verify_arguments(const std::vector<std::string> arguments)
{
  if (arguments.size() != 3) {
    const std::string error = get_red_error();
    std::cerr << error << " Invalid number of arguments. ./audio_crossfade [ audio_file1 ] [ audio_file2 ]" << std::endl;
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

  Wave wave_file;
  wave_file.parse_header(arguments[1].c_str());
  
  return 0;
}
