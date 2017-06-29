/**
 * @file String.cpp
 * @brief Implementation of the String class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#include <algorithm>
#include "String.hpp"




namespace MatrixInpsector
{


/******************************************************************************
* PUBLIC STATIC FUNCTIONS *****************************************************
******************************************************************************/


std::vector<std::string> String::split(
    std::string const & str,
    std::string const & delim) noexcept
{
  std::vector<std::string> chunks;

  size_t pos;
  size_t lastPos = 0;
  while ((pos = str.find(delim,lastPos)) != std::string::npos) {
    if (lastPos < pos) {
      size_t const len = pos - lastPos;
      chunks.emplace_back(str.substr(lastPos,len));
    }
    lastPos = pos + delim.length();
  }
  if (str.length() > lastPos) {
    chunks.emplace_back(str.substr(lastPos));
  }

  return chunks;
}


bool String::contains(
    std::string const & str,
    std::string const & substr) noexcept
{
  if (str.find(substr) != std::string::npos) {
    return true;
  } else {
    return false;
  }
}


size_t String::count(
    std::string const & str,
    std::string const & substr) noexcept
{
  size_t count = 0;

  size_t pos;
  size_t lastPos = 0;
  while ((pos = str.find(substr,lastPos)) != std::string::npos) {
    if (lastPos < pos) {
      ++count;
    }
    lastPos = pos + substr.length();
  }
  if (str.length() > 0) {
    ++count;
  }

  return count;
}


std::string String::toLower(
    std::string const * const str) noexcept
{
  std::string lower(*str);

  std::transform(lower.begin(),lower.end(),lower.begin(), ::tolower);

  return lower;
}




}
