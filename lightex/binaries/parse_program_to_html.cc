#include <fstream>
#include <iostream>
#include <string>

#include <lightex/lightex.h>

int main(int argc, char** argv) {
  char const* input_file;
  char const* output_file;
  if (argc == 3) {
    input_file = argv[1];
    output_file = argv[2];
  } else {
    std::cerr << "Error: invalid number of arguments! (Expected 2, got " << std::to_string(argc - 1) << ")"
              << std::endl;
    return 1;
  }

  std::ifstream in(input_file);
  if (!in) {
    std::cerr << "Error: failed to open input file for reading: " << input_file << std::endl;
    return 1;
  }

  std::string storage;
  in.unsetf(std::ios::skipws);  // Avoids white space skipping.
  std::copy(std::istream_iterator<char>(in), std::istream_iterator<char>(), std::back_inserter(storage));
  in.close();

  std::string error_message;
  std::string result;
  if (!lightex::ParseProgramToHtml(storage, &error_message, &result)) {
    std::cerr << "Error: failed to parse input!" << std::endl;
    std::cerr << error_message << std::endl;
    return 1;
  }

  std::ofstream out(output_file);
  if (!out) {
    std::cerr << "Error: failed to open output file for writing: " << input_file << std::endl;
    return 1;
  }
  out << result;
  out.close();

  return 0;
}