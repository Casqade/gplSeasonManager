#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <functional>
#include <set>

#include <GplEvent.hpp>
#include <GplSeason.hpp>
#include <GplTrack.hpp>
#include <Defaults.hpp>


std::string&
stringTrimLeft(
  std::string& str,
  const std::string& trim_chars = "\t\v\f; " )
{
  return str.erase(0, str.find_first_not_of(trim_chars));
}

std::string&
stringTrimRight(
  std::string& str,
  const std::string& trim_chars = "\t\v\f; " )
{
  return str.erase(str.find_last_not_of(trim_chars) + 1);
}

std::string&
stringTrim(
  std::string& str,
  const std::string& trim_chars = "\t\v\f; " )
{
  return stringTrimLeft(stringTrimRight(str, trim_chars), trim_chars);
}

using IniParseHandler = std::function <bool(
  const std::string& section,
  const std::string& key,
  const std::string& value,
  const size_t line )>;

using IniErrorHandler = std::function <void(
  const size_t line,
  const std::string& message )>;

bool
iniParse(
  const std::filesystem::path& iniPath,
  const IniParseHandler& parseHandler,
  const IniErrorHandler& errorHandler )
{
  std::ifstream file {};

  if ( fileExists(iniPath) == false )
  {
    std::cerr << "Failed to open '" << iniPath.string() << "': file not found\n";
    return false;
  }

  file.open(iniPath);

  if ( file.is_open() == false )
  {
    std::cerr << "Failed to open '" << iniPath.string() << "'\n";
    return false;
  }

  bool fileIsEmpty {true};
  std::string currentSection {};
  std::string currentKey {};
  std::set <std::string> sections {};
  std::set <std::string> keys {};

  size_t currentLine {1};

  for ( std::string line {};
        std::getline(file, line);
        ++currentLine )
  {
    line = line.substr(0, line.find(";"));

    if ( stringTrim(line).empty() == true )
      continue;

    fileIsEmpty = false;

    if ( line.front() == '[' )
    {
      if ( line.find("]") == std::string::npos )
      {
        errorHandler(currentLine, "Section name must be enclosed in square brackets ([])");
        return false;
      }

      if ( stringTrim(line, "[] ").empty() == true )
      {
        errorHandler(currentLine, "Empty section names are not allowed");
        return false;
      }

      if ( sections.count(line) > 0 )
      {
        errorHandler(currentLine, "Duplicate section '" + line + "'");
        return false;
      }

      currentSection = line;
      sections.insert(line);
      keys.clear();

      continue;
    }

    if ( currentSection.empty() == true )
    {
      errorHandler(currentLine, "Orphan items outside sections are not allowed");
      return false;
    }

    const auto delimPos = line.find("=");

    if ( delimPos == std::string::npos )
    {
      errorHandler(currentLine, "Key-value format violation");
      return false;
    }

    auto key = line.substr(0, delimPos);
    auto value = line.substr(delimPos + 1);

    stringTrim(key);
    stringTrim(value);

    if (  key.empty() == true ||
          value.empty() == true )
    {
      errorHandler(currentLine, "Key-value format violation");
      return false;
    }

    if ( keys.count(key) > 0 )
    {
      errorHandler(currentLine, "Duplicate key '" + key + "'");
      return false;
    }

    currentKey = key;
    keys.insert(key);

    if ( parseHandler(currentSection, key, value, currentLine) == false )
      break;
  }

  if ( fileIsEmpty == true )
  {
    std::cerr << "'" << iniPath.string() << "' is empty\n";
    return false;
  }

  return true;
}


void
printHelp()
{
  std::cout <<
R"help(gplSeasonManager - Grand Prix Legends Season Manager

Usage: gplSeasonManager [options] <command> [<args>]

Examples:
  gplSeasonManager enable rouen spa67
  gplSeasonManager disable mexico silver
  gplSeasonManager -s=customSeason.ini enable monza10k nepliget
  gplSeasonManager --season=2024season.ini list


Commands:
  enable  [track...]    Add one or more tracks to the season
  disable [track...]    Remove one or more tracks from the season
  restore               Rewrite season with original 67season.ini contents
  list                  List currently enabled tracks in a season


Options:
  -h --help             Show this help

  -s=FILENAME           Use specified file for operations
  --season=FILENAME     instead of default 67season.ini
                        (File must be in seasons/ subdirectory)
)help";
}


