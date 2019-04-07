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

#define DATA_CHUNK_NAME "data"

Wave::Wave()
{
  Wave::init_header();
}

Wave::~Wave()
{
  // No allocs or pointers
}

void Wave::init_header()
{
  Wave::_header.file_size       = 0;
  Wave::_header.format_length   = 0;
  Wave::_header.format_type     = 0;
  Wave::_header.num_channels    = 0;
  Wave::_header.sample_rate     = 0;
  Wave::_header.byte_rate       = 0;
  Wave::_header.block_align     = 0;
  Wave::_header.bits_per_sample = 0;
  Wave::_header.data_chunk_size = 0;
  Wave::_header.data_marker     = "";
}

// http://truelogic.org/wordpress/2015/09/04/parsing-a-wav-file-in-c/
void Wave::parse_header(const char *read_file_name, const char *write_file_name)
{
  FILE *read_file = fopen(read_file_name, "r");
  FILE *write_file = fopen(write_file_name, "w+");
  u_int32_t read_byte;
  u_int64_t count = 0;
  std::string current_string = "";
  Wave::data_chunk_start_index = 0;
  Wave::filename = std::string(read_file_name);

  verify_file_open(read_file, read_file_name);
  verify_file_open(write_file, write_file_name);

  std::cout << "Parsing header from " << read_file_name << std::endl;
  while ((u_int32_t) (read_byte = getc(read_file)) != EOF) {
    // Append and process data only if not in data sub chunk
    if (Wave::_header.data_marker != DATA_CHUNK_NAME) {
      current_string.push_back(read_byte);
      _header.header_in_byte_form.push_back(read_byte);
      Wave::update_header_pointer(count, read_byte);
      Wave::parse_read_byte_from_header(count, current_string);

      // Set the starting index of the data chunk
      if (current_string.find(DATA_CHUNK_NAME) != std::string::npos && data_chunk_start_index == 0) {
        Wave::data_chunk_start_index = count;
      }
    }

    if (Wave::should_write_to_header(count, current_string)) {
      fprintf(write_file, "%c", read_byte);
    } else {
      break;
    }
    count++;
  }
  fclose(read_file);
}

/**
 * This is slightly confusing but:
 *   Write to the Wave::_header if we're still reading the contents of the header
 *   (first && part) OR if we just finished reading the header. It appears the header 
 *   has to be padded by a NULL byte, hence the count < Wave::data_chunk_start_index + 1
 */
bool Wave::should_write_to_header(const u_int64_t count, const std::string &current_string)
{
  return (Wave::_header.data_marker != DATA_CHUNK_NAME
    && current_string.find(DATA_CHUNK_NAME) == std::string::npos)
    || count < Wave::data_chunk_start_index + 1;
}

void Wave::write_song_to_file(const char *read_file_name, const char *write_file_name, const double start_timestamp)
{
  FILE *read_file = fopen(read_file_name, "r");
  FILE *write_file = fopen(write_file_name, "w+");
  u_int32_t read_byte;
  const u_int64_t start_index = Wave::_header.byte_rate * start_timestamp;
  u_int64_t count = 0;

  fseek(read_file, start_index, SEEK_SET);
  fseek(write_file, 0, SEEK_END);

  std::cout << "\nWriting to file, skipping the first " << start_timestamp << " seconds..." << std::endl;
  while ((read_byte = getc(read_file)) != EOF) {
    fprintf(write_file, "%c", read_byte);
    count++;
  }

  #if !defined(DEBUG)
  if (count != WAVE_HEADER_BYTES) {
    char input;
    const std::string warning = get_yellow_warning();
    std::cerr << warning << " Unable to write correctly formatted header to file" << std::endl;
    std::cerr << "This may be due to the file's header having more information than a standard .wav file. In this case, you should continue." << std::endl;
    std::cerr << warning << " Index of last written byte: " << count + start_index + Wave::data_chunk_start_index + 1 << std::endl;
    std::cout << "Continue? (y/n): ";
    std::cin >> input;
    if (input != 'y') {
      exit(EXIT_FAILURE);
    }
  } else {
    std::cout << "Successfully wrote the audio file" << std::endl;
  }
  #endif
}

