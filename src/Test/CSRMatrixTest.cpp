/**
* @file CSRMatrixTest.cpp
* @brief Unit tests for the CSRMatrix class. 
* @author Dominique LaSalle <dominique@solidlake.com>
* Copyright 2018
* @version 1
* @date 2018-02-04
*/


#include "Test/UnitTest.hpp"
#include "Data/CSRMatrix.hpp"


using namespace MatrixInspector;


namespace Test
{

TEST
{
  CSRMatrix mat(5,5,7);

  index_type * const offsets = mat.getOffsets();
  offsets[0] = 0;
  offsets[1] = 1;
  offsets[2] = 3;
  offsets[3] = 4;
  offsets[4] = 6;
  offsets[5] = 7;

  dim_type * const columns = mat.getColumns();
  columns[0] = 0;
  columns[1] = 1;
  columns[2] = 3;
  columns[3] = 2;
  columns[4] = 1;
  columns[5] = 3;
  columns[6] = 4;

  value_type * const values = mat.getValues();
  values[0] = 2.0;
  values[1] = 1.0;
  values[2] = 5.0;
  values[3] = 3.0;
  values[4] = 5.0;
  values[5] = 6.0;
  values[6] = 7.0;

  // compute symmetry
  mat.computeSymmetry()

  // check for symmetry
  testEquals(mat.isSymmetric(), true);
}

}
