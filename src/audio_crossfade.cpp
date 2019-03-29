//
//  audio_crossfade.cpp
//  AudioCrossfadeML
//
//  Created by Ryan Elliott on 3/28/19.
//  Copyright © 2019 Ryan Elliott. All rights reserved.
//

#include <bitset>
#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <string.h>

struct AACFileHeader
{

};

void verify_arguments(const std::vector<std::string> arguments)
{
  if (arguments.size() != 3) {
    std::cout << "Invalid number of arguments. ./audio_crossfade [ audio_file1 ] [ audio_file2 ]" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void get_file_data(const char *filename, std::vector<char> &file_contents) {
  FILE *fd = fopen(filename, "r");
  unsigned char read_byte;
  int count = 0;
  while ((char) (read_byte = getc(fd)) != EOF) {
    #if defined(DEBUG)
    std::bitset<8> x(read_byte);
    std::cout << x << " ";
    std::cout << std::hex << read_byte << std::endl;
    #endif
    file_contents.push_back(read_byte);
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

  std::vector<char> file_one_contents;
  get_file_data(argv[1], file_one_contents);

  return 0;
}
