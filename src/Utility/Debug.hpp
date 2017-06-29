/**
 * @file Debug.hpp
 * @brief Debugging functions
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017, Dominique LaSalle
 * @version 1
 */




#ifndef MATRIXINSPECTOR_UTILITY_DEBUG_HPP
#define MATRIXINSPECTOR_UTILITY_DEBUG_HPP




#include <cassert>
#include <iostream>




/******************************************************************************
* MACROS **********************************************************************
******************************************************************************/


#ifndef NDEBUG
  #define ASSERT_TRUE(a) \
    do { \
      if (!a) { \
        std::cerr << "("#a") is false" << std::endl; \
        assert(false); \
      } \
    } while (false)
  #define ASSERT_FALSE(a) \
    do { \
      if (a) { \
        std::cerr << "("#a") is true" << std::endl; \
        assert(false); \
      } \
    } while (false)
  #define ASSERT_EQUAL(a,b) \
    do { \
      if (a != b) { \
        std::cerr << "("#a" = " << a << ") != ("#b" = " << b << ")" << \
            std::endl; \
        assert(false); \
      } \
    } while (false)
  #define ASSERT_NOTEQUAL(a,b) \
    do { \
      if (a == b) { \
        std::cerr << "("#a" = " << a << ") == ("#b" = " << b << ")" << \
            std::endl; \
        assert(false); \
      } \
    } while (false)
  #define ASSERT_NOTNULL(a) \
    do { \
      if (a == nullptr) { \
        std::cerr << "("#a") is null " << std::endl; \
        assert(false); \
      } \
    } while (false)
  #define ASSERT_LESS(a,b) \
    do { \
      if (a >= b) { \
        std::cerr << "("#a" = " << a << ") !< ("#b" = " << b << ")" << \
            std::endl; \
        assert(false); \
      } \
    } while (false)
  #define ASSERT_LESSEQUAL(a,b) \
    do { \
      if (a > b) { \
        std::cerr << "("#a" = " << a << ") !<= ("#b" = " << b << ")" << \
            std::endl; \
        assert(false); \
      } \
    } while (false)
  #define ASSERT_GREATER(a,b) \
    do { \
      if (a <= b) { \
        std::cerr << "("#a" = " << a << ") !> ("#b" = " << b << ")" << \
            std::endl; \
        assert(false); \
      } \
    } while (false)
  #define ASSERT_GREATEREQUAL(a,b) \
    do { \
      if (a < b) { \
        std::cerr << "("#a" = " << a << ") !>= ("#b" = " << b << ")" << \
            std::endl; \
        assert(false); \
      } \
    } while (false)


#else
  #define ASSERT_TRUE(a) assert(false)
  #define ASSERT_FALSE(a) assert(false)
  #define ASSERT_EQUAL(a,b) assert(false)
  #define ASSERT_NOTEQUAL(a,b) assert(false)
  #define ASSERT_NOTNULL(a) assert(false)
  #define ASSERT_LESS(a,b) assert(false)
  #define ASSERT_LESSEQUAL(a,b) assert(false)
  #define ASSERT_GREATER(a,b) assert(false)
  #define ASSERT_GREATEREQUAL(a,b) assert(false)
#endif




#endif
