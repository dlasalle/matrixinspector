/**
 * @file Timer.hpp
 * @brief The Timer class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#ifndef MATRIXINSPECTOR_UTILITY_TIMER_HPP
#define MATRIXINSPECTOR_UTILITY_TIMER_HPP




#include "Utility/Time.hpp"




namespace MatrixInspector
{


class Timer
{
  public:
    Timer() :
      m_last(0),
      m_duration(0)
    {
      // do nothing
    }


    inline void start() noexcept
    {
      m_last = Time::stamp();
    }


    inline void stop() noexcept
    {
      m_duration += Time::stamp() - m_last;
      m_last = 0;
    }


    inline double poll() noexcept
    {
      return m_duration;
    }


  private:
    double m_last;
    double m_duration;




};




}




#endif
