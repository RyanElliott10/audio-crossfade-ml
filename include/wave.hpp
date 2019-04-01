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
  // Use pass by reference to avoid passing a billion chars each time this is called
  bool should_print_to_file(const u_int64_t count, const int data_chunk_start_index);
  void parse_read_byte_from_header(const int count, std::string &current_string);
  void update_header_pointer(const short count, const u_int32_t read_byte);
  void convert_little_endian_to_big_endian(const short mod, u_int16_t &header_access);
  void convert_little_endian_to_big_endian(const short mod, u_int32_t &header_access);
  
  struct WaveHeaderTemplate _header;
  struct HeaderPointer _header_pointer;
};

#endif /* wave_hpp */