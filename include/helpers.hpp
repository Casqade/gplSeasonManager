#pragma once

#include <filesystem>


bool
fileExists(
  const std::filesystem::path& path )
{
  const auto fileStatus = std::filesystem::status(path);

  return fileStatus.type() != std::filesystem::file_type::not_found;
}

template <typename T>
bool
stringToNumber(
  const std::string& str,
  T& number )
{
  std::stringstream stream {str};
  stream >> number;

  return stream.fail() == false;
}
