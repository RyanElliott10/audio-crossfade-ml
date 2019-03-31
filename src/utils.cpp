//
//  utils.hpp
//  audio_crossfade
//
//  Created by Ryan Elliott on 3/30/19.
//  Copyright © 2018 Ryan Elliott. All rights reserved.
//

#include <string>

#include "utils.hpp"

std::string get_red_error()
{
  return "\033[1;31merror:\033[0m";
}

std::string get_yellow_warning()
{   
  return "\033[0;33mwarning:\033[0m";
}
