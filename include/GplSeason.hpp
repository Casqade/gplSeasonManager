#pragma once

#include <helpers.hpp>
#include <ParseResult.hpp>

#include <string>
#include <vector>
#include <map>


struct GplSeason
{
  std::string name {"Unnamed season"};

  size_t year {2023};
  size_t numEvents {};

  std::map <std::string, bool> processedKeys
  {
    {"name", false},
    {"year", false},
    {"numEvents", false},
  };


  bool complete() const;
  std::vector <std::string> serialize() const;

  ParseResult parse(
    const std::string& key,
    const std::string& value );
};


bool
GplSeason::complete() const
{
  for ( auto&& [key, processed] : processedKeys )
    if ( processed == false )
      return false;

  return true;
}

std::vector <std::string>
GplSeason::serialize() const
{
  return
  {
    "name=" + name,
    "year=" + std::to_string(year),
    "numEvents=" + std::to_string(numEvents),
  };
}

ParseResult
GplSeason::parse(
  const std::string& key,
  const std::string& value )
{
  if ( processedKeys.count(key) == 0 )
    return {false, "Unknown key '" + key + "'"};


  if ( key == "name" )
    name = value;

  else if ( key == "year" )
  {
    if ( stringToNumber(value, year) == false )
      return {false, "'" + value + "' is not a valid integer"};
  }

  else if ( key == "numEvents" )
  {
    if ( stringToNumber(value, numEvents) == false )
      return {false, "'" + value + "' is not a valid integer"};
  }

  processedKeys[key] = true;
  return {true};
}
