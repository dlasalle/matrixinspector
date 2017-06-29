/**
 * @file Random.hpp
 * @brief The Random class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#ifndef MATRIXINSPECTOR_UTILITY_RANDOM_HPP
#define MATRIXINSPECTOR_UTILITY_RANDOM_HPP




#include <cstddef>
#include <algorithm>
#include <ctime>
#include "Utility/Debug.hpp"




namespace MatrixInpsector
{


class Random
{
  public:
    /**
     * @brief Set the seed for random number generation.
     *
     * @param seed The seed.
     */
    static void setSeed(
        unsigned int const seed = \
            static_cast<unsigned int>(std::time(nullptr)))
    {
      std::srand(seed);
    }

    /**
     * @brief Generate a random number in a specified range.
     *
     * @tparam T The type of number to generate.
     * @param min The lower bound on the range (inclusive).
     * @param max The upper bound on the range (inclusive).
     *
     * @return The generated number.
     */
    template<typename T>
    static T inRange(
        T const min,
        T const max)
    {
      ASSERT_LESSEQUAL(min,max);
      T val;
      T upper = max - min;
      if (upper == 0) {
        val = min;
      } else if (upper + 1 == 0) {
        val = min + std::rand();
      } else {
        val =  min + (std::rand() % (upper+1));
      }
      ASSERT_GREATEREQUAL(val,min);
      ASSERT_LESSEQUAL(val,max);

      return val;
    }


    template<typename T>
    static void permutation(
        T * const perm,
        size_t const len)
    {
      if (len == 0) {
        return;
      }

      // fill the array
      for (size_t i = 0; i < len; ++i) {
        perm[i] = i;
      }

      // TODO: compare speeds
      #if 0
      // shuffle array
      if (len < 100) {
        for (size_t i = 0; i < len; ++i) {
          u = size_rand_r(0,n,seed);
          v = size_rand_r(0,n,seed);
          size_t const u = inRange(0,len-1);
          size_t const v = inRange(0,len-1);
          std::swap(ptr[u],ptr[v]);
        }
      } else {
        size_t const nshuffles = len / 8;
        size_t const r = ((len-3) / (double)nshuffles);
        size_t d = 0;
        for (size_t i=0; i < nshuffles; ++i) {
          size_t const u = (size_t)d;
          size_t const v = inRange(0,len-4);
          std::swap(perm[u],perm[v+1]);
          std::swap(perm[u+1],perm[v+3]);
          std::swap(perm[u+2],perm[v+0]);
          std::swap(perm[u+3],perm[v+2]);
          d += r;
        }
      }
      #else
      std::random_shuffle(perm,perm+len);
      #endif
    }


    template<typename T>
    static void sample(
        T * const set,
        size_t const len,
        size_t const sampleLen)
    {
      ASSERT_LESSEQUAL(sampleLen,len);

      // do nothing for empty sample lenths of sets
      if (len == 0 || sampleLen == 0) {
        return;
      }

      // TODO: add optimization such that if sampleLen > len/2, we remove
      // elements rather than add

      for (size_t i = 0; i < sampleLen; ++i) {
        size_t const j = inRange(i,len-1);
        std::swap(set[i],set[j]);
      }
    }




};




}




#endif
