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

  
  // Variables
  struct _header_template {
    std::string riff;      // 0-3
    int file_size;         // 4-7
    std::string file_type; // 8-11
    std::string format;    // 12-15
    short num_channels;    // 22-23
  };
  struct _header_template _header;

};

#endif /* wave_hpp */