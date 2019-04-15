//
//  utils.cpp
//  audio_crossfade
//
//  Created by Ryan Elliott on 3/30/19.
//  Copyright © 2018 Ryan Elliott. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

#include "../include/utils.hpp"

std::string get_red_error()
{
  return "\033[1;31merror:\033[0m";
}

std::string get_yellow_warning()
{   
  return "\033[0;33mwarning:\033[0m";
}

void verify_file_open(const FILE *fp, const char *filename)
{
  if (fp == 0) {
    const std::string error = get_red_error();
    std::cerr << error << " Failed to open file " << filename << std::endl;
    exit(EXIT_FAILURE);
  }
} 

void display_error(const char *message)
{
  const std::string error = get_red_error();
  std::cerr << error << " " << message << " " << std::endl;
  exit(EXIT_FAILURE);
}
