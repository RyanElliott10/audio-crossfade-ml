//
//  header_structs.hpp
//  audio_crossfade
//
//  Created by Ryan Elliott on 3/30/19.
//  Copyright © 2018 Ryan Elliott. All rights reserved.
//

#ifndef header_structs_hpp
#define header_structs_hpp

#include <string>

#define TWO_BYTE_CHUNK_SIZE 2
#define FOUR_BYTE_CHUNK_SIZE 4
#define WAVE_HEADER_BYTES 44

/**
 * Header contains 44 bytes, ranging from strings to
 * 2 byte integers to 4 byte ints. This header properly
 * sets up a correct header
 */
struct WaveHeaderTemplate
{
    std::string riff;            // 0-3
    u_int32_t   file_size;       // 4-7
    std::string file_type;       // 8-11
    std::string format;          // 12-15
    u_int32_t   format_length;   // 16-19
    u_int16_t   format_type;     // 20-21
    u_int16_t   num_channels;    // 22-23
    u_int32_t   sample_rate;     // 24-27
    u_int32_t   byte_rate;       // 28-31
    u_int16_t   block_align;     // 32-33
    u_int16_t   bits_per_sample; // 34-35
    std::string data_marker;     // 36-39
    u_int32_t   data_chunk_size; // 40-43
    std::string header_in_byte_form;
};

struct HeaderPointer
{
    int count;
    u_int32_t read_byte;
};

#endif
