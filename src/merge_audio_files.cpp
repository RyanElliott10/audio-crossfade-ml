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

#define OUTPUT_FILENAME "test_output_file.wav"
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

  // This should be deleted, only for testing purposes
  write_file_header(skipping_from_file.get_header());
}

void MergeAudioFiles::write_file_header(struct WaveHeaderTemplate header_to_write)
{
  FILE *fp = fopen(OUTPUT_FILENAME, "w+");

  for (int i = 0; i < header_to_write.header_in_byte_form.length(); i++) {
    fprintf(fp, "%c", header_to_write.header_in_byte_form[i]);
  }
}
