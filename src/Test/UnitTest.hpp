/**
 * @file UnitTest.hpp
 * @brief Unit testing framework.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2015
 * @version 1
 */




#ifndef DOMGEON_TEST_UNITTEST_HPP
#define DOMGEON_TEST_UNITTEST_HPP




#include <exception>
#include <iostream>




/******************************************************************************
* MACROS **********************************************************************
******************************************************************************/


#define testEquals(a,b) \
  if ((a) != (b)) { \
    std::cerr << "Test Failed: " << #a << ":'" << a << "' != " << #b << ":'" \
        << b << "' at " << __LINE__ << std::endl; \
    throw TestFailed("Equals Failed"); \
  }


#define testStringEquals(a,b) \
  if ((a).compare(b) != 0) { \
    std::cerr << "Test Failed: " << #a << ":'" << a << "' != " << #b << ":'" \
        << b << "' at " << __LINE__ << std::endl; \
    throw TestFailed("Equals Failed"); \
  }


#define testTrue(a) \
  if (!(a)) { \
    std::cerr << "Test Failed: '" << #a << "' is false at: " << __LINE__ \
      << std::endl; \
    throw TestFailed("True Failed"); \
  }


#define testGreaterThan(a,b) \
  if ((a) <= (b)) { \
    std::cerr << "Test Failed:" << #a << ":'" << a << "' <= " << #b << ":'" \
        << b << "' at " << __LINE__ << std::endl; \
    throw TestFailed("Greater Than Failed"); \
  }


#define testGreaterThanOrEqual(a,b) \
  if ((a) < (b)) { \
    std::cerr << "Test Failed:" << #a << ":'" << a << "' < " << #b << ":'" \
        << b << "' at " << __LINE__ << std::endl; \
    throw TestFailed("Greater Than Or Equal Failed"); \
  }

#define testLessThan(a,b) \
  if ((a) >= (b)) { \
    std::cerr << "Test Failed:" << #a << ":'" << a << "' >= " << #b << ":'" \
        << b << "' at " << __LINE__ << std::endl; \
    throw TestFailed("Less Than Failed"); \
  }


#define testLessThanOrEqual(a,b) \
  if ((a) > (b)) { \
    std::cerr << "Test Failed:" << #a << ":'" << a << "' > " << #b << ":'" \
        << b << "' at " << __LINE__ << std::endl; \
    throw TestFailed("Less Than Or Equal Failed"); \
  }




namespace Test
{

class TestFailed : public std::logic_error
{
  public:
    TestFailed(std::string const & str) :
        std::logic_error(str)
    {
    }
};




class UnitTest
{
  public:
    UnitTest()
    {
    }

    bool evaluate()
    {
      try {
        run();
      } catch (std::exception & e) {
        std::cerr << "FAILED: " << e.what() << std::endl;
        // fail
        return false;
      }

      // pass
      return true;
    };

    virtual ~UnitTest() 
    {
    }

  protected:
    void run();
};


}


#define TEST \
  void UnitTest::run()

int main(
    int argc,
    char ** argv)
{
  // make sure we don't have any useless arguments
  if (argc > 1) {
    for (int i=1; i<argc; ++i) {
      std::cerr << "Unused parameter: " << argv[i] << std::endl;
    }
    return 0;
  }

  Test::UnitTest test;

  if (test.evaluate()) {
    return 0;
  } else {
    return 1;
  }
}


#endif
