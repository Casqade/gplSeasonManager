#pragma once

#include <helpers.hpp>
#include <ParseResult.hpp>

#include <string>
#include <vector>
#include <map>


struct GplEvent
{
  std::string name {"Custom Grand Prix"};
  std::string shortname {"Custom GP"};
  std::string trackDirectory {};
  std::string flag {};

  size_t month {1};
  size_t day {1};
  size_t numberOfLaps {5};
  size_t startingGrid {2};

  std::map <std::string, bool> processedKeys
  {
    {"name", false},
    {"shortname", false},
    {"trackDirectory", false},
    {"flag", false},
    {"month", false},
    {"day", false},
    {"numberOfLaps", false},
    {"startingGrid", false},
  };


  GplEvent() = default;


  bool complete() const;
  std::vector <std::string> serialize() const;

  ParseResult parse(
    const std::string& key,
    const std::string& value );
};


bool
GplEvent::complete() const
{
  for ( auto&& [key, processed] : processedKeys )
    if ( key != "flag" && processed == false )
      return false;

  return true;
}

std::vector <std::string>
GplEvent::serialize() const
{
  std::vector <std::string> result
  {
    "name=" + name,
    "shortname=" + shortname,
    "trackDirectory=" + trackDirectory,
    "month=" + std::to_string(month),
    "day=" + std::to_string(day),
    "numberOfLaps=" + std::to_string(numberOfLaps),
    "startingGrid=" + std::to_string(startingGrid),
  };

  if ( processedKeys.at("flag") == true )
    result.insert(result.begin() + 3, "flag=" + flag);

  return result;
}

ParseResult
GplEvent::parse(
  const std::string& key,
  const std::string& value )
{
  if ( processedKeys.count(key) == 0 )
    return {false, "Unknown key '" + key + "'"};

  if ( key == "name" )
    name = value;

  else if ( key == "shortname" )
    shortname = value;

  else if ( key == "trackDirectory" )
    trackDirectory = value;

  else if ( key == "flag" )
    flag = value;

  else if ( key == "month" )
  {
    if ( stringToNumber(value, month) == false )
      return {false, "'" + value + "' is not a valid integer"};
  }

  else if ( key == "day" )
  {
    if ( stringToNumber(value, day) == false )
      return {false, "'" + value + "' is not a valid integer"};
  }

  else if ( key == "numberOfLaps" )
  {
    if ( stringToNumber(value, numberOfLaps) == false )
      return {false, "'" + value + "' is not a valid integer"};
  }

  else if ( key == "startingGrid" )
  {
    if ( stringToNumber(value, startingGrid) == false )
      return {false, "'" + value + "' is not a valid integer"};
  }

  processedKeys[key] = true;
  return {true};
}
