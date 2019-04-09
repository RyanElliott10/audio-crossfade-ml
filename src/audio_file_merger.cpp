//
//  audio_file_merger.cpp
//  audio_crossfade
//
//  Created by Ryan Elliott on 4/2/19.
//  Copyright © 2018 Ryan Elliott. All rights reserved.
//

#include <tuple>
#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <fcntl.h>
#include <iostream>

#include "../include/audio_file_merger.hpp"
#include "../include/utils.hpp"
#include "../include/wave.hpp"

#define OUTPUT_FILENAME "../output/test_output_file.wav"
#define WAVE_HEADER_TEMPLATE_NUM_MEMBERS 13

AudioFileMerger::AudioFileMerger(Wave skipping_from_file_, Wave skipping_to_file_,
                                 const double from_timestamp_, const double to_timestamp_,
                                 const double crossfade_duration_, const bool should_output_crossfade_)
{
  AudioFileMerger::skipping_from_file = skipping_from_file_;
  AudioFileMerger::skipping_to_file = skipping_to_file_;
  AudioFileMerger::from_timestamp = from_timestamp_;
  AudioFileMerger::to_timestamp = to_timestamp_;
  AudioFileMerger::crossfade_duration = crossfade_duration_;
  AudioFileMerger::should_output_crossfade = should_output_crossfade_;
}

AudioFileMerger::~AudioFileMerger()
{
  // No allocs or pointers
}

void AudioFileMerger::merge_two_files()
{
  std::cout << "\nSkipping from file " << AudioFileMerger::skipping_from_file.get_filename() << " at "
    << AudioFileMerger::from_timestamp << " seconds" << std::endl;
  std::cout << "Skipping from file " << AudioFileMerger::skipping_to_file.get_filename() << " at "
    << AudioFileMerger::to_timestamp << " seconds" << std::endl;

  /**
   * Write the base header. All the information in here will be wrong because, well,
   * we're using one header for two different songs...
   */
  AudioFileMerger::write_file_header(AudioFileMerger::skipping_from_file.get_header());
  AudioFileMerger::intertwine_audio_files();

  // AudioFileMerger::write_first_song();
  // AudioFileMerger::write_second_song();
}

void AudioFileMerger::write_file_header(struct WaveHeaderTemplate header_to_write)
{
  AudioFileMerger::merged_file = fopen(OUTPUT_FILENAME, "w+");
  for (int i = 0; i < header_to_write.header_in_byte_form.length(); i++) {
    fprintf(AudioFileMerger::merged_file, "%c", header_to_write.header_in_byte_form[i]);
  }
}

