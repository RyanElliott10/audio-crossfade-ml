# AudioCrossfadeML
A tool to effectively -- and smoothly -- crossfade audio

## Compiling
The make file allows for two build configuration: release and debug.

#### Release Version
Run `make`, `make release`, or `make all` to create a release version. This will ignore all `#if defined(DEBUG)` statements and run as a proper release.

#### Debug Version
Run `make debug` to create a debug version. This will catch all `#if defined(DEBUG)` statements and run with the correct debugging statements and checks.

## Running
To use, run
`./audio_crossfade [ audio_file1 ] [ audio_file2 ] [ timestamp1 ] [ timestamp2 ] [ crossfade_duration ]`.

To run using sample data, use:
`./audio_crossfade ../test_audio_files/5AM.wav ../test_audio_files/Roll\ Call.wav 20 40 12
`.
This will use the two provided sample songs where 5AM is being skipping from, and Roll Call is being skipped to. This will begin the crossfade at the 20th second in 5AM, and start Roll Call from the 40th second. The crossfade will last 12 seconds.

## General Plan
The file/data/bit processing and everything should be done in C or C++. Use Python for the machine learning part to take advantage of easy to use libraries, like NumPy and TensorFlow.

#### General Notes
* There are quite a few audio codecs that should be considered:
    * `.wav` (General purpose)
    * `.mp3` (General purpose)
    * `.aac` (Spotify)
    * `.ogg` (Apple Music)
    
    
#### Rough Outline
- [x] Read contents of a given audio track through the command line
- [x] Parse file headers (only supports `.wav` files right now)
- [x] Jump to location in file given by timestamps
- [x] Merge the two audio files (without smoothing)
- [x] Add audio intertwining
- [ ] Add audio smoothing


### Future Plans
#### Merge the two audio files supplied as arguments
   * Create two Wave objects, one for each file
   * Parse both headers, etc.
   * Write to a single file `crossfade_duration` seconds from the first file
   * In the first `crossfade_duration` seconds, alternate bytes from the files
      * This will cause only `crossfade_duration` / 2 to be written from each file, but this is just a proof of concept
   * Write the remainder of the second audio file to the destination file
