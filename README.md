# AudioCrossfadeML
A tool to effectively -- and smoothly -- crossfade audio

## Compiling
The make file allows for two build configuration: release and debug.

### Release Version
Run `make` or `make all` to create a release version. This will ignore all `#if defined(DEBUG)` statements and run as a proper release.

### Debug Version
Run `make debug` to create a debug version. This will catch all `#if defined(DEBUG)` statements and run with the correct debugging statements and checks.

## Running
To use, run `./audio_crossfade [ audio_file1 ] [ audio_file2 ] [ timestamp1 ] [ timestamp2 ] [ crossfade_duration ]`

## General Plan
The file/data/bit processing and everything should be done in C or C++. Use Python for the machine learning part to take advantage of easy to use libraries, like NumPy and TensorFlow.

### General Notes
* There are quite a few audio codecs that should be considered:
    * `.wav` (General purpose)
    * `.mp3` (General purpose)
    * `.aac` (Spotify)
    * `.ogg` (Apple Music)
    
    
### Rough Outline
- [x] Read contents of a given audio track through the command line
- [x] Parse file headers (only supports `.wav` files right now)
- [ ] Jump to location in file given by timestamps, and perform audio processing


### Future Plans
#### Merge the two audio files supplied as arguments
   * Create two Wave objects, one for each file
   * Parse both headers, etc.
   * Write to a single file `crossfade_duration` seconds from the first file
   * In the first `crossfade_duration` seconds, alternate bytes from the files
      * This will cause only `crossfade_duration` / 2 to be written from each file, but this is just a proof of concept
   * Write the remainder of the second audio file to the destination file
