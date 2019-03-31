# audio-crossfade-ml
A tool to effectively -- and smoothly -- crossfade audio

@charlie
Right now, I'm thinking the file/data/bit processing and everything should be done in C or C++. We should probably use Python for the machine learning part to take advantage of easy to use libraries like NumPy and TensorFlow. Just my two cents, though.

### General Notes
* There are quite a few audio codecs that should be considered:
    * WAVE (General purpose)
    * MP3 (General purpose)
    * AAC (Spotify)
    * OGG (Apple Music)
    
    
### Rough Outline
* Read contents of a given audio track through the command line
* Do stuff
