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




namespace MatrixInspector
{


/******************************************************************************
* STATIC FUNCTIONS ************************************************************
******************************************************************************/


void Time::sleep(
    double const sec) noexcept
{
  // convert to microseconds
  uint64_t ms = static_cast<uint64_t>(sec * MICROSECONDS);

  // sleep
  std::this_thread::sleep_for(std::chrono::microseconds(ms));
}




} 
