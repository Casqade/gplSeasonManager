#pragma once

#include <GplEvent.hpp>
#include <GplSeason.hpp>


std::vector <GplEvent>
OriginalEvents()
{
  return
  {
//    Event1
    {
      .name = "South African Grand Prix",
      .shortname = "South African GP",
      .trackDirectory = "kyalami",
      .month = 1,
      .day = 2,
      .numberOfLaps = 80,
      .startingGrid = 4,
    },
//    Event2
    {
      .name = "Mexican Grand Prix",
      .shortname = "Mexican GP",
      .trackDirectory = "mexico",
      .month = 10,
      .day = 22,
      .numberOfLaps = 65,
      .startingGrid = 4,
    },
//    Event3
    {
      .name = "Monaco Grand Prix",
      .shortname = "Monaco GP",
      .trackDirectory = "monaco",
      .month = 5,
      .day = 7,
      .numberOfLaps = 100,
      .startingGrid = 4,
    },
//    Event4
    {
      .name = "Italian Grand Prix",
      .shortname = "Italian GP",
      .trackDirectory = "monza",
      .month = 9,
      .day = 10,
      .numberOfLaps = 68,
      .startingGrid = 6,
    },
//    Event5
    {
      .name = "Canadian Grand Prix",
      .shortname = "Canadian GP",
      .trackDirectory = "mosport",
      .month = 8,
      .day = 27,
      .numberOfLaps = 90,
      .startingGrid = 6,
    },
//    Event6
    {
      .name = "German Grand Prix",
      .shortname = "German GP",
      .trackDirectory = "nurburg",
      .month = 8,
      .day = 6,
      .numberOfLaps = 15,
      .startingGrid = 9,
    },
//    Event7
    {
      .name = "French Grand Prix",
      .shortname = "French GP",
      .trackDirectory = "rouen",
      .month = 7,
      .day = 2,
      .numberOfLaps = 60,
      .startingGrid = 6,
    },
//    Event8
    {
      .name = "British Grand Prix",
      .shortname = "British GP",
      .trackDirectory = "silver",
      .month = 7,
      .day = 15,
      .numberOfLaps = 80,
      .startingGrid = 10,
    },
//    Event9
    {
      .name = "Belgian Grand Prix",
      .shortname = "Belgian GP",
      .trackDirectory = "spa",
      .month = 6,
      .day = 18,
      .numberOfLaps = 28,
      .startingGrid = 5,
    },
//    Event10
    {
      .name = "United States Grand Prix",
      .shortname = "U.S.A. GP",
      .trackDirectory = "watglen",
      .month = 10,
      .day = 1,
      .numberOfLaps = 108,
      .startingGrid = 4,
    },
//    Event11
    {
      .name =  "Dutch Grand Prix",
      .shortname = "Dutch GP",
      .trackDirectory = "zandvort",
      .month = 6,
      .day = 4,
      .numberOfLaps = 90,
      .startingGrid = 6,
    },
  };
}

GplSeason
OriginalSeason()
{
  return
  {
    .name = "All Tracks In Alphabetical Order",
    .year = 1967,
    .numEvents = OriginalEvents().size(),
  };
}
