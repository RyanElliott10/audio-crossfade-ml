//
//  merge_audio_files.cpp
//  audio_crossfade
//
//  Created by Ryan Elliott on 4/2/19.
//  Copyright © 2018 Ryan Elliott. All rights reserved.
//

#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <fcntl.h>

#include "../include/merge_audio_files.hpp"
#include "../include/utils.hpp"
#include "../include/wave.hpp"

#define OUTPUT_FILENAME "../output/test_output_file.wav"
#define WAVE_HEADER_TEMPLATE_NUM_MEMBERS 13

MergeAudioFiles::MergeAudioFiles()
{

}

MergeAudioFiles::~MergeAudioFiles()
{
  // No allocs or pointers
}

void MergeAudioFiles::merge_two_files(Wave skipping_from_file, Wave skipping_to_file, const double from_timestamp, const double to_timestamp)
{
  std::cout << "\nSkipping from file " << skipping_from_file.get_filename() << " at " << from_timestamp << " seconds" << std::endl;
  std::cout << "Skipping from file " << skipping_to_file.get_filename() << " at " << to_timestamp << " seconds" << std::endl;

  /**
   * Write the base header. All the information in here will be wrong because, well,
   * we're using one header for two different songs...
   */
  write_file_header(skipping_from_file.get_header());
  MergeAudioFiles::write_first_song(skipping_from_file, from_timestamp);
  MergeAudioFiles::write_second_song(skipping_to_file, to_timestamp);
}

void MergeAudioFiles::write_file_header(struct WaveHeaderTemplate header_to_write)
{
  MergeAudioFiles::merged_file = fopen(OUTPUT_FILENAME, "w+");
  for (int i = 0; i < header_to_write.header_in_byte_form.length(); i++) {
    fprintf(MergeAudioFiles::merged_file, "%c", header_to_write.header_in_byte_form[i]);
  }
}

void MergeAudioFiles::write_first_song(Wave skipping_from_file, const double from_timestamp)
{
  const u_int64_t data_start_index = skipping_from_file.get_data_chunk_start_index();
  const u_int64_t byte_rate = skipping_from_file.get_header().byte_rate;
  const u_int64_t last_index = byte_rate * from_timestamp;
  u_int64_t count = 0;
  u_int32_t read_byte;

  FILE *read_file = fopen(skipping_from_file.get_filename().c_str(), "r");
  fseek(read_file, data_start_index, SEEK_SET);

  while ((read_byte = getc(read_file)) != EOF && count < last_index) {
    fprintf(MergeAudioFiles::merged_file, "%c", read_byte);
    count++;
  }
}
  
void MergeAudioFiles::write_second_song(Wave skipping_to_file, const double to_timestamp)
{
  const u_int64_t data_start_index = skipping_to_file.get_data_chunk_start_index();
  const u_int64_t byte_rate = skipping_to_file.get_header().byte_rate;
  const u_int64_t first_index = byte_rate * to_timestamp;
  u_int32_t read_byte;

  FILE *read_file = fopen(skipping_to_file.get_filename().c_str(), "r");
  fseek(read_file, data_start_index + first_index, SEEK_SET);

  std::cout << skipping_to_file.get_header().file_size << std::endl;

  while ((read_byte = getc(read_file)) != EOF) {
    fprintf(MergeAudioFiles::MergeAudioFiles::merged_file, "%c", read_byte);
  }
}
