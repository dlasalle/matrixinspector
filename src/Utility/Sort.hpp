/**
 * @file Sort.hpp
 * @brief The Sort class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#ifndef MATRIXINSPECTOR_UTILITY_SORT_HPP
#define MATRIXINSPECTOR_UTILITY_SORT_HPP




#include <algorithm>
#include <vector>
#include "Utility/PrefixSum.hpp"




namespace MatrixInspector
{


class Sort
{
  public:
    template<typename K, typename V>
    static void keyValue(
        K const * const keys,
        V * const values,
        size_t const len,
        bool ascending = true)
    {
      // don't process empty things
      if (len == 0) {
        return;
      }

      // find the range of keys
      K max = keys[0];
      K min = keys[0];
      for (size_t i = 1; i < len; ++i) {
        if (keys[i] > max) {
          max = keys[i];
        } else if (keys[i] < min) {
          min = keys[i];
        }
      }

      // add one to get the actual range
      size_t const size = (max - min)+1;

      if (std::is_floating_point<K>::value || size > len) {
        keyValueSmallInteger(keys,values,len,min,max,ascending);
      } else {
        keyValueGeneric(keys,values,len,ascending);
      }
    }


  private:
    template <typename K, typename V>
    struct kv_struct
    {
      K k;
      V v;

      bool operator<(
          const kv_struct & rhs) {
        return k < rhs.k;
      }
    };


    template <typename T>
    struct compareAscending
    {
      bool operator()(
          T const & lhs,
          T const & rhs)
      {
        return lhs.k < rhs.k;
      }
    };


    template <typename T>
    struct compareDescending
    {
      bool operator()(
          T const & lhs,
          T const & rhs)
      {
        return lhs.k > rhs.k;
      }
    };


    template<typename K, typename V>
    static void keyValueSmallInteger(
        K const * const keys,
        V * const values,
        size_t const len,
        K const min,
        K const max,
        bool ascending)
    {
      K const range = max - min + 1;

      // allocate an extra space for prefixsum
      std::vector<size_t> counts(range+1,0);

      // allocate values swap vector
      std::vector<V> swap(values,values+len);

      if (ascending) {
        // avoid having to do offsets in each iteration
        size_t * const start = counts.data() - static_cast<ptrdiff_t>(min);

        for (size_t i = 0; i < len; ++i) {
          size_t const bucket = keys[i];
          ++start[bucket];
        }

        // find out where our keys and values will go (start)
        PrefixSum::exclusive(counts.data(),range+1);

        for (size_t i = 0; i < len; ++i) {
          size_t const bucket = keys[i];
          values[start[bucket]++] = swap[i];
        }
      } else {
        for (size_t i = 0; i < len; ++i) {
          size_t const bucket = max - keys[i];
          ++counts[bucket];
        }

        // find out where our keys and values will go (start)
        PrefixSum::exclusive(counts.data(),range+1);

        for (size_t i = 0; i < len; ++i) {
          size_t const bucket = max - keys[i];
          values[counts[bucket]++] = swap[i];
        }
      }
    }


    template<typename K, typename V>
    static void keyValueGeneric(
        K const * const keys,
        V * const values,
        size_t const len,
        bool ascending)
    {
      std::vector<kv_struct<K,V>> kvs(len);

      // create KVs
      for (size_t i = 0; i < len; ++i) {
        kvs[i].k = keys[i];
        kvs[i].v = values[i];
      }

      if (ascending) {
        compareAscending<kv_struct<K,V>> comp;
        std::sort(kvs.begin(),kvs.end(),comp);
      } else {
        compareDescending<kv_struct<K,V>> comp;
        std::sort(kvs.begin(),kvs.end(),comp);
      }

      // save sorting
      for (size_t i = 0; i < len; ++i) {
        values[i] = kvs[i].v;
      }
    }


};




}




#endif
