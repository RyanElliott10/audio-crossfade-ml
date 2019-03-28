#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>

bool verify_arguments(std::vector<std::string> arguments)
{
  if (arguments.size() != 3) {
    std::cout << "Invalid number of arguments. ./audio_crossfade [ audio_file1 ] [ audio_file2 ]" << std::endl;
    return false;
  }
  return true;
}

int main(int argc, char *argv[])
{
  std::vector<std::string> arguments;
  for (int i = 0; i < argc; i++) {
    arguments.push_back(std::string(argv[i]));
  }

  if (!verify_arguments(arguments)) {
    return 0;
  }

  return 0;
}