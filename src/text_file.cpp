#include "text_file.h"

#include <fstream>
#include <sstream>

std::string ReadText(const std::string& path) {
  std::ifstream file;
  file.open(path);

  if(!file) {
    throw std::runtime_error(std::string("Failed to open text file: ") + path);
  }

  std::ostringstream source_stream;
  source_stream << file.rdbuf();
  file.close();

  std::string source_string = source_stream.str();

  return source_string;
};