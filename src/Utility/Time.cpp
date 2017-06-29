/**
 * @file Time.cpp
 * @brief Implementation of the Time class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2015
 * @version 1
 */




#include <cstdint>
#include <chrono>
#include <thread>
#include "Time.hpp"




namespace MatrixInpsector
{


/******************************************************************************
* STATIC FUNCTIONS ************************************************************
******************************************************************************/


uint64_t Time::now() noexcept
{
  struct timeval tv;
  gettimeofday(&tv,nullptr);

  return static_cast<uint64_t>(tv.tv_sec);
}


void Time::sleep(
    double const sec) noexcept
{
  // convert to microseconds
  uint64_t ms = static_cast<uint64_t>(sec * MICROSECONDS);

  // sleep
  std::this_thread::sleep_for(std::chrono::microseconds(ms));
}




} 
