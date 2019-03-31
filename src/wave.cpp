//
//  wave.cpp
//  audio_crossfade
//
//  Created by Ryan Elliott on 3/30/19.
//  Copyright © 2018 Ryan Elliott. All rights reserved.
//

#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <iostream>

#include "wave.hpp"

Wave::Wave() {}

Wave::~Wave()
{
    // No allocs or pointers
}

void Wave::parse_header(const char *filename)
{
  FILE *fd = fopen(filename, "r");
  unsigned char read_byte;
  short count = 0;
  std::string current_string = "";


  while ((char) (read_byte = getc(fd)) != EOF && count < 44) {
    #if defined(DEBUG)
    // std::cout << std::hex << read_byte << std::endl;
    #endif
    current_string.push_back(read_byte);
    
    // Assign values to WAVFileHeader struct
    if (count == 3) {
      _header.riff = current_string;
      current_string = "";
    } else if (count == 7) {
    //   _header.file_size = current_string;
      current_string = "";
    } else if (count == 11) {
      _header.file_type = current_string;
      current_string = "";
    } else if (count == 15) {
      _header.format = current_string;
      current_string = "";
    } else if (count == 21) {
      current_string = "";
    } else if (count == 22) {
      _header.num_channels = read_byte;
    }
    count++;
  }

  print_header_contents();
}

void Wave::print_header_contents()
{
  std::cout << "Riff: " << _header.riff << std::endl;
  std::cout << "File Type: " << _header.file_size << std::endl;
  std::cout << "Format: " << _header.format << std::endl;
  std::cout << "Number of Channels: " << _header.num_channels << std::endl;
}
