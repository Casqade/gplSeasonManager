#pragma once

#include <helpers.hpp>
#include <ParseResult.hpp>

#include <vector>


struct GplTrack
{
  std::string name {};
  std::string shortname {};
  float length {};


  GplTrack() = default;


  ParseResult deserialize( const std::vector <std::string>& );
};


ParseResult
GplTrack::deserialize(
  const std::vector <std::string>& lines )
{
  if ( lines.empty() == true )
    return {false, "Empty 'track' section"};


  for ( const auto& line : lines )
  {
    if ( line.empty() == true )
      continue;

    const auto delimPos = line.find("=");

    if ( delimPos == std::string::npos )
      continue;

    const auto key = line.substr(0, delimPos);
    const auto value = line.substr(delimPos + 1);

    if ( key.empty() == true )
      return {false, "Key-value format violation"};

    if ( value.empty() == true )
      return {false, "Key-value format violation"};


    if ( key.find("track_name_short") != std::string::npos )
    {
      shortname = value.substr(value.find_first_not_of(' '));
      continue;
    }

    if ( key.find("track_name") != std::string::npos )
    {
      name = value.substr(value.find_first_not_of(' '));
      continue;
    }

    if ( key.find("track_length") != std::string::npos )
    {
      if ( stringToNumber(value, length) == false )
        return {false, "'" + value + "' is not a valid float"};

      continue;
    }

    continue;
  }

  return {};
}
