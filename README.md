# AudioCrossfadeML
A tool to effectively -- and smoothly -- crossfade audio

## Compiling
The make file allows for two build configuration: release and debug.

### Release Version
Run `make` to create a release version. This will ignore all `#if defined(DEBUG)` statements and run as a proper release.

### Debug Version
Run `make debug` to create a debug version. This will catch all `#if defined(DEBUG)` statements and run with the correct debug statements and checks.

## Running
To use, run `./audio_crossfade [ audio_file1 ] [ audio_file2 ] [ timestamp1 ] [ timestamp2 ]`

## Notes

@charlie
Right now, I'm thinking the file/data/bit processing and everything should be done in C or C++. We should probably use Python for the machine learning part to take advantage of easy to use libraries like NumPy and TensorFlow. Just my two cents, though.

### General Notes
* There are quite a few audio codecs that should be considered:
    * WAVE (General purpose)
    * MP3 (General purpose)
    * AAC (Spotify)
    * OGG (Apple Music)
    
    
### Rough Outline
[x] Read contents of a given audio track through the command line
[x] Parse file headers (only supports `.wav` files right now)
[] Jump to location in file given by timestamps, and perform audio processing