/* Smooth skip between songs */
void AudioFileMerger::intertwine_audio_files()
{
  u_int32_t from_read_byte, to_read_byte;
  // u_int64_t 
  const std::tuple<FILE *, u_int64_t> from_file_data = AudioFileMerger::get_file_pointer_for_intertwine(AudioFileMerger::skipping_from_file, AudioFileMerger::from_timestamp);
  const std::tuple<FILE *, u_int64_t> to_file_data = AudioFileMerger::get_file_pointer_for_intertwine(AudioFileMerger::skipping_to_file, AudioFileMerger::to_timestamp, true);
  FILE *from_file = std::get<0>(from_file_data);
  FILE *to_file = std::get<0>(to_file_data);
  const u_int64_t from_index = std::get<1>(from_file_data);
  u_int64_t count = 0;
  u_int64_t crossfade_count = 0;
  FILE *crossfade_file;

  if (AudioFileMerger::should_output_crossfade) {
    crossfade_file = fopen("crossfade_contents", "w+");
  }

  /**
   * POA:
   * Read from 'from_file' until you hit the timestamp. Then, begin reading from 'to_file', while STILL reading from 
   * 'from_file'. Interweave the audio files, and then after the duration is up, just read from 'to_file'
   */

  /**
   * Many issues here:
   * Using the from file to write the entire new file, so it'll be cut off before finishing since
   *   the new file will likely be larger than the old from file
   */
  std::cout << "\nWriting first part of first file..." << std::endl;
  while ((from_read_byte = getc(from_file)) != EOF) {
    if (count >= from_index) {
      // UNSAFE, not chechking for EOF. Fix later
      to_read_byte = getc(to_file);

      // If we're in the "crossfade" portion
      if (crossfade_count < (crossfade_duration * AudioFileMerger::skipping_from_file.get_header().byte_rate)) {
        if (crossfade_count == 0) {
          std::cout << "Adding crossfade..." << std::endl;
        }
        // Alternate the bytes being written
        const u_int32_t byte_to_be_written = AudioFileMerger::which_byte(crossfade_count, 4) ? to_read_byte : from_read_byte;
        if (!AudioFileMerger::should_output_crossfade) {
          fprintf(AudioFileMerger::merged_file, "%c", byte_to_be_written);
        } else {
          fprintf(crossfade_file, "%c", byte_to_be_written);
        }
        crossfade_count++;
      } else {
        // Otherwise, write the to file since we're past the crossfade portion
        if (AudioFileMerger::should_output_crossfade) {
          fprintf(AudioFileMerger::merged_file, "%c", to_read_byte);
      }
      }
    } else {
      // If not at or past the first timestamp, normally write to the file from first audio file
      if (AudioFileMerger::should_output_crossfade) {
        fprintf(AudioFileMerger::merged_file, "%c", to_read_byte);
      }
    }
    count++;
  }
}

bool AudioFileMerger::which_byte(const u_int64_t crossfade_count, const u_int64_t alternate_rate)
{
  return crossfade_count % alternate_rate < (alternate_rate / 2);
}

std::tuple<FILE *, u_int64_t> AudioFileMerger::get_file_pointer_for_intertwine(Wave file, const double timestamp, const bool to_file)
{
  const u_int64_t data_start_index = file.get_data_chunk_start_index();
  const u_int64_t byte_rate = file.get_header().byte_rate;
  const u_int64_t crossfade_start_index = byte_rate * timestamp;

  FILE *read_file = fopen(file.get_filename().c_str(), "r");
  fseek(read_file, data_start_index, SEEK_SET);

  if (to_file) {
    fseek(read_file, crossfade_start_index, SEEK_CUR);
  }

  return std::make_tuple(read_file, crossfade_start_index);
}

/* Write to file with abrupt skip to next song */
void AudioFileMerger::write_first_song()
{
  const u_int64_t data_start_index = AudioFileMerger::skipping_from_file.get_data_chunk_start_index();
  const u_int64_t byte_rate = AudioFileMerger::skipping_from_file.get_header().byte_rate;
  const u_int64_t last_index = byte_rate * AudioFileMerger::from_timestamp;
  u_int64_t count = 0;
  u_int32_t read_byte;

  FILE *read_file = fopen(AudioFileMerger::skipping_from_file.get_filename().c_str(), "r");
  fseek(read_file, data_start_index, SEEK_SET);

  while ((read_byte = getc(read_file)) != EOF && count < last_index) {
    fprintf(AudioFileMerger::merged_file, "%c", read_byte);
    count++;
  }
}
  
void AudioFileMerger::write_second_song()
{
  const u_int64_t data_start_index = AudioFileMerger::skipping_to_file.get_data_chunk_start_index();
  const u_int64_t byte_rate = AudioFileMerger::skipping_to_file.get_header().byte_rate;
  const u_int64_t first_index = byte_rate * AudioFileMerger::to_timestamp;
  u_int32_t read_byte;

  FILE *read_file = fopen(AudioFileMerger::skipping_to_file.get_filename().c_str(), "r");
  fseek(read_file, data_start_index + first_index, SEEK_SET);

  while ((read_byte = getc(read_file)) != EOF) {
    fprintf(AudioFileMerger::AudioFileMerger::merged_file, "%c", read_byte);
  }
}
