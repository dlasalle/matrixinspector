/**
 * @file StringTest.cpp
 * @brief Unit tests for the String class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#include <string>
#include "Test/UnitTest.hpp"
#include "Utility/String.hpp"




using namespace MatrixInspector;




namespace Test
{


TEST
{
  int x = 9000000;

  std::string str = String::addThousandsSeparators(x);

  testStringEquals(str,"9,000,000");
}



}


