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

Wave::Wave()
{
  init_header();
}

Wave::~Wave()
{
    // No allocs or pointers
}

void Wave::init_header()
{
  _header.file_size = 0;
}

// http://truelogic.org/wordpress/2015/09/04/parsing-a-wav-file-in-c/
void Wave::parse_header(const char *filename)
{
  FILE *fd = fopen(filename, "r");
  short count = 0;
  u_int32_t read_byte;
  std::string current_string = "";

  if (fd == 0) {
    std::cout << "Error: Failed to open file " << filename << std::endl;
    exit(EXIT_FAILURE);
  }

  while ((u_int32_t) (read_byte = getc(fd)) != EOF && count < 44) {
    // #if defined(DEBUG)
    // std::cout << std::hex << read_byte << std::endl;
    // #endif
    current_string.push_back(read_byte);
    parse_read_byte_for_header(read_byte, current_string, count);
    count++;
  }

  print_header_contents();
}

void Wave::parse_read_byte_for_header(u_int32_t read_byte, std::string &current_string, int count)
{
  switch (count) {
    case 3:
    _header.riff = current_string;
    current_string = "";
    break;
  case 4 ... 7:
    // Convert little endian to big endian 32 bit unsigned int
    read_byte <<= 8 * (count % 4);
    _header.file_size |= read_byte;
    current_string = "";
    break;
  case 11:
    _header.file_type = current_string;
    current_string = "";
    break;
  case 15:
    _header.format = current_string;
    current_string = "";
    break;
  case 16 ... 19:
    read_byte <<= 8 * (count % 4);
    _header.format_length |= read_byte;
    current_string = "";
    break;
  case 20 ... 21:
    read_byte <<= 8 * (count % 2);
    _header.format_type |= read_byte;
    current_string = "";
    break;
  case 22:
    _header.num_channels = read_byte;
    break;
    }
}

void Wave::print_header_contents()
{
  std::cout << "Riff:                " << _header.riff << std::endl;
  std::cout << "File Size:           " << _header.file_size << " Bytes" << std::endl;
  std::cout << "                     " << _header.file_size / 1024.0 << "  KB" << std::endl;
  std::cout << "                     " << _header.file_size / (1024.0 * 1024) << "  MB" << std::endl;
  std::cout << "File Type:           " << _header.file_type << std::endl;
  std::cout << "Format:              " << _header.format << std::endl;
  std::cout << "Format Length:       " << _header.format_length << std::endl;
  std::cout << "Format Type:         " << _header.format_type << std::endl;
  std::cout << "Number of Channels:  " << _header.num_channels << std::endl;
}
