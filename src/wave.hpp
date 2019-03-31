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

class Wave
{
public:
  // Functions
  Wave();
  ~Wave();
  void parse_header(const char *filename);
  void print_header_contents();
  
private:
  // Functions
  void init_header();
  void parse_read_byte_for_header(u_int32_t read_byte, std::string &current_string, int count);
  
  // Variables
  struct _header_template {
    std::string riff;          // 0-3
    u_int32_t   file_size;     // 4-7
    std::string file_type;     // 8-11
    std::string format;        // 12-15
    u_int32_t   format_length; // 16-19
    short       format_type;   // 20-21
    short       num_channels;  // 22-23
  };
  struct _header_template _header;

};

#endif /* wave_hpp */