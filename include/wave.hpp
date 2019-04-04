//
//  wave.hpp
//  audio_crossfade
//
//  Created by Ryan Elliott on 3/30/19.
//  Copyright © 2018 Ryan Elliott. All rights reserved.
//

#ifndef wave_hpp
#define wave_hpp

#include <string>

#include "header_structs.hpp"

class Wave
{
public:
  Wave();
  ~Wave();
  void parse_header(const char *filename);
  void print_header_contents();
  
private:
  void init_header();
  bool should_write_to_header(const std::string &current_string, const u_int64_t count);
  void parse_read_byte_from_header(const int count, std::string &current_string);
  void update_header_pointer(const short count, const u_int32_t read_byte);
  void convert_little_endian_to_big_endian(const short mod, u_int16_t &header_access);
  void convert_little_endian_to_big_endian(const short mod, u_int32_t &header_access);
  void write_song_to_file(FILE *read_file, FILE *write_file);
  
  struct WaveHeaderTemplate _header;
  struct HeaderPointer _header_pointer;
  u_int16_t data_chunk_start_index;
};

#endif /* wave_hpp */