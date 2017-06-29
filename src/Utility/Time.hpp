/**
 * @file Time.hpp
 * @brief The Time class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2015
 * @version 1
 */




#ifndef MATRIXINSPECTOR_UTILITY_TIME_HPP
#define MATRIXINSPECTOR_UTILITY_TIME_HPP




#include <sys/time.h>




namespace MatrixInpsector
{
 

/**
 * @brief Class for getting and manipulating times.
 */
class Time
{
  public:
    static double constexpr MILLESECONDS = 1000.0;
    static double constexpr MICROSECONDS = MILLESECONDS*1000.0;


    /**
     * @brief Get the number of seconds elapsed since Jan 1st. 1970 GMT.
     *
     * @return The number of elapsed seconds.
     */
    static uint64_t now() noexcept;


    /**
     * @brief Return a double representing the current time stamp. This stamp
     * is only guarenteed to be increasing for the duration of this program,
     * and substracting two stamps will result in the number of seconds between
     * them.
     *
     * @return The current timestamp. 
     */
    static inline double stamp() noexcept
    {
      struct timeval tv;
      gettimeofday(&tv,nullptr);

      return static_cast<double>(tv.tv_sec) + (static_cast<double>(tv.tv_usec)/MICROSECONDS);
    }


    /**
     * @brief Sleep the current thread for the given number of seconds (down to
     * a resolution of microseconds).
     *
     * @param sec The number of seconds to sleep.
     */
    static void sleep(
        double sec) noexcept;
};




}




#endif
