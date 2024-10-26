#pragma once

#include <string>


class ParseResult
{
  std::string mMessage {};
  bool mStatus {true};


public:
  ParseResult() = default;

  ParseResult(
    const bool status,
    const std::string& message = {} );

  bool ok() const;

  std::string message() const;
};


ParseResult::ParseResult(
  const bool status,
  const std::string& message )
  : mStatus{status}
  , mMessage{message}
{}

bool
ParseResult::ok() const
{
  return mStatus;
}

std::string
ParseResult::message() const
{
  return mMessage;
}