int
main(
  int argc,
  char* argv[] )
{
  std::vector <std::string> args {};

  for ( int i = 0; i < argc; ++i )
    args.push_back(argv[i]);

  const std::filesystem::path pathToExe {args.front()};
  args.erase(args.begin());


  enum class CommandType
  {
    Unknown,
    Enable,
    Disable,
    Restore,
    List,

  } command {};

  std::string seasonFilename {"67season.ini"};
  float raceDistance {210.0};
  std::vector <std::string> tracks {};


  for ( const auto& arg : args )
  {
    if ( command != CommandType::Unknown )
    {
      tracks.push_back(arg);
      continue;
    }

    if (  arg == "-h" ||
          arg == "--help" )
      break;

    if (  arg.find("-s") != std::string::npos ||
          arg.find("--season") != std::string::npos )
    {
      const auto delimPos = arg.find("=");

      if (  delimPos != std::string::npos &&
            delimPos < arg.size() - 1 )
      {
        const auto season = arg.substr(delimPos + 1);

        if ( season.empty() == false )
        {
          seasonFilename = season;
          continue;
        }
      }
    }

    if ( arg == "restore" )
    {
      command = CommandType::Restore;
      break;
    }

    if ( arg == "list" )
    {
      command = CommandType::List;
      break;
    }

    if ( arg == "enable" )
    {
      command = CommandType::Enable;
      continue;
    }

    if ( arg == "disable" )
    {
      command = CommandType::Disable;
      continue;
    }

    auto argType = "option";

    if ( arg[0] == '-' )
      argType = "command";

    std::cerr << "Unknown " << argType << " '" << arg << "'\n";

    return -1;
  }


  if ( command == CommandType::Unknown )
  {
    printHelp();
    return 0;
  }


  const auto seasonPath = pathToExe.parent_path() / "seasons" / seasonFilename;

  GplSeason season {};
  std::vector <GplEvent> events {};

  std::fstream file {};

  if ( command == CommandType::Restore )
  {
    season = OriginalSeason();
    events = OriginalEvents();
  }
  else
  {
    bool parseFailed {};

    const auto seasonHandler =
    [&season, &seasonPath, &events, &parseFailed] (
      const std::string& section,
      const std::string& key,
      const std::string& value,
      const size_t line )
    {
      static std::string lastSection {};
      static bool seasonSectionProcessed {};
      static GplEvent event {};

      if ( section == "Season" )
      {
        const auto status = season.parse(key, value);

        if ( status.ok() == false )
        {
          std::cerr << "Failed to parse season " << seasonPath.string()
                    << ":" << line << " - "
                    << status.message() << "\n";

          parseFailed = true;
          return false;
        }

        seasonSectionProcessed = season.complete();
      }
      else if ( section.find("Event") != std::string::npos )
      {
        if ( seasonSectionProcessed == false )
        {
          std::cerr << "Failed to parse " << seasonPath.string() << ":" << line
                    << " - " << "Unexpected Event section '" << section
                    << "', Event sections must be placed after Season section\n";

          parseFailed = true;
          return false;
        }

        if (  lastSection.empty() == false &&
              section != lastSection )
        {
          if ( event.complete() == true )
            event = {};

          else
          {
            std::cerr << "Failed to parse " << seasonPath.string()
                      << ":" << line << " - " << "Event '"
                      << lastSection << "' is incomplete\n";

            parseFailed = true;
            return false;
          }
        }

        const auto status = event.parse(key, value);

        if ( status.ok() == false )
        {
          std::cerr << "Failed to parse " << seasonPath.string()
                    << ":" << line << " - "
                    << status.message() << "\n";

          parseFailed = true;
          return false;
        }

        if ( event.complete() == true )
          events.push_back(event);

        lastSection = section;
      }
      else
      {
        std::cerr << "Failed to parse " << seasonPath.string()
                  << ":" << line << " - Unknown section '"
                  << section << "'\n";

        parseFailed = true;
        return false;
      }

      return true;
    };

    const auto errorHandler =
    [&seasonPath] (
      const size_t line,
      const std::string& message )
    {
      std::cerr << "Failed to parse " << seasonPath.string()
                << ":" << line << " - "
                << message << "\n";
    };

    if (  iniParse(seasonPath, seasonHandler, errorHandler) == false ||
          parseFailed == true )
      return -1;

    season.numEvents = events.size();
  }

  switch (command)
  {
    case CommandType::Enable:
    {
      for ( const auto& trackDir : tracks )
      {
        const auto track = std::find_if(
          events.begin(), events.end(),
          [trackDir] ( const GplEvent& event )
          {
            return event.trackDirectory == trackDir;
          });

        if ( track != events.end() )
          continue;

        const auto trackPath = std::filesystem::path{"tracks"} / trackDir;
        const auto eventIniPath = trackPath / "event.ini";

        if ( fileExists(trackPath) == false )
        {
          std::cerr << "Track '" << trackPath.string() << "' does not exist\n";
          return -1;
        }

        GplEvent event {};

        if ( fileExists(eventIniPath) == true )
        {
          bool parseFailed {};

          const auto eventHandler =
          [&event, &eventIniPath, &events, &parseFailed] (
            const std::string& section,
            const std::string& key,
            const std::string& value,
            const size_t line )
          {
            static std::string lastSection {};

            if ( section != "Event" )
            {
              std::cerr << "Failed to parse " << eventIniPath.string()
                        << ":" << line << " - Unknown section '" << section << "'. "
                        << "Only a single [Event] section is allowed in event.ini\n";

              parseFailed = true;
              return false;
            }

            if (  lastSection.empty() == false &&
                  section != lastSection &&
                  event.complete() == false )
            {
              std::cerr << "Failed to parse " << eventIniPath.string()
                        << ":" << line << " - Event is incomplete\n";

              parseFailed = true;
              return false;
            }

            const auto status = event.parse(key, value);

            if ( status.ok() == false )
            {
              std::cerr << "Failed to parse " << eventIniPath.string()
                        << ":" << line << " - "
                        << status.message() << "\n";

              parseFailed = true;
              return false;
            }

            if ( event.complete() == true )
            {
              lastSection.clear();
              return false;
            }

            lastSection = section;

            return true;
          };

          const auto errorHandler =
          [&eventIniPath] (
            const size_t line,
            const std::string& message )
          {
            std::cerr << "Failed to parse " << eventIniPath.string()
                      << ":" << line << " - "
                      << message << "\n";
          };

          if (  iniParse(eventIniPath, eventHandler, errorHandler) == false ||
                parseFailed == true ||
                event.complete() == false )
            return -1;
        }
        else
        {
          const auto originalEvents = OriginalEvents();

          const auto eventIter = std::find_if(
            originalEvents.begin(), originalEvents.end(),
            [trackDir] ( const GplEvent& originalEvent )
            {
              return originalEvent.trackDirectory == trackDir;
            });

          if ( eventIter == originalEvents.end() )
          {
            std::cerr << "Failed to enable non-Papyrus track '" << trackDir
                      << "': event.ini not found in track directory\n";
            return -1;
          }

          event = *eventIter;
        }

        events.push_back(event);
      }

      season.numEvents = events.size();

      break;
    }

    case CommandType::Disable:
    {
      for ( const auto& trackDir : tracks )
      {
        const auto event = std::find_if(
          events.begin(), events.end(),
          [trackDir] ( const GplEvent& event )
          {
            return event.trackDirectory == trackDir;
          });

        if ( event != events.end() )
          events.erase(event);
        else
          std::cout << "Faled to disable track '"<< trackDir
                    << "': No such track in '" << seasonPath.string() << "'\n";
      }

      season.numEvents = events.size();

      break;
    }

    case CommandType::Restore:
      break;

    case CommandType::List:
    {
      std::cout << "Found " << season.numEvents
                << " tracks in season '" << seasonFilename << "':\n";

      const size_t maxDigits =
        std::to_string(events.size()).size();

      size_t eventIndex {};
      for ( const auto& event : events )
      {
        auto eventIndexString =
          std::to_string(++eventIndex);

        eventIndexString.insert( // padding
          0, maxDigits - eventIndexString.size(), ' ' );

        std::cout << "  " << eventIndexString << ": " << event.trackDirectory
                  << " (" << event.name << " / " << event.shortname << ")\n";
      }

      return 0;
    }

    default:
    {
      printHelp();
      return 0;
    }
  }

  file.open(seasonPath, std::ios::out | std::ios::trunc);

  if ( file.is_open() == false )
  {
    std::cerr << "Failed to open '" << seasonPath << "'" << "\n";
    return -1;
  }


  file << "[Season]" << "\n";

  for ( const auto& line : season.serialize() )
    file << line << "\n";

  file << "\n";

  size_t eventId {};

  for ( const auto& event : events )
  {
    file << "[Event" << ++eventId << "]" << "\n";

    for ( const auto& line : event.serialize() )
      file << line << "\n";

    file << "\n";
  }


  return 0;
}
