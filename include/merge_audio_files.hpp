//
//  merge_audio_files.hpp
//  audio_crossfade
//
//  Created by Ryan Elliott on 4/2/19.
//  Copyright © 2018 Ryan Elliott. All rights reserved.
//

#ifndef merge_audio_files_hpp
#define merge_audio_files_hpp

#include "header_structs.hpp"
#include "../include/wave.hpp"

class MergeAudioFiles
{
public:
  MergeAudioFiles();
  ~MergeAudioFiles();
  void merge_two_files(Wave skipping_from_file, Wave skipping_to_file, const double from_timestamp, const double to_timestamp);
  void write_file_header(struct WaveHeaderTemplate header_to_write);
  void write_first_song(Wave skipping_from_file, const double from_timestamp);
  void write_second_song(Wave skipping_to_file, const double to_timestamp);

private:
  FILE *merged_file;
};

#endif /* merge_audio_files_hpp */
