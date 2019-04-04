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

MergeAudioFiles::MergeAudioFiles()
{

}

MergeAudioFiles::~MergeAudioFiles()
{
  // No allocs or pointers
}
