/**
 * @file Time.hpp
 * @brief The Time class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2015
 * @version 1
 */




#ifndef MATRIXINSPECTOR_UTILITY_TIME_HPP
#define MATRIXINSPECTOR_UTILITY_TIME_HPP




#include <chrono>




namespace MatrixInspector
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
     * @brief Return a double representing the current time stamp. This stamp
     * is only guarenteed to be increasing for the duration of this program,
     * and substracting two stamps will result in the number of seconds between
     * them.
     *
     * @return The current timestamp. 
     */
    static inline double stamp() noexcept
    {
      std::chrono::high_resolution_clock::time_point time =
          std::chrono::high_resolution_clock::now();
			size_t const ticks = time.time_since_epoch().count();
			double constexpr SECONDS_PER_TICK = \
					static_cast<double>(std::chrono::system_clock::period::num) / \
					static_cast<double>(std::chrono::system_clock::period::den);
 			double const seconds = ticks * SECONDS_PER_TICK;
			return seconds;
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
