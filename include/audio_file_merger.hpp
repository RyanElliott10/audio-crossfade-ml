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
  AudioFileMerger(Wave skipping_from_file, Wave skipping_to_file,
                  const double from_timestamp, const double to_timestamp,
                  const double crossfade_duration, const bool should_output_crossfade);
  ~AudioFileMerger();

  void merge_two_files();

private:
  void output_crossfade();
  void intertwine_audio_files();
  void write_first_song();
  void write_second_song();
  void write_file_header(struct WaveHeaderTemplate header_to_write);
  bool which_byte(const u_int64_t crossfade_count, const u_int64_t alternate_rate);
  std::tuple<FILE *, u_int64_t> get_file_pointer_for_intertwine(Wave file, const double timestamp, const bool to_file=false);
  bool should_continue_single_output(const u_int32_t from_read_byte,
                                     const u_int32_t to_read_byte,
                                     const u_int64_t crossfade_duration);

  FILE *merged_file;
  Wave skipping_from_file;
  Wave skipping_to_file;
  double from_timestamp;
  double to_timestamp;
  double crossfade_duration;
  bool should_output_crossfade;
};

#endif /* audio_file_merger_hpp */
