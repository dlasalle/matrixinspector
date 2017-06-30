/**
 * @file CollisionTest.cpp
 * @brief Unit tests for the Collision class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#include <vector>
#include "Test/UnitTest.hpp"
#include "Utility/Sort.hpp"




using namespace MatrixInspector;




namespace Test
{


TEST
{
  std::vector<int> keys{1,2,0,2,1};

  std::vector<int> valuesAsc{0,1,2,3,4};
  Sort::keyValue(keys.data(),valuesAsc.data(),keys.size(),true);

  testEquals(valuesAsc[0],2);
  testEquals(valuesAsc[1],0);
  testEquals(valuesAsc[2],4);
  testEquals(valuesAsc[3],1);
  testEquals(valuesAsc[4],3);

  std::vector<int> valuesDes{0,1,2,3,4};
  Sort::keyValue(keys.data(),valuesDes.data(),keys.size(),false);

  testEquals(valuesDes[0],1);
  testEquals(valuesDes[1],3);
  testEquals(valuesDes[2],0);
  testEquals(valuesDes[3],4);
  testEquals(valuesDes[4],2);
}




}


