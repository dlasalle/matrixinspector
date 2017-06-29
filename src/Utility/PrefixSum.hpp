/**
 * @file PrefixSum.hpp
 * @brief THe PrefixSum class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */





#ifndef MATRIXINSPECTOR_UTILITY_PREFIXSUM_HPP
#define MATRIXINSPECTOR_UTILITY_PREFIXSUM_HPP




#include <cstddef>




namespace MatrixInpsector
{


class PrefixSum
{
  public:
    /**
     * @brief Perform an exclusive prefix sum.
     *
     * @tparam T The type to perform the prefix sum on.
     * @param ptr The array of values to prefix sum.
     * @param n The length of the array.
     *
     * @return The total sum.
     */
    template<typename T>
    static T exclusive(
        T * const ptr,
        size_t const n)
    {
      size_t i;
      T tmp1,tmp2;

      if (n > 0) {
        tmp1 = ptr[0];
        ptr[0] = 0;
        for (i = 1; i < n; ++i) {
          tmp2 = ptr[i];
          ptr[i] = tmp1 + ptr[i-1];
          tmp1 = tmp2;
        }
        return tmp1 + ptr[n-1];
      } else {
        return 0;
      }
    }


    /**
     * @brief Perform an inclusive prefix sum.
     *
     * @tparam T The type to perform the prefix sum on.
     * @param ptr The array of values to prefix sum.
     * @param n The length of the array.
     */
    template<typename T>
    static void inclusive(
        T * const ptr,
        size_t const n)
    {
      size_t i;

      if (n > 0) {
        for (i = 1; i < n; ++i) {
          ptr[i] += ptr[i-1];
        }
      }
    }
};




}




#endif