void Wave::parse_read_byte_from_header(const int count, std::string &current_string)
{
  switch (count) {
    case 3:
      if (current_string != "RIFF") {
        display_error("File not using RIFF format");
      }
      Wave::_header.riff = current_string;
      current_string = "";
      break;
    case 4 ... 7:
      Wave::convert_little_endian_to_big_endian(FOUR_BYTE_CHUNK_SIZE, Wave::_header.file_size);
      current_string = "";
      break;
    case 11:
      Wave::_header.file_type = current_string;
      current_string = "";
      break;
    case 15:
      Wave::_header.format = current_string;
      current_string = "";
      break;
    case 16 ... 19:
      Wave::convert_little_endian_to_big_endian(FOUR_BYTE_CHUNK_SIZE, Wave::_header.format_length);
      current_string = "";
      break;
    case 20 ... 21:
      Wave::convert_little_endian_to_big_endian(TWO_BYTE_CHUNK_SIZE, Wave::_header.format_type);
      current_string = "";
      break;
    case 22 ... 23:
      Wave::convert_little_endian_to_big_endian(TWO_BYTE_CHUNK_SIZE, Wave::_header.num_channels);
      current_string = "";
      break;
    case 24 ... 27:
      Wave::convert_little_endian_to_big_endian(FOUR_BYTE_CHUNK_SIZE, Wave::_header.sample_rate);
      current_string = "";
      break;
    case 28 ... 31:
      Wave::convert_little_endian_to_big_endian(FOUR_BYTE_CHUNK_SIZE, Wave::_header.byte_rate);
      current_string = "";
      break;
    case 32 ... 33:
      Wave::convert_little_endian_to_big_endian(TWO_BYTE_CHUNK_SIZE, Wave::_header.block_align);
      current_string = "";
      break;
    case 34 ... 35:
      Wave::convert_little_endian_to_big_endian(TWO_BYTE_CHUNK_SIZE, Wave::_header.bits_per_sample);
      current_string = "";
      break;
    case 39:
      Wave::_header.data_marker = current_string;
      current_string = "";
    case 40 ... 43:
      Wave::convert_little_endian_to_big_endian(FOUR_BYTE_CHUNK_SIZE, Wave::_header.data_chunk_size);
      current_string = "";
      break;
  }
}

void Wave::update_header_pointer(const short count, const u_int32_t read_byte)
{
  Wave::_header_pointer.count = count;
  Wave::_header_pointer.read_byte = read_byte;
}

void Wave::convert_little_endian_to_big_endian(const short mod, u_int16_t &header_access)
{
  Wave::_header_pointer.read_byte <<= 8 * (Wave::_header_pointer.count % mod);
  header_access |= Wave::_header_pointer.read_byte;
}

void Wave::convert_little_endian_to_big_endian(const short mod, u_int32_t &header_access)
{
  Wave::_header_pointer.read_byte <<= 8 * (Wave::_header_pointer.count % mod);
  header_access |= Wave::_header_pointer.read_byte;
}

const void Wave::print_header_contents()
{
  printf("\n---- RIFF chunk descriptor ----\n");
  std::cout << "Riff Marker:         " << Wave::_header.riff << std::endl;
  std::cout << "File Size:           " << Wave::_header.file_size << " Bytes" << std::endl;
  std::cout << "                     " << Wave::_header.file_size / 1000.0 << "  KB" << std::endl;
  std::cout << "                     " << Wave::_header.file_size / (1000.0 * 1000.0) << "  MB" << std::endl;
  std::cout << "File Type:           " << Wave::_header.file_type << std::endl;

  printf("\n---- fmt sub-chunk ----\n");
  std::cout << "Format Marker:       " << Wave::_header.format << std::endl;
  std::cout << "Format Length:       " << Wave::_header.format_length << std::endl;
  std::cout << "Format Type:         " << Wave::_header.format_type << std::endl;
  std::cout << "Number of Channels:  " << Wave::_header.num_channels << std::endl;
  std::cout << "Sample Rate:         " << Wave::_header.sample_rate << " Hz" << std::endl;
  std::cout << "Byte Rate:           " << Wave::_header.byte_rate << std::endl;
  std::cout << "Block Align:         " << Wave::_header.block_align << std::endl;
  std::cout << "Bits Per Sample:     " << Wave::_header.bits_per_sample << std::endl;

  printf("\n---- data sub-chunk ----\n");
  std::cout << "Data Marker:         " << Wave::_header.data_marker << std::endl;
  std::cout << "Data Chunk Size:     " << Wave::_header.data_chunk_size << std::endl;
}

const std::string Wave::get_filename()
{
  return Wave::filename;
}

const struct WaveHeaderTemplate Wave::get_header()
{
  return Wave::_header;
}

const u_int16_t Wave::get_data_chunk_start_index()
{
  return Wave::data_chunk_start_index;
}
