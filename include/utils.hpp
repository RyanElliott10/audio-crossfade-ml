//
//  utils.hpp
//  audio_crossfade
//
//  Created by Ryan Elliott on 3/30/19.
//  Copyright © 2018 Ryan Elliott. All rights reserved.
//

#ifndef utils_hpp
#define utils_hpp

std::string get_red_error();
std::string get_yellow_warning();
void verify_file_open(const FILE *fp, const char *filename);
void display_error(const char *message);

#endif