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
#include <fcntl.h>

#include "../include/wave.hpp"
#include "../include/utils.hpp"

#define TEST_DATA_CHUNK_NAME "LIST"

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
  _header.file_size       = 0;
  _header.format_length   = 0;
  _header.format_type     = 0;
  _header.num_channels    = 0;
  _header.sample_rate     = 0;
  _header.byte_rate       = 0;
  _header.block_align     = 0;
  _header.bits_per_sample = 0;
  _header.data_chunk_size = 0;
  _header.data_marker     = "";
}

bool Wave::should_print_to_file(const u_int64_t count, const int data_chunk_start_index)
{
  if (_header.data_marker != TEST_DATA_CHUNK_NAME) {
    return true;
  }
  return ((count - data_chunk_start_index) < _header.byte_rate * 10);
}

// http://truelogic.org/wordpress/2015/09/04/parsing-a-wav-file-in-c/
void Wave::parse_header(const char *filename)
{
  FILE *fp = fopen(filename, "rw");
  FILE *fp2 = fopen("tmpname.wav", "w+");
  int data_chunk_start_index = 0;
  u_int32_t read_byte;
  u_int64_t count = 0;
  std::string current_string = "";

  if (fp == 0) {
    const std::string error = get_red_error();
    std::cerr << error << " Failed to open file " << filename << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "Reading file " << filename << std::endl;
  while ((u_int32_t) (read_byte = getc(fp)) != EOF) {
    // Append and process data only if not in data sub chunk
    if (_header.data_marker != TEST_DATA_CHUNK_NAME) {
      current_string.push_back(read_byte);
      update_header_pointer(count, read_byte);
      parse_read_byte_from_header(count, current_string);

      if (current_string.find(TEST_DATA_CHUNK_NAME) && data_chunk_start_index == 0) {
        data_chunk_start_index = count;
      }
    }

    if (should_print_to_file(count, data_chunk_start_index)) {
      fprintf(fp2, "%c", read_byte);
    } else {
      printf("Seeking to end\n");
      fseek(fp, 0, SEEK_END);
    }

    count++;
  }

  fclose(fp);
  #if !defined(DEBUG)
  if (count != WAVE_HEADER_BYTES) {
    const std::string error = get_red_error();
    std::cerr << error << " Unable to parse header for file " << filename << std::endl;
    exit(EXIT_FAILURE);
  }
  #endif

  print_header_contents();
}

void Wave::parse_read_byte_from_header(const int count, std::string &current_string)
{
  switch (count) {
    case 3:
      if (current_string != "RIFF") {
        const std::string error = get_red_error();
        std::cerr << error << " File not using RIFF format " << std::endl;
        exit(EXIT_FAILURE);
      }
      _header.riff = current_string;
      current_string = "";
      break;
    case 4 ... 7:
      convert_little_endian_to_big_endian(FOUR_BYTE_CHUNK_SIZE, _header.file_size);
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
      convert_little_endian_to_big_endian(FOUR_BYTE_CHUNK_SIZE, _header.format_length);
      current_string = "";
      break;
    case 20 ... 21:
      convert_little_endian_to_big_endian(TWO_BYTE_CHUNK_SIZE, _header.format_type);
      current_string = "";
      break;
    case 22 ... 23:
      convert_little_endian_to_big_endian(TWO_BYTE_CHUNK_SIZE, _header.num_channels);
      current_string = "";
      break;
    case 24 ... 27:
      convert_little_endian_to_big_endian(FOUR_BYTE_CHUNK_SIZE, _header.sample_rate);
      current_string = "";
      break;
    case 28 ... 31:
      convert_little_endian_to_big_endian(FOUR_BYTE_CHUNK_SIZE, _header.byte_rate);
      current_string = "";
      break;
    case 32 ... 33:
      convert_little_endian_to_big_endian(TWO_BYTE_CHUNK_SIZE, _header.block_align);
      current_string = "";
      break;
    case 34 ... 35:
      convert_little_endian_to_big_endian(TWO_BYTE_CHUNK_SIZE, _header.bits_per_sample);
      current_string = "";
      break;
    case 39:
      _header.data_marker = current_string;
      current_string = "";
    case 40 ... 43:
      convert_little_endian_to_big_endian(FOUR_BYTE_CHUNK_SIZE, _header.data_chunk_size);
      current_string = "";
      break;
  }
}

void Wave::update_header_pointer(const short count, const u_int32_t read_byte)
{
  _header_pointer.count = count;
  _header_pointer.read_byte = read_byte;
}

void Wave::convert_little_endian_to_big_endian(const short mod, u_int16_t &header_access)
{
  _header_pointer.read_byte <<= 8 * (_header_pointer.count % mod);
  header_access |= _header_pointer.read_byte;
}

void Wave::convert_little_endian_to_big_endian(const short mod, u_int32_t &header_access)
{
  _header_pointer.read_byte <<= 8 * (_header_pointer.count % mod);
  header_access |= _header_pointer.read_byte;
}

void Wave::print_header_contents()
{
  printf("\n----RIFF chunk descriptor----\n");
  std::cout << "Riff Marker:         " << _header.riff << std::endl;
  std::cout << "File Size:           " << _header.file_size << " Bytes" << std::endl;
  std::cout << "                     " << _header.file_size / 1024.0 << "  KB" << std::endl;
  std::cout << "                     " << _header.file_size / (1024.0 * 1024) << "  MB" << std::endl;
  std::cout << "File Type:           " << _header.file_type << std::endl;

  printf("\n----fmt sub-chunk----\n");
  std::cout << "Format Marker:       " << _header.format << std::endl;
  std::cout << "Format Length:       " << _header.format_length << std::endl;
  std::cout << "Format Type:         " << _header.format_type << std::endl;
  std::cout << "Number of Channels:  " << _header.num_channels << std::endl;
  std::cout << "Sample Rate:         " << _header.sample_rate << " Hz" << std::endl;
  std::cout << "Byte Rate:           " << _header.byte_rate << std::endl;
  std::cout << "Block Align:         " << _header.block_align << std::endl;
  std::cout << "Bits Per Sample:     " << _header.bits_per_sample << std::endl;

  printf("\n----data sub-chunk----\n");
  std::cout << "Data Marker:         " << _header.data_marker << std::endl;
  std::cout << "Data Chunk Size:     " << _header.data_chunk_size << std::endl;
}
