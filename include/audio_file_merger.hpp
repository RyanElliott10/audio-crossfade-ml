//
//  audio_file_merger.hpp
//  audio_crossfade
//
//  Created by Ryan Elliott on 4/2/19.
//  Copyright © 2018 Ryan Elliott. All rights reserved.
//

#ifndef audio_file_merger_hpp
#define audio_file_merger_hpp

#include <tuple>

#include "header_structs.hpp"
#include "../include/wave.hpp"

class AudioFileMerger
{
public:
  AudioFileMerger();
  ~AudioFileMerger();
  void merge_two_files(Wave skipping_from_file, Wave skipping_to_file,
                       const double from_timestamp, const double to_timestamp,
                       const double crossfade_duration);

private:
  void intertwine_audio_files(Wave skipping_from_file, Wave skipping_to_file,
                              const double from_timestamp, const double to_timestamp,
                              const double crossfade_duration);
  void write_first_song(Wave skipping_from_file, const double from_timestamp);
  void write_second_song(Wave skipping_to_file, const double to_timestamp);
  void write_file_header(struct WaveHeaderTemplate header_to_write);
  bool which_byte(const u_int64_t crossfade_count, const u_int64_t alternate_rate);
  std::tuple<FILE *, u_int64_t> get_file_pointer_for_intertwine(Wave file, const double timestamp, const bool to_file=false);

  FILE *merged_file;
};

#endif /* audio_file_merger_hpp */
