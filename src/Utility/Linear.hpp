/**
 * @file Linear.hpp
 * @brief The Linear class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#ifndef MATRIXINSPECTOR_UTILITY_LINEAR_HPP
#define MATRIXINSPECTOR_UTILITY_LINEAR_HPP




#include <cstddef>




namespace MatrixInspector
{


class Linear
{
  public:
    template <typename T>
    static void step(
        T * const data,
        size_t const len,
        T const start = 0,
        T const step = 1)
    {
      if (start == 0 && step == 1) {
        T cur = start;

        for (size_t i = 0; i < len; ++i) {
          data[i] = cur;
          cur += step;
        }
      } else {
        for (size_t i = 0; i < len; ++i) {
          data[i] = static_cast<T>(i);
        }
      }
    }




};




}




#endif
